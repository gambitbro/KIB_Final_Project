#include "functions.h"
#include "globals.h"
#include "pins.h"

// LCD에 문자열 출력 함수
void printLCD(int col, int row, char *str) {
    for (int i = 0; i < strlen(str); i++) {
        lcd.setCursor(col + i, row);
        lcd.print(str[i]);
    }
}

// WiFi 상태 출력 함수
void printWifiStatus() {
    #if DEBUG
    // 연결된 네트워크의 SSID 출력
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // WiFi 쉴드의 IP 주소 출력
    IPAddress ip =
