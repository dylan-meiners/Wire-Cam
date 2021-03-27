#include <conio.h>

#include "Constants.h"
#include "SerialPort.h"
#include "Winch.h"
#include "WireCam.h"
#include "Joystick.h"
#include "Utils.h"
#include "ConfigParser.h"
#include "Socket.h"

int result = system("CLS");

unsigned char thisToArduinoSendBuffer[3] = { 0 };
/*
THIS TO ARDUINO BUFFER IS AS FOLLOWS:
{[00000000], [00000000], [00000000]}
leftWinchSpeedToSet     unsigned char   (1 byte)
rightWinchSpeedToSet    unsigned char   (1 byte)
reset                   unsigned char   (1 byte)
=================================================
TOTALS                                  3 bytes
*/

unsigned char arduinoRXBuffer[8] = { 0 };
int leftEncoderPositionToSet;
int rightEncoderPositionToSet;
/*
ARDUINO RX BUFFER IS AS FOLLOWS:
{[00000000|00000000|00000000|00000000], [00000000|00000000|00000000|00000000]}
leftEncoderPos      int     (4 bytes)
rightEncoderPos     int     (4 bytes)
=============================================
TOTALS                              8 bytes
*/

std::string thisToFrontendString = "";
/*
STRING COMPOSITION IS AS FOLLOWS:
leftPos_1rightPos_2leftDirection_3rightDirection_4arduinoConnection_5x_6x
*/

int mains() {

    std::vector<std::vector<double>> x = { {1, 0, 2, 4, 1}, {0, 1, 0, 4, 2}, {2, 0, 1, 4, 3} };
    std::vector<std::vector<double>> y = rref(x);
    std::cout << "Finished rref" << std::endl;
    PrintVector(y);

    return 0;
}

int main() {

    ConfigParser parser("T:\\Documents\\Coding\\Coding\\Wire-Cam\\MASTER_CONFIG.ini");
    SerialPort arduino(TEXT("COM4"), true, true);
    Socket frontend("http://localhost:3000");

    bool usingInsight;
    parser.GetConfigChoice("USING_INSIGHT", &usingInsight);
    int joystickID;
    parser.GetConfigChoice("JOYSTICK_ID", &joystickID);

    LogitechExtreme3DPro* pilot = _LogitechExtreme3DPro(joystickID, X_DEADZONE, Y_DEADZONE, Z_DEADZONE);

    if (!arduino.Setup()) { 

        std::cout << "FATAL: Unable to setup arduino connection!" << std::endl;    
        return -1;
    }
    else { std::cout << "Successfully connected to arduino." << std::endl; }

    if (usingInsight) {
        
        std::cout << "Connecting to frontend..." << std::endl;
        frontend.Connect();
    }

    std::cout << std::endl;

    Winch* leftWinch = new Winch();
    Winch* rightWinch = new Winch(REVERSE);
    WireCam wireCam(leftWinch, rightWinch, thisToArduinoSendBuffer);
    
    wireCam.Zero();
    wireCam.Stop();
    std::cout << PythagC(WIDTH_FULL_RES / 2.0, HEIGHT_FULL_RES / 2.0) << std::endl;
    bool shouldRun = true;
    while (shouldRun) {

        //CaptureInput(pilot);
        CaptureInput(pilot, true);
        CorrectAxis(pilot, true, true, true, false);
        //CorrectDeadzones(pilot);
        //std::cout << pilot->x << std::endl;
        
        /*
        BUTTON MAPPINGS
        ===========================================================================
        12      -   Jog: If holding thumb button and button 12, leftWinch will jog.
                        If it is just button 12, right winch will jog.
        TRIGGER -   FlyByWire: If holding trigger, the wireCam will move with
                            joytick axis values in real time.  NOT COMPLETED
                            YET.
        11       -  JogPulse: If holding button 11, you must press either button 5
                            or 6, 5 to go a little to the left, or 6 to go a
                            little to the right.  DO NOT HOLD 5 OR 6, just CLICK.
        7       -   Zero: Sets positions to 0
        NONE    -   Stop: Stops the wire cam.
        ===========================================================================
        */

        if (pilot->trigger) {

            wireCam.Stop();
            std::cout << "User stop detected, exiting..." << std::endl;
            return -1;
        }
        else wireCam.Fly(pilot);

        wireCam.UpdateBuffer();
        
        arduino.WriteBytes(thisToArduinoSendBuffer, sizeof(thisToArduinoSendBuffer));
        thisToArduinoSendBuffer[2] = 0; // Reset the reset byte
        arduino.ReadBytes(arduinoRXBuffer, 8);

        memcpy(&leftEncoderPositionToSet, arduinoRXBuffer, 4);
        memcpy(&rightEncoderPositionToSet, arduinoRXBuffer + 4, 4);
        leftWinch->m_position = leftEncoderPositionToSet;
        rightWinch->m_position = rightEncoderPositionToSet;

        if (usingInsight) {
            
            thisToFrontendString = std::to_string(leftWinch->GetPosition()) + std::string("_1") +
                                   std::to_string(rightWinch->GetPosition()) + std::string("_2") +
                                   ((thisToArduinoSendBuffer[0] == 127) ? std::string("idle_3") : ((thisToArduinoSendBuffer[0] < 127) ? std::string("out_3") : std::string("in_3"))) +
                                   ((thisToArduinoSendBuffer[1] == 127) ? std::string("idle_4") : ((thisToArduinoSendBuffer[1] < 127) ? std::string("out_4") : std::string("in_4"))) +
                                   std::string("connected") + std::string("_5") +
                                   std::to_string(wireCam.GetX()) + std::string("_6") +
                                   std::to_string(wireCam.GetY());
            frontend.SendConsoleData(thisToFrontendString);
            Sleep(5);
        }
        if (DEBUG) {

            //Sleep(WRITE_SLEEP_TIME);
            std::cout << "\r" <<
            std::to_string(thisToArduinoSendBuffer[0]) + std::string("    ") +
            std::to_string(thisToArduinoSendBuffer[1]) + std::string("    ") +
            std::to_string(leftWinch->GetPosition()) + std::string("    ") +
            std::to_string(rightWinch->GetPosition()) + std::string("    ") +
            ((thisToArduinoSendBuffer[0] == 127) ? std::string("idle    ") : ((thisToArduinoSendBuffer[0] < 127) ? std::string("out    ") : std::string("in    "))) +
            ((thisToArduinoSendBuffer[1] == 127) ? std::string("idle    ") : ((thisToArduinoSendBuffer[1] < 127) ? std::string("out    ") : std::string("in    "))) +
            std::string("connected    ") +
            std::to_string(wireCam.GetX()) + std::string("    ") +
            std::to_string(wireCam.GetY()) + std::string("    ");
        }

        //Uses short-circuiting to determine if a key was pressed and if it was the escape key
        shouldRun = !(_kbhit() && (int)_getch() == 27) && shouldRun;
    }

    return 0;
}