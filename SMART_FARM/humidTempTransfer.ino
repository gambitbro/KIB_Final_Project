#include "DHT.h" // DHT 라이브러리 설치

#define DHTPIN 2    // DHT 센서 데이터 핀 연결
#define DHTTYPE DHT11 // DHT 11사용 (DHT22를 사용 시 DHT22로 변경)

// DHT dht(2, DHT11);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(9600);     //9600으로 설정
    Serial1.begin(9600);    // 블루투스 모듈 연결 설정

    dht.begin();
}

void loop() {
    float humid = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(humid) || isnan(temp)) {
        Serial1.println("Failed to read from DHT sensor!");
        return;
    }
    
    
    Serial1.print("Humidity: ");
    Serial1.print(humid);
    Serial1.print(" %\t");
    Serial1.print("Temperature: ");
    Serial1.print(temp);
    Serial1.println(" *C ");

    delay(2000);    // 2초대기
}


