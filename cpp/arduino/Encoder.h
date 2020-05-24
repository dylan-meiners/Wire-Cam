#include "Arduino.h"

class Encoder {

    public:

        Encoder(const int encoderCLKPinToSet, const int encoderDTPinToSet) {

            m_CLKPin = encoderCLKPinToSet;
            m_DTPin = encoderDTPinToSet;
            m_oldRead = LOW;
            m_position = 0;
        }

        void Update() {

            int readBuffer = digitalRead(m_CLKPin);
            //If we go from low to high on CLK
            if (readBuffer != m_oldRead && readBuffer == HIGH) {

                if (digitalRead(m_DTPin) == HIGH) {

                    m_position++;
                }
                else {

                    m_position--;
                }
            }
            m_oldRead = readBuffer;
        }

        long GetPosition() { return m_position; }

    private:

        long m_position;
        unsigned int m_CLKPin;
        unsigned int m_DTPin;
        unsigned int m_oldRead;
};