#include <Servo.h>
#include "DHT.h"
#include "LiquidCrystal_I2C.h"
#include "WiFiEsp.h"

#define LED 23
#define MOTOR 32
#define VR A4

// DHT 센서 설정
#define DHTPIN 12
#define DHTTYPE DHT11
#define SERVOPIN 9
#define LIGHTPIN 4
#define FAN_PIN 32
#define WATER_PUMP_PIN 31

float temperature, humidity;
int angle = 0;
int get_co2_ppm = 0; 
int cdcValue = 0;
int waterValue = 0;
int lightOutput = 0;
int fanOutput = 0;
int waterPumpPin = 0;
int timeout = 0; 
bool water_State = false;
unsigned water_Time = 0;
unsigned local_time = 0;

char sData[64] = { 0x00, };
char rData[32] = { 0x00, };
char nData[32] = { 0x00, };
int rPos = 0;
int nPos = 0;

// LCD 설정
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi 설정
char ssid[] = "iptime_iot";
char password[] = "1234567890!";
char server[] = "35.232.26.6";  // 서버 IP 주소
int port = 80;                  // 서버 포트 번호

WiFiEspClient client;
WiFiEspServer server_f(400);

DHT dht(DHTPIN, DHTTYPE);
Servo servo;

void printLCD(int col, int row, char *str) {
  for (int i = 0; i < strlen(str); i++) {
    lcd.setCursor(col + i, row);
    lcd.print(str[i]);
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  delay(10);

  Serial.print("IP Address: ");
  Serial.println(ip);

  char ipno2[26];
  sprintf(ipno2, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  printLCD(0, 1, ipno2);

  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void setup() {
  pinMode(MOTOR, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LIGHTPIN, OUTPUT);
  pinMode(FAN_PIN, OUTPUT);
  pinMode(WATER_PUMP_PIN, OUTPUT);

  Serial.begin(115200);  // 시리얼 통신 속도 설정
  Serial1.begin(115200); // ESP8266 통신용 하드웨어 시리얼 시작
  dht.begin();
  analogWrite(LIGHTPIN, 255);

  lcd.init();
  lcd.backlight();
  printLCD(0, 0, "MMB_SmartFarm");
  printLCD(0, 1, "NETWORKING...");
  lcd.clear();

  // AT 명령어를 보내 ESP8266 모듈이 응답하는지 확인합니다.
  Serial1.println("AT");
  delay(1000);
  if (Serial1.available()) {
    String response = Serial1.readString();
    Serial.println(response);
    if (response.indexOf("OK") != -1) {
      Serial.println("ESP8266 initialized successfully");
    } else {
      Serial.println("ESP8266 initialization failed");
      lcd.print("ESP INIT FAIL");
      while (true);
    }
  } else {
    Serial.println("No response from ESP8266");
    lcd.print("NO RESPONSE");
    while (true);
  }

  // WiFi 연결
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    lcd.print("WL_NO_SHIELD");
    Serial.println("WiFi shield not present");
    while (true);
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    delay(10000);

    // WiFi 연결 상태 체크 추가
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi");
    } else {
      Serial.println("Failed to connect to WiFi");
      lcd.print("WIFI FAIL");
    }
  }

  printWifiStatus();
  delay(2000);
  server_f.begin();

  Serial.println("START");
  lcd.clear();
  lcd.print("System Ready");
}

void loop() {
  timeout += 1;
  if (timeout % 10 == 0) {
    cdcValue = analogRead(0) / 10;
    waterValue = analogRead(1) / 10;
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    lcd.clear();
    bool displayToggle = (timeout / 10) % 2;
    if (displayToggle) {
      sprintf(sData, "temp %02dC humi %02d%%", (int)temperature, (int)humidity);
      printLCD(0, 0, sData);
      sprintf(sData, "cdc%-04d soil%-04d", cdcValue, waterValue);
      printLCD(0, 1, sData);
    } else {
      sprintf(sData, "temp %02dC humi %02d%%", (int)temperature, (int)humidity);
      printLCD(0, 0, sData);
      sprintf(sData, "co2 %d ppm", get_co2_ppm);
      printLCD(0, 1, sData);
    }

    sprintf(sData, "{ \"temp\":%02d,\"humidity\":%02d,\"cdc\":%-04d,\"water\":%-04d,\"co2\":%-04d }",
            (int)temperature, (int)humidity, cdcValue, waterValue, get_co2_ppm);
    Serial.println(sData);

    Serial1.println(sData);
  }

  while (0 < Serial1.available()) {
    char ch = Serial1.read();
    rData[rPos++] = ch;
    Serial.print(ch);

    if (ch == '\n') {
      Serial.print("rPos=");
      Serial.print(rPos);
      Serial.print(" ");
      Serial.println(rData);

      if (memcmp(rData, "C_S-", 4) == 0) {
        angle = (rData[4] == '0') ? 10 : 80;
        servo.attach(SERVOPIN);
        servo.write(angle);
        delay(500);
        servo.detach();
        Serial.print("server_f_MOTOR=");
        Serial.println(angle);
      }

      if (memcmp(rData, "C_F-", 4) == 0) {
        digitalWrite(FAN_PIN, (rData[4] == '0') ? 0 : 1);
        Serial.print("FAN=");
        Serial.println(rData[4]);
      }

      if (memcmp(rData, "C_L-", 4) == 0) {
        int light = atoi(rData + 4);
        analogWrite(LIGHTPIN, 25 * light);
        Serial.print("LIGHT=");
        Serial.println(25 * light);
      }

      if (memcmp(rData, "C_W-", 4) == 0) {
        if (rData[4] == '0') {
          digitalWrite(WATER_PUMP_PIN, 0);
        } else {
          digitalWrite(WATER_PUMP_PIN, 1);
          water_State = true;
        }
        Serial.print("WATER=");
        Serial.println(rData[4]);
      }

      rPos = 0;
      memset(rData, 0x00, 32);
      break;
    }
    delay(10);
  }

  WiFiEspClient c = server_f.available();
  if (c) {
    Serial.println("N#RECV: ");
    bool bDataRead = false;
    nPos = 0;

    while (0 < c.available()) {
      char ch = c.read();
      Serial.write(ch);
      bDataRead = true;
      nData[nPos++] = ch;
    }

    Serial.print("nData=");
    Serial.println(nData);
    if (bDataRead) {
      Serial.println();
      c.print(sData);

      if (5 <= nPos) {
        Serial.println(nData);

        if (memcmp(nData, "C_S-", 4) == 0) {
          angle = (nData[4] == '0') ? 10 : 80;
          servo.attach(SERVOPIN);
          servo.write(angle);
          delay(500);
          servo.detach();
          Serial.print("N#server_f_MOTOR=");
          Serial.println(angle);
        }

        if (memcmp(nData, "C_F-", 4) == 0) {
          digitalWrite(FAN_PIN, (nData[4] == '0') ? 0 : 1);
          Serial.print("N#FAN=");
          Serial.println(nData[4]);
        }

        if (memcmp(nData, "C_L-", 4) == 0) {
          int light = atoi(nData + 4);
          analogWrite(LIGHTPIN, 25 * light);
          Serial.print("N#LIGHT=");
          Serial.println(light);
        }

        if (memcmp(nData, "C_W-", 4) == 0) {
          if (nData[4] == '0') {
            digitalWrite(WATER_PUMP_PIN, 0);
          } else {
            digitalWrite(WATER_PUMP_PIN, 1);
            water_State = true;
          }
          Serial.print("N#WATER=");
          Serial.println(nData[4]);
        }

        nPos = 0;
        memset(nData, 0x00, 32);
      }
    }

    delay(10);
  }

  if (water_State) {
    water_Time += 1;
    if (water_Time > 2500) {
      digitalWrite(WATER_PUMP_PIN, 0);
      water_State = false;
      water_Time = 0;
    }
  }

  delay(100);
}
