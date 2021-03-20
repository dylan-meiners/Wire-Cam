#pragma once
#define _USE_MATH_DEFINES

#include "Winch.h"
#include "Joystick.h"
#include "Utils.h"
#include <math.h>

class WireCam {

    public:

        WireCam(Winch * leftWinch, Winch * rightWinch, unsigned char * thisToArduinoSendBuffer) {

            m_leftWinch = leftWinch;
            m_rightWinch = rightWinch;
            m_thisToArduinoSendBuffer = thisToArduinoSendBuffer;
        }

        void Stop() {

            m_thisToArduinoSendBuffer[0] = m_leftWinch->Stop();
            m_thisToArduinoSendBuffer[1] = m_rightWinch->Stop();
        }

        void Zero() {

            m_leftWinch->Zero();
            m_rightWinch->Zero();
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

                m_thisToArduinoSendBuffer[1] = m_rightWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 0) {

                    if (pilot->y < 0) { m_thisToArduinoSendBuffer[0] = m_leftWinch->Jog(true); }
                    else { m_thisToArduinoSendBuffer[0] = m_leftWinch->Jog(false); }
                }
                else m_thisToArduinoSendBuffer[0] = m_leftWinch->Stop();
            }
            else {

                m_thisToArduinoSendBuffer[0] = m_leftWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 0) {

                    if (pilot->y < 0) { m_thisToArduinoSendBuffer[1] = m_rightWinch->Jog(true); }

                    else { m_thisToArduinoSendBuffer[1] = m_rightWinch->Jog(false); }
                }
                else m_thisToArduinoSendBuffer[1] = m_rightWinch->Stop();
            }
        }

        void AutoFly(LogitechExtreme3DPro * pilot, const int amountInInches = 0) {
            
            //If both winches do not have a target position set, then get a new
            //position to give to them
            if (m_leftWinch->ShouldSetANewTarget() && m_rightWinch->ShouldSetANewTarget()) {

                if (pilot->five) { //left

                    m_leftWinch->SetTargetPosition(RoundLit(PythagC(GetX() - amountInInches, GetY()) * inchesToClicksMultiplier));
                    m_rightWinch->SetTargetPosition(RoundLit(PythagC(width - (GetX() - amountInInches), GetY()) * inchesToClicksMultiplier));
                }
                else if (pilot->six) { //right

                    m_leftWinch->SetTargetPosition(RoundLit(PythagC(GetX() + amountInInches, GetY()) * inchesToClicksMultiplier));
                    m_rightWinch->SetTargetPosition(RoundLit(PythagC(width - (GetX() + amountInInches), GetY()) * inchesToClicksMultiplier));
                }
            }

            m_thisToArduinoSendBuffer[0] = m_leftWinch->GotoTargetPosition();
            m_thisToArduinoSendBuffer[1] = m_rightWinch->GotoTargetPosition();
        }

        void FlyByWire(LogitechExtreme3DPro * pilot) {

            //TODO ACTUALLY WRITE THIS FUNCTION
        }

        void Fly(LogitechExtreme3DPro * pilot) {

            if (pilot->twelve) Jog(pilot);
            else {

                Stop();
            }
            /*else if (pilot->three) FlyByWire(pilot);
            else if (pilot->thumb) {

                m_thisToArduinoSendBuffer[0] = m_leftWinch->GotoPosition(400);
                m_thisToArduinoSendBuffer[1] = m_rightWinch->GotoPosition(400);
            }
            else if (pilot->seven) Zero();
            else AutoFly(pilot, 5);*/
        }

    private:

        Winch *m_leftWinch;
        Winch *m_rightWinch;
        unsigned char * m_thisToArduinoSendBuffer;
};