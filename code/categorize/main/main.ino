#include "pins.h"
#include "globals.h"
#include "functions.h"
#include "network.h"
#include "lcd.h"
#include "bluetooth.h"

void setup() {
    pinMode(LIGHTPIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    Serial.begin(9600);
    Serial1.begin(9600);
    Serial2.begin(9600);
    dht.begin();
    analogWrite(LIGHTPIN, 255);
    while (!Serial) {
        ; // 시리얼 포트가 연결될 때까지 대기. USB 포트에 필요
    }

    lcdInit();
    printLCD(0, 0, "MMB_SmartFarm");
    printLCD(0, 1, "NETWORKING...");  

    #if USE_NETWORK
    networkInit();
    #endif

    #if DEBUG
    Serial.println("START");
    #endif
}

void loop() {
    mainLoop();
    bluetoothLoop();
    networkLoop();
    delay(100);
}
