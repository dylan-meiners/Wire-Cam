#include <Windows.h>
#include <string>
#include <iostream>
#include <tchar.h>
#include <stdio.h>

#define WRITE_SLEEP_TIME 22

class SerialPort {

    public:

        SerialPort(const LPCWSTR COMPortToSet, const bool readingToSet, const bool writingToSet) {

            m_COMPortPath = COMPortToSet;
            m_canRead = readingToSet;
            m_canWrite = writingToSet;
        }

        bool Setup() {

            DWORD desiredAccess;

            if (!m_canRead || !m_canWrite) {

                if (m_canRead) {

                    desiredAccess = GENERIC_READ;
                }
                else {

                    desiredAccess = GENERIC_WRITE;
                }
            }
            else {

                desiredAccess = GENERIC_READ | GENERIC_WRITE;
            }

            m_hComm = CreateFile(m_COMPortPath, desiredAccess, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (m_hComm == INVALID_HANDLE_VALUE) {

                std::cout << "Error in opening serial port on " << m_COMPortPath << std::endl;
                return false;
            }
            else {

                std::cout << "Successfully  opened serial port on " << *m_COMPortPath << std::endl;
            }

            SecureZeroMemory(&m_dcb, sizeof(DCB));
            m_dcb.DCBlength = sizeof(DCB);

            bool success = GetCommState(m_hComm, &m_dcb);

            if (!success) {

                std::cout << "GetCommState failed with error " << GetLastError() << std::endl;
                return false;
            }

            //PrintCommState();
            
            m_dcb.BaudRate = CBR_57600;
            m_dcb.ByteSize = 8;
            m_dcb.StopBits = ONESTOPBIT;
            m_dcb.Parity = NOPARITY;
            m_dcb.EvtChar = 0x04;
            success = SetCommState(m_hComm, &m_dcb);

            if (!success) {

                std::cout << "SetCommState failed with error " << GetLastError() << std::endl;
                return false;
            }

            PrintCommState();

            m_timeouts.ReadIntervalTimeout = 50;
            m_timeouts.ReadTotalTimeoutConstant = 50;
            m_timeouts.ReadTotalTimeoutMultiplier = 10;
            m_timeouts.WriteTotalTimeoutConstant = 50;
            m_timeouts.WriteTotalTimeoutMultiplier = 10;

            DWORD dwStoredFlags = EV_RXCHAR;
            success = SetCommMask(m_hComm, dwStoredFlags);

            if (!success) {

                std::cout << "SetCommMask failed with error " << GetLastError() << std::endl;
                return false;
            }

            return true;
        }

        bool Close() {

            return CloseHandle(m_hComm);
        }

        bool WriteBytes(const unsigned char *bytesToWrite, const unsigned int size) {

            DWORD numOfBytesToWrite = size;
            DWORD numOfBytesWritten = 0;

            return WriteFile(m_hComm, bytesToWrite, numOfBytesToWrite, &numOfBytesWritten, NULL);
        }

        bool ReadTheLonelyByte(unsigned int &p_buffer) {

            bool success = false;

            DWORD dwCommEvent;
            WaitCommEvent(m_hComm, &dwCommEvent, NULL);

            DWORD noBytesRead;
            success = ReadFile(m_hComm,
                               &p_buffer,
                               1,
                               &noBytesRead,
                               NULL);
            if (!success) { return false; }
            else { return true; }
        }

        bool ReadBytes(unsigned char * buffer, const unsigned char bytesToRead) {

            bool success = false;

            DWORD dwCommEvent;
            WaitCommEvent(m_hComm, &dwCommEvent, NULL);

            unsigned char tempChar;
            DWORD noBytesRead;
            int i = 0;
            do {
                success = ReadFile(m_hComm,
                                   &tempChar,
                                   sizeof(tempChar),
                                   &noBytesRead,
                                   NULL);
                if (!success) { return false; }

                buffer[i] = tempChar;
                i++;
            } while (i < bytesToRead);
            
            return true;
        }

        void PrintCommState() {

            _tprintf(TEXT("\nBaudRate = %d\nByteSize = %d\nParity = %d\nStopBits = %d\nEventChar = %d\n\n"),
                     m_dcb.BaudRate,
                     m_dcb.ByteSize,
                     m_dcb.Parity,
                     m_dcb.StopBits,
                     m_dcb.EvtChar);
        }

    private:

        HANDLE m_hComm;
        LPCWSTR m_COMPortPath;
        DCB m_dcb;
        COMMTIMEOUTS m_timeouts;
        bool m_canRead;
        bool m_canWrite;
};