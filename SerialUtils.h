void clearSerial() {

    while (Serial.available() > 0) { Serial.read(); }
}

void FlashAllBytes() {

    for (int i = 0; i < Serial.available(); i++) {

        byte b = Serial.read();

        for (int j = 0; j < 8; j++) {

            digitalWrite(LED_BUILTIN, HIGH);
            if ((b >> (7 - j)) & 0x01) {

                delay(500);
            }
            else {

                delay(100);
            }
            digitalWrite(LED_BUILTIN, LOW);
            delay(1000);
        }

        delay(2000);
    }
}

void DemonstrateAllBytes(Servo *servo) {

    for (int i = 0; i < Serial.available(); i++) {

        byte b = Serial.read();

        for (int j = 0; j < 8; j++) {

            servo->write(135);
            if ((b >> (7 - j)) & 0x01) {

                delay(500);
            }
            else {

                delay(100);
            }
            servo->write(180);
            delay(1000);
        }

        delay(2000);
    }
}