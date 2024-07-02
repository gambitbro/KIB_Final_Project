#include "network.h"
#include "globals.h"

void networkInit() {
    // ESP 모듈을 위한 시리얼 초기화
    Serial2.begin(9600);
    // ESP 모듈 초기화
    WiFi.init(&Serial2);

    // 쉴드 유무 확인
    if (WiFi.status() == WL_NO_SHIELD) {
        #if DEBUG
        Serial.println("WiFi 쉴드가 없습니다");
        #endif
        // 계속 진행하지 않음
        while (true);
    }

    // WiFi 네트워크에 연결 시도
    while (status != WL_CONNECTED) {
        #if DEBUG
        Serial.print("WPA SSID에 연결 시도: ");
        Serial.println(ssid);
        #endif
        // WPA/WPA2 네트워크에 연결
        status = WiFi.begin(ssid, pass);
    }
    #if DEBUG
    Serial.println("네트워크에 연결되었습니다");
    #endif
    printWifiStatus(); // LCD에 IP 주소 표시
    delay(2000);
    
    server_f.begin();
}

void printWifiStatus() {
    #if DEBUG
    // 연결된 네트워크의 SSID 출력
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // WiFi 쉴드의 IP 주소 출력
    IPAddress ip = WiFi.localIP();
    delay(10);
    Serial.print("IP Address: ");
    Serial.println(ip);

    // 수신 신호 강도 출력
    long rssi = WiFi.RSSI();
    Serial.print("Signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    #endif

    char ipno2[26];
    sprintf(ipno2, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    printLCD(0, 1, ipno2);
}

void networkLoop() {
    #if USE_NETWORK
    WiFiEspClient c = server_f.available();
    if (c) {
        #if DEBUG
        Serial.println("N#RECV: ");
        #endif
        boolean bDataRead = false;

        nPos = 0;
        
        while (0 < c.available()) {
            char ch = c.read();
            #if DEBUG
            Serial.write(ch);
            #endif
            bDataRead = true;
            nData[nPos] = ch;
            nPos += 1;
        }

        Serial.print("nData=");
        Serial.println(nData);
        if (bDataRead == true) {
            #if DEBUG
            Serial.println();
            #endif
            c.print(sData);
            
            if (5 <= nPos) {
                #if DEBUG
                Serial.println(nData);
                #endif
                
                if (memcmp(nData, "C_S-", 4) == 0) {
                    if (nData[4] == '0') angle = 10;
                    else angle = 80;
                    servo.attach(SERVOPIN);
                    servo.write(angle);
                    delay(500);
                    servo.detach();
                    #if DEBUG
                    Serial.print("N#server_f_MOTOR=");
                    Serial.println(angle);
                    #endif
                }
                
                if (memcmp(nData, "C_F-", 4) == 0) {
                    if (nData[4] == '0') digitalWrite(FAN_PIN, 0);
                    else digitalWrite(FAN_PIN, 1);
                    #if DEBUG
                    Serial.print("N#FAN=");
                    Serial.println(nData[4]);
                    #endif
                }
          
                if (memcmp(nData, "C_L-", 4) == 0) {
                    int light = atoi(nData + 4);
                    analogWrite(LIGHTPIN, (int)(25 * light));
                    #if DEBUG
                    Serial.print("N#LIGHT=");
                    Serial.println(light);
                    #endif
                } 
                
                nPos = 0;
                memset(nData, 0x00, 32);
            }
        }
        
        delay(10);
        // close the connection:
        // c.stop();
        // Serial.println("Client disconnected");
    }
    #endif
}
