#include "globals.h"

// 전역 변수 정의
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
