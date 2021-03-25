#ifndef WIRECAM_H
#define WIRECAM_H

#include "Winch.h"
#include "Joystick.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <math.h>
#include <Windows.h>

class WireCam {

    public:

        WireCam(Winch * leftWinch, Winch * rightWinch, unsigned char * thisToArduinoSendBuffer) {

            m_leftWinch = leftWinch;
            m_rightWinch = rightWinch;
            m_thisToArduinoSendBuffer = thisToArduinoSendBuffer;
        }

        void Stop() {

            m_leftWinch->Stop();
            m_rightWinch->Stop();
        }

        void Zero() {

            m_leftWinch->Zero();
            m_rightWinch->Zero();
            m_thisToArduinoSendBuffer[2] = 1;
        }

        void UpdateBuffer() {

            m_thisToArduinoSendBuffer[0] = StandardToUCharRange(m_leftWinch->GetSpeed());
            m_thisToArduinoSendBuffer[1] = StandardToUCharRange(m_rightWinch->GetSpeed());
        }

        double GetArcCosTopLeftAngle() {

            double b = m_leftWinch->GetPosition();
            return 90 - Degrees(acos((sq(m_rightWinch->GetPosition()) -
                                      sq(b) -
                                      sq(WIDTH_FULL_RES)) / (-2 * WIDTH_FULL_RES * b)));
        }

        //returns x position
        double GetX() {

            return m_leftWinch->GetPosition() * sin(Radians(GetArcCosTopLeftAngle()));
        }

        //returns y position
        double GetY() {

            return m_leftWinch->GetPosition() * cos(Radians(GetArcCosTopLeftAngle()));
        }

        void Jog(LogitechExtreme3DPro * pilot) {

            if (pilot->eleven) {

                m_rightWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 0) {

                    if (pilot->y < 0) m_leftWinch->Jog(true);
                    else m_leftWinch->Jog(false);
                }
                else m_leftWinch->Stop();
            }
            else {

                m_leftWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 0) {

                    if (pilot->y < 0) m_rightWinch->Jog(true);
                    else m_rightWinch->Jog(false);
                }
                else m_rightWinch->Stop();
            }
        }

        void FlyByWire(LogitechExtreme3DPro* pilot) {

            double angle = StandardDegrees(pilot->x, pilot->y);
            double magnitude = PythagC(pilot->x, pilot->y);
            double targetX = cos(angle * (M_PI / 180.0)) * magnitude * TARGET_MAGNITUDE_SCALAR;
            double targetY = sin(angle * (M_PI / 180.0)) * magnitude * TARGET_MAGNITUDE_SCALAR;
            std::vector<std::vector<double>> system;

            system.push_back(std::vector<double>());
            system[0].push_back(WINCH_1_X - GetX());
            system[0].push_back(WINCH_2_X - GetX());
            system[0].push_back(targetX);
            system.push_back(std::vector<double>());
            system[1].push_back(WINCH_1_Y - GetY());
            system[1].push_back(WINCH_2_Y - GetY());
            system[1].push_back(targetY);

            double lMagnitude = PythagC(system[0][0], system[1][0]);
            double rMagnitude = PythagC(system[0][1], system[1][1]);

            /*std::cout << std::endl << "System" << std::endl;
            PrintVector(system);
            std::cout << std::endl;*/

            std::vector<std::vector<double>> solved = rref(system);

            /*std::cout << "Solved" << std::endl;
            PrintVector(solved);
            std::cout << std::endl;*/

            double lScalar = solved[0][2];
            double rScalar = solved[1][2];
            double lSpeed = lScalar * lMagnitude;
            double rSpeed = rScalar * rMagnitude;

            /*std::cout << "Left Speed: " << lSpeed << std::endl;
            std::cout << "Right Speed: " << rSpeed << std::endl;
            std::cout << std::endl;*/

            m_leftWinch->Set(lSpeed);
            m_rightWinch->Set(rSpeed);
            //Sleep(1000);
        }

        void Fly(LogitechExtreme3DPro * pilot) {

            if (pilot->twelve) Jog(pilot);
            else if (pilot->three) FlyByWire(pilot);
            else if (pilot->seven) Zero();
            else Stop();
            //else AutoFly(pilot, 5);
        }

    private:

        Winch *m_leftWinch;
        Winch *m_rightWinch;
        unsigned char * m_thisToArduinoSendBuffer;
};

#endif