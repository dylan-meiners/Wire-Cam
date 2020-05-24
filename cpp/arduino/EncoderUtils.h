/*void UpdateEncoders() {

    Serial.println(m_position);

    int readBuffer = digitalRead(m_encoderCLKPin);
    //If we go from low to high on CLK
    if (readBuffer != m_oldRead && readBuffer == HIGH) {

        if (digitalRead(m_encoderDTPin) == HIGH) {

            increasePosition();
        }
        else {

            increasePosition(-1);
        }
    }
    m_oldRead = readBuffer;
}*/