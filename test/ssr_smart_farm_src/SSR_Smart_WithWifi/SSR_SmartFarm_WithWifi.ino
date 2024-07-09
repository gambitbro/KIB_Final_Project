#include <Servo.h>
#include "DHT.h"
#include "Wire.h"
#include "LiquidCrystal_I2C.h"
#include "WiFiEsp.h"

#define SERVOPIN 9 // 서보 모터의 핀 번호 정의
#define DHTPIN 12 // DHT 센서의 핀 번호 정의
#define DHTTYPE DHT11 // DHT 센서의 타입 정의 (DHT11)
#define LIGHTPIN 4 // 조도 센서의 핀 번호 정의
#define FAN_PIN 32 // 팬의 핀 번호 정의
#define LIGHT_SENSOR_PIN A0 // 조도 센서의 아날로그 핀
#define SOIL_MOISTURE_PIN A1 // 토양 수분 센서의 아날로그 핀
#define USE_NETWORK 1 // 네트워크 사용 여부 정의
#define USE_BLUETOOTH 1 // 블루투스 사용 여부 정의
#define DEBUG 1 // 디버그 모드 사용 여부 정의

// WiFi 및 서버 설정
char ssid[] = "iptime_iot"; // WiFi SSID
char pass[] = "1234567890!"; // WiFi 비밀번호
int status = WL_IDLE_STATUS; // WiFi 상태 변수
WiFiEspClient client; // WiFi 클라이언트 객체
WiFiEspServer server_f(400); // 서버 포트 설정
char server[] = "35.232.26.6"; // 서버 주소
char path[] = "/var/www/html/insert_data.php"; // PHP 파일 경로

// 전역 변수 선언
float temperature, humidity; // 온도와 습도 값 저장 변수
int angle = 0; // 서보 모터 각도 변수
int lightOutput = 0; // 조도 센서 출력 값 저장 변수
int fanOutput = 0; // 팬 출력 값 저장 변수
int timeout = 0; // 타임아웃 값 저장 변수
unsigned local_time = 0; // 로컬 시간 변수

// 센서 및 장치 초기화
DHT dht(DHTPIN, DHTTYPE); // DHT 센서 초기화
Servo servo; // 서보 모터 객체 생성
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C LCD 객체 생성

// LCD에 문자열 출력 함수
void printLCD(int col, int row , char *str) {
  for(int i=0 ; i < strlen(str) ; i++) {
    lcd.setCursor(col + i, row);
    lcd.print(str[i]);
  }
}

// WiFi 상태 출력 함수
void printWifiStatus() {
#if DEBUG
  Serial.print("SSID: "); // 연결된 네트워크 SSID 출력
  Serial.println(WiFi.SSID());
#endif

  IPAddress ip = WiFi.localIP(); // 로컬 IP 주소 가져오기
  delay(10);
#if DEBUG
  Serial.print("IP Address: "); // IP 주소 출력
  Serial.println(ip);
#endif
  char ipno2[26];
  sprintf(ipno2, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
  printLCD(0, 1, ipno2); // LCD에 IP 주소 출력

  long rssi = WiFi.RSSI(); // 신호 세기 가져오기
#if DEBUG
  Serial.print("Signal strength (RSSI):"); // 신호 세기 출력
  Serial.print(rssi);
  Serial.println(" dBm");
#endif
}

// 설정 함수
void setup() {
  pinMode(LIGHTPIN, OUTPUT); // 조도 센서 핀을 출력 모드로 설정
  pinMode(FAN_PIN, OUTPUT); // 팬 핀을 출력 모드로 설정

  Serial.begin(9600); // 시리얼 통신 시작
  Serial1.begin(9600); // 시리얼 통신 1 시작
  Serial2.begin(9600); // 시리얼 통신 2 시작

  dht.begin(); // DHT 센서 시작
  analogWrite(LIGHTPIN, 255); // 조도 센서 초기 값 설정

  while (!Serial) {
    ; // 시리얼 포트 연결 대기
  }

  lcd.init(); // LCD 초기화
  lcd.backlight(); // LCD 백라이트 켜기
  printLCD(0, 0, "SSR_SmartFarm"); // LCD에 문자열 출력
  printLCD(0, 1, "HELLO!");  

#if USE_NETWORK
  Serial2.begin(9600); // ESP 모듈과 시리얼 통신 시작
  WiFi.init(&Serial2); // ESP 모듈 초기화

  if (WiFi.status() == WL_NO_SHIELD) {
#if DEBUG
    Serial.println("WiFi shield not present"); // WiFi 모듈 없음 출력
#endif
    while (true); // 무한 대기
  }

  // 네트워크 연결 안될 시 디버그
  while (status != WL_CONNECTED) {
#if DEBUG
    Serial.print("Attempting to connect to WPA SSID: "); // 네트워크 연결 시도 출력
    Serial.println(ssid);
#endif
    status = WiFi.begin(ssid, pass); // 네트워크 연결 시도
  }

#if DEBUG
  Serial.println("You're connected to the network");
#endif
  printWifiStatus(); // LCD에 IP 주소 출력
  delay(2000);
  
  server_f.begin(); // 서버 시작
#endif

#if DEBUG
  Serial.println("START"); // 시작 메시지 출력
#endif
}

// 메인 루프 함수
void loop() {
  // DHT 센서에서 데이터 읽기
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // 조도 센서에서 데이터 읽기
  int lightLevel = analogRead(LIGHT_SENSOR_PIN);

  // 토양 수분 센서에서 데이터 읽기
  int soilMoisture = analogRead(SOIL_MOISTURE_PIN);

  // LCD에 온습도 데이터 표시
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperature);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humid: ");
  lcd.print(humidity);
  lcd.print("%");

  // WiFi 상태 체크
  if (WiFi.status() != WL_CONNECTED) {
#if DEBUG
    Serial.println("WiFi not connected, attempting to reconnect...");
#endif
    while (status != WL_CONNECTED) {
      status = WiFi.begin(ssid, pass); // 네트워크 재연결 시도
      delay(5000); // 재연결 시도 후 5초 대기
    }
  }

  // WiFi를 통해 서버로 데이터 전송
  if (client.connect(server, 80)) {
    String postData = "temperature=" + String(temperature) +
                      "&humidity=" + String(humidity) +
                      "&light=" + String(lightLevel) +
                      "&soilMoisture=" + String(soilMoisture);
    
    client.println("POST " + String(path) + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Content-Length: " + postData.length());
    client.println();
    client.println(postData);

#if DEBUG
    Serial.println("Data sent to server:");
    Serial.println(postData);
#endif

    // HTTP 응답 코드 확인
    int timeout = 5000; // 5초 대기
    long startMillis = millis();
    while (!client.available() && (millis() - startMillis < timeout)) {
      delay(100);
    }

    if (client.available()) {
      String response = client.readString();
#if DEBUG
      Serial.println("Server response:");
      Serial.println(response);
#endif
    } else {
#if DEBUG
      Serial.println("No response from server");
#endif
    }

    client.stop();
  } else {
#if DEBUG
    Serial.println("Connection to server failed");
#endif
  }

  delay(5000); // 5초 대기 후 다시 측정
}
