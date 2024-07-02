#include "WiFiEsp.h"
#include "DHT.h"
#include "LiquidCrystal_I2C.h"

#define LED 23
#define MOTOR 32
#define VR A4

// DHT 센서 설정
#define DHTPIN 12     // DHT 센서 데이터 핀
#define DHTTYPE DHT11 // DHT11을 사용
DHT dht(DHTPIN, DHTTYPE);

// 조도 센서 설정
#define LIGHT_SENSOR_PIN A0

// 토양 수분 센서 설정
#define SOIL_MOISTURE_PIN A1

// LCD 설정
LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi 설정
char ssid[] = "iptime_iot";
char password[] = "1234567890!";

// 서버 설정
char server[] = "35.232.26.6";  // 서버 IP 주소
int port = 80;                  // 서버 포트 번호

WiFiEspClient client;

void setup() {
  pinMode(MOTOR, OUTPUT);
  pinMode(LED, OUTPUT);

  // LCD 초기화
  lcd.begin(16, 2);
  lcd.print("Initializing...");

  // DHT 센서 초기화
  dht.begin();

  // 시리얼 시작
  Serial.begin(9600);
  Serial1.begin(9600); // ESP8266 통신용 하드웨어 시리얼 시작
  WiFi.init(&Serial1);

  // WiFi 연결
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);
  }

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    delay(10000);
  }
  Serial.println("Connected to WiFi");

  lcd.clear();
  lcd.print("System Ready");
}

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

  // WiFiEspClient를 사용하여 서버에 데이터 전송
  if (client.connect(server, port)) {
    Serial.println("Connected to server");

    // HTTP POST 요청 작성
    String postData = "temperature=" + String(temperature) + "&humidity=" + String(humidity) + "&lightLevel=" + String(lightLevel) + "&soilMoisture=" + String(soilMoisture);

    client.println("POST /insert_data.php HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.println(postData.length());
    client.println();
    client.println(postData);

    // 서버 응답 읽기
    while (client.connected()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
          Serial.println("Headers received");
          break;
        }
      }
    }

    // 서버 응답 본문 출력
    while (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }

    // 연결 종료
    client.stop();
    Serial.println("Disconnected from server");
  } else {
    Serial.println("Connection to server failed");
  }

  // 데이터를 주기적으로 읽기 위해 60초 대기
  delay(60000);
}
