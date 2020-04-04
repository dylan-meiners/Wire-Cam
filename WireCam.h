#ifndef WireCam_h
#define WireCam_h

#include "Winch.h"

class WireCam {

    public:

        WireCam(Winch *leftWinch, Winch *rightWinch) {

            m_leftWinch = leftWinch;
            m_rightWinch = rightWinch;
        }

        void Stop() {

            m_leftWinch->Stop();
            m_rightWinch->Stop();
        }

        void UpdateEncoders() {

            m_leftWinch->UpdateEncoder();
            m_rightWinch->UpdateEncoder();
        }

        void Zero() {

            m_leftWinch->Zero();
            m_rightWinch->Zero();
        }

    private:

        Winch *m_leftWinch;
        Winch *m_rightWinch;
};

#endif