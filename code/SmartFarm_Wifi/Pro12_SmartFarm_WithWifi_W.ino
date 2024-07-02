#include <Servo.h>
#include "DHT.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "WiFiEsp.h"

// 핀 정의
#define DHTPIN 12
#define DHTTYPE DHT11
#define SERVOPIN 9
#define LIGHTPIN 4
#define FAN_PIN 32

// 설정
#define USE_NETWORK 1
#define USE_BLUETOOTH 1
#define DEBUG 1

// 전역 변수
float temperature, humidity;
int angle = 0;
int get_co2_ppm = 0; 
int cdcValue = 0;
int waterValue = 0;
int lightOutput = 0;
int fanOutput = 0;
int timeout = 0; 
unsigned local_time = 0;  // 로컬 시간

char sData[64] = { 0x00 };
char rData[32] = { 0x00 };
char nData[32] = { 0x00 };
int rPos = 0;
int nPos = 0;
int right = 10;
int displayToggle = 1;

// 네트워크 설정
char ssid[] = "iptime_iot";   // 네트워크 SSID (이름)
char pass[] = "1234567890!";  // 네트워크 비밀번호
int status = WL_IDLE_STATUS;  // WiFi 상태
WiFiEspServer server_f(400);  // 서버 포트 설정

// 객체 초기화
DHT dht(DHTPIN, DHTTYPE);
Servo servo; 
LiquidCrystal_I2C lcd(0x27, 16, 2);

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

    lcd.init();
    lcd.backlight();
    printLCD(0, 0, "MMB_SmartFarm");
    printLCD(0, 1, "NETWORKING...");  

    #if USE_NETWORK
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
    #endif

    #if DEBUG
    Serial.println("START");
    #endif
}

void loop() {
    // 메인 루프에서 반복 실행할 코드
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

delay(100);
}
