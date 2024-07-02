#ifndef GLOBALS_H
#define GLOBALS_H

// 설정
#define USE_NETWORK 1
#define USE_BLUETOOTH 1
#define DEBUG 1

// 전역 변수
extern float temperature, humidity;
extern int angle;
extern int get_co2_ppm; 
extern int cdcValue;
extern int waterValue;
extern int lightOutput;
extern int fanOutput;
extern int timeout; 
extern unsigned local_time;  // 로컬 시간

extern char sData[64];
extern char rData[32];
extern char nData[32];
extern int rPos;
extern int nPos;
extern int right;
extern int displayToggle;

// 네트워크 설정
extern char ssid[];   // 네트워크 SSID (이름)
extern char pass[];  // 네트워크 비밀번호
extern int status;  // WiFi 상태
extern WiFiEspServer server_f;  // 서버 포트 설정

// 객체 초기화
extern DHT dht;
extern Servo servo; 
extern LiquidCrystal_I2C lcd;

#endif
