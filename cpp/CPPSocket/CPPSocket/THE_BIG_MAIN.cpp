#include "Constants.h"
#include "SerialPort.h"
#include "Winch.h"
#include "WireCam.h"
#include "Joystick.h"
#include "Utils.h"
#include "conio.h"
#include "Socket.h"

SerialPort arduino(TEXT("COM4"), true, true);
Socket frontend("http://localhost:3000");

unsigned char thisToArduinoSendBuffer[2] = {0};
/*
THIS TO ARDUINO BUFFER IS AS FOLLOWS:
{[00000000], [00000000]}
leftWinchSpeedToSet     unsigned char   (1 byte)
rightWinchSpeedToSet    unsigned char   (1 byte)
=================================================
TOTALS                                  2 bytes
*/

unsigned char arduinoRXBuffer[8] = {0};
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
leftPos-1rightPos-2leftDirection-3rightDirection-4arduinoConnection
*/

int main() {

    if (!arduino.Setup()) { 

        std::cout << "FATAL: Unable to setup arduino connection!" << std::endl;    
        return -1;
    }
    else { std::cout << "Successfully connected to arduino." << std::endl; }

    std::cout << "Connecting to frontend..." << std::endl;
    frontend.Connect();

    Winch * leftWinch = new Winch();
    Winch * rightWinch = new Winch(REVERSE);
    WireCam wireCam(leftWinch, rightWinch, thisToArduinoSendBuffer);
    wireCam.Zero();
    wireCam.Stop();

    bool shouldRun = true;
    while (shouldRun) {

        //GET NEW ENCODER DATA

        //GET JOYSTICK DATA SOMEHOW
        
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

        /*if (pilot->trigger) {

            while (true) {

                wireCam.Stop();
                shouldRun = false;
            }
        }*/
        //else if (pilot->twelve) wireCam.Jog(pilot);
        //else if (pilot->trigger) wireCam.FlyByWire(pilot);
        //else if (pilot->eleven) wireCam.JogPulse(pilot, 30);
        //else if (pilot->seven) wireCam.Zero();
        //else wireCam.Stop();
        //wireCam.Stop();
        //wireCam.Stop();
        //std::cout << (int)thisToArduinoSendBuffer[0] << (int)thisToArduinoSendBuffer[1] << std::endl;
        
        thisToArduinoSendBuffer[0] = leftWinch->GotoPosition(20);
        thisToArduinoSendBuffer[1] = rightWinch->Stop();//rightWinch->GotoPosition(20);
        
        arduino.WriteBytes(thisToArduinoSendBuffer, sizeof(thisToArduinoSendBuffer));
        arduino.ReadBytes(arduinoRXBuffer, 8);

        memcpy(&leftEncoderPositionToSet, arduinoRXBuffer, 4);
        memcpy(&rightEncoderPositionToSet, arduinoRXBuffer + 4, 4);
        leftWinch->m_position = leftEncoderPositionToSet;
        rightWinch->m_position = rightEncoderPositionToSet;
        std::cout << "    " << leftEncoderPositionToSet << "    " << rightEncoderPositionToSet << "    "
                  << (unsigned int)thisToArduinoSendBuffer[0] << std::endl;

        thisToFrontendString = std::to_string(leftWinch->m_position) + std::string("_1") +
                               std::to_string(rightWinch->m_position) + std::string("_2") +
                               ((thisToArduinoSendBuffer[0] == 127) ? std::string("idle_3") : ((thisToArduinoSendBuffer[0] < 127) ? std::string("out_3") : std::string("in_3"))) +
                               ((thisToArduinoSendBuffer[1] == 127) ? std::string("idle_4") : ((thisToArduinoSendBuffer[1] < 127) ? std::string("out_4") : std::string("in_4"))) +
                               std::string("connected");
        std::cout << thisToFrontendString << std::endl;
        frontend.SendConsoleData(thisToFrontendString);

        Sleep(5);

        //Uses short-circuiting to determine if a key was pressed and if it was the escape key
        shouldRun = !(_kbhit() && (int)_getch() == 27);
    }

    return 0;
}