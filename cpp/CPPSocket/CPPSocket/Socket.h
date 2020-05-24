#include "sio_client.h"
#include "sio_message.h"
#include <Windows.h>
#include <time.h>
#include <ctime>

class Socket {

    public:
        
        Socket(const std::string address) {

            m_address = address;
        }

        void Connect() {

            m_client.connect(m_address);
            Sleep(100);
            SendString("vc");
        }

        void SendString(const std::string stringToSend, const std::string emitName = "message") {

            m_client.socket()->emit(emitName, stringToSend);
        }

        void SendConsoleData(const std::string data) {

            time_t t = time(NULL);
            struct tm time_info;
            char buffer[32];
            localtime_s(&time_info, &t);
            std::strftime(buffer, 32, "%m/%e/%Y-%H:%M:%S", &time_info);
            SendString(std::string("j") + "message-c" + buffer + " > " + data); //+ "\n");
        }

        void Disconnect() {

            m_client.close();
        }
    
    private:

        std::string m_address;
        sio::client m_client;
};