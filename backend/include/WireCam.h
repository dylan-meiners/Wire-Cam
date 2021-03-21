#ifndef WIRECAM_H
#define WIRECAM_H

#include "Winch.h"
#include "Joystick.h"
#include "Utils.h"
#include "Constants.h"
#include <vector>
#include <math.h>

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
        }

        void UpdateBuffer() {

            m_thisToArduinoSendBuffer[0] = StandardToUCharRange(m_leftWinch->GetSpeed());
            m_thisToArduinoSendBuffer[1] = StandardToUCharRange(m_rightWinch->GetSpeed());
        }

        double GetArcCosTopLeftAngle() {

            double b = m_leftWinch->GetPosition() * clicksToInchesMultiplier;
            return 90 - Degrees(acos((sq(m_rightWinch->GetPosition() * clicksToInchesMultiplier) -
                                      sq(b) -
                                      sq(width)) / (-2 * width * b)));
        }

        //returns x position in inches
        double GetX() {

            return m_leftWinch->GetPosition() * clicksToInchesMultiplier * sin(Radians(GetArcCosTopLeftAngle()));
        }

        //returns y position in inches
        double GetY() {

            return m_leftWinch->GetPosition() * clicksToInchesMultiplier * cos(Radians(GetArcCosTopLeftAngle()));
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
            std::vector<std::vector<double>> solved = rref(system);
            //PrintVector(solved);
            //std::cout << std::endl;
            double x = solved[0][2];
            double y = solved[1][2];
            m_leftWinch->Set(x);
            m_rightWinch->Set(y);
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