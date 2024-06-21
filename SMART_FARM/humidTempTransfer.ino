#include "DHT.h" // DHT 라이브러리 설치

#define DHTPIN 2    // DHT 센서 데이터 핀 연결
#define DHTTYPE DHT11 // DHT 11사용 (DHT22를 사용 시 DHT22로 변경)

// DHT dht(2, DHT11);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);     //9600으로 설정
    dht.begin();
}

void loop() {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");

    delay(2000);    // 2초대기
}


