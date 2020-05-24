#pragma once

#include "Winch.h"
#include "Joystick.h"
#include "Utils.h"
#include <math.h>

class WireCam {

    public:

        WireCam(Winch *leftWinch, Winch *rightWinch, unsigned char * thisToArduinoSendBuffer) {

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
            return 90 - acos(radians((sq(m_rightWinch->GetPosition() * clicksToInchesMultiplier) -
                                      sq(b) -
                                      sq(width)) / (-2 * width * b)));
        }

        //returns x position in inches
        double GetX() {

            return m_leftWinch->GetPosition() * clicksToInchesMultiplier * sin(radians(GetArcCosTopLeftAngle()));
        }

        //returns y position in inches
        double GetY() {

            return m_leftWinch->GetPosition() * clicksToInchesMultiplier * cos(radians(GetArcCosTopLeftAngle()));
        }

        void Jog(struct Joystick * pilot) {

            if (pilot->eleven) {

                m_thisToArduinoSendBuffer[1] = m_rightWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 127) {

                    if (pilot->y < 127) { m_thisToArduinoSendBuffer[0] = m_leftWinch->Jog(true); }
                    else { m_thisToArduinoSendBuffer[1] = m_leftWinch->Jog(false); }
                }
                else m_thisToArduinoSendBuffer[0] = m_leftWinch->Stop();
            }
            else {

                m_thisToArduinoSendBuffer[0] = m_leftWinch->Stop();
                //If we WANNA GET GOIN
                if (pilot->y != 127) {

                    if (pilot->y < 127) { m_thisToArduinoSendBuffer[1] = m_rightWinch->Jog(true); }

                    else { m_thisToArduinoSendBuffer[1] = m_rightWinch->Jog(false); }
                }
                else m_thisToArduinoSendBuffer[1] = m_rightWinch->Stop();
            }
        }

        void JogPulse(struct Joystick * pilot, const int amountInInches) {

            //If both winches do not have a target position set, then get a new
            //position to give to them
            if (!m_leftWinch->TargetPositionSet() && !m_rightWinch->TargetPositionSet()) {

                if (pilot->five) { //left

                    m_leftWinch->SetTargetPosition(RoundLit(PythagC(GetX() - amountInInches * inchesToClicksMultiplier, GetY())));
                    m_rightWinch->SetTargetPosition(RoundLit(PythagC(width - (GetX() - amountInInches * inchesToClicksMultiplier), GetY())));
                }
                else if (pilot->six) { //right

                    m_leftWinch->SetTargetPosition(RoundLit(PythagC(GetX() + amountInInches * inchesToClicksMultiplier, GetY())));
                    m_rightWinch->SetTargetPosition(RoundLit(PythagC(width - (GetX() + amountInInches * inchesToClicksMultiplier), GetY())));
                }
            }

            m_leftWinch->GotoTargetPosition();
            m_rightWinch->GotoTargetPosition();
        }

        void FlyByWire(struct Joystick * pilot) {

            //TODO ACTUALLY WRITE THIS FUNCTION
        }

    private:

        Winch *m_leftWinch;
        Winch *m_rightWinch;
        unsigned char * m_thisToArduinoSendBuffer;
};