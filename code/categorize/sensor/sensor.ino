#include "sensors.h"
#include "globals.h"
#include "lcd.h"

void sensorLoop() {
    // 주기적으로 센서 데이터를 읽어 처리
    timeout += 1;
    if (timeout % 10 == 0) {
        cdcValue = analogRead(0);
        cdcValue /= 10;

        waterValue = analogRead(1);
        waterValue /= 10;

        humidity = dht.readHumidity();
        temperature = dht.readTemperature();

        lcd.clear();
        displayToggle = !displayToggle;
        if (displayToggle == 1) {
            memset(sData, 0x00, 64);
            sprintf(sData, "temp %02dC humi %02d%%", (int)temperature, (int)humidity);
            printLCD(0, 0, sData);
            memset(sData, 0x00, 64);
            sprintf(sData, "cdc%-04d soil%-04d", cdcValue, waterValue);
            printLCD(0, 1, sData);
        } else {
            memset(sData, 0x00, 64);
            sprintf(sData, "temp %02dC humi %02d%%", (int)temperature, (int)humidity);
            printLCD(0, 0, sData);
            memset(sData, 0x00, 64);
            sprintf(sData, "co2 %d ppm", get_co2_ppm);
            printLCD(0, 1, sData);
        }

        sprintf(sData, "{ \"temp\":%02d,\"humidity\":%02d,\"cdc\":%-04d,\"water\":%-04d,\"co2\":%-04d }", 
            (int)temperature, (int)humidity, cdcValue, waterValue, get_co2_ppm);
        Serial.println(sData);
        Serial1.println(sData);
    }
}
