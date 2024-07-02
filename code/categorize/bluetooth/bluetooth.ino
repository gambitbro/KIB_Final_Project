#include "bluetooth.h"
#include "globals.h"
#include "Servo.h"
#include "pins.h"

void bluetoothLoop() {
    // Bluetooth로 받은 데이터 처리
    while (0 < Serial1.available()) {
        char ch = Serial1.read();
        rData[rPos] = ch;
        rPos += 1;
        Serial.print(ch);

        if (ch == '\n') {
            #if DEBUG
            Serial.print("rPos=");
            Serial.print(rPos);
            Serial.print(" ");
            Serial.println(rData);
            #endif
            
            if (memcmp(rData, "C_S-", 4) == 0) {
                if (rData[4] == '0') angle = 10;
                else angle = 80;
                servo.attach(SERVOPIN);
                servo.write(angle);
                delay(500);
                servo.detach();
                #if DEBUG
                Serial.print("서보 모터=");
                Serial.println(angle);
                #endif
            }
            
            if (memcmp(rData, "C_F-", 4) == 0) {
                if (rData[4] == '0') digitalWrite(FAN_PIN, 0);
                else digitalWrite(FAN_PIN, 1);
                #if DEBUG
                Serial.print("팬=");
                Serial.println(rData[4]);
                #endif
            }

            if (memcmp(rData, "C_L-", 4) == 0) {
                int light = atoi(rData + 4);
                analogWrite(LIGHTPIN, (int)(25 * light));
                #if DEBUG
                Serial.print("조명=");
                Serial.println(25 * light);
                #endif
            }
            
            rPos = 0;
            memset(rData, 0x00, 32);
            break;
        }
        delay(10);
    }
}
