#include "Arduino.h"
#include "T:\\Documents\\Coding\\Coding\\Wire-Cam\\arduino\\Constants.h"

class Encoder {

    public:

        Encoder(const int encoderCLKPinToSet, const int encoderDTPinToSet) {

            m_CLKPin = encoderCLKPinToSet;
            m_DTPin = encoderDTPinToSet;
            m_oldRead = LOW;
            m_position = 0;
            #ifdef VIRTUAL_WINCHES
            m_virtual_speedToSet = 127;
            m_precisePosition = 0;
            m_lastTime = micros();
            #endif
        }

        #ifndef VIRTUAL_WINCHES
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
        #else
        int m_virtual_speedToSet;

        void VirtualUpdate() {

            if (m_virtual_speedToSet != 127) digitalWrite(LED_BUILTIN, HIGH);

            unsigned long currentTime = micros();
            unsigned long deltaTime = currentTime - m_lastTime;
            //                   first, get clicks from microseconds
            m_precisePosition += m_virtual_speedToSet / 500.0 * CLICKS_PER_500_MICROSECONDS_PER_SECOND * deltaTime / 1000000.0;
            m_position = RoundLit(m_precisePosition);
            m_lastTime = currentTime;
        }
        #endif

        long GetPosition() { return m_position; }

    private:

        long m_position;
        unsigned int m_CLKPin;
        unsigned int m_DTPin;
        unsigned int m_oldRead;
        #ifdef VIRTUAL_WINCHES
        unsigned long m_lastTime;
        double m_precisePosition;
        #endif
};