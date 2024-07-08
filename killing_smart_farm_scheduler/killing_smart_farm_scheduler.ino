#include <SoftwareSerial.h>
#include <DS3231.h>
#include "DHT.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "Servo.h"

#define FAN 32 //환기팬 핀
#define PHOTO A0  //조도 핀
#define LED 4   //LED 핀
//=========================================================================================================================
//  Scheduling
#define LED_SCH   1
#define WINDOW_SCH  2
#define FAN_SCH     4

#define MAX_DO  3
#define MAX_SCHEDULE 24

//RTC
DS3231 Clock;
bool h12, PM;
int hour;
int prehour = -1;

//서보모터
Servo myServo;
int angle;

//millis
unsigned long time1 = 0;
unsigned long time3 = 0;

//온습도 센서
DHT dht(12, DHT11);

//조도 센서
int Photo_Value = 0;

//토양 수분 센서
float waterAdc;
int waterValue;
int red = 23;
int green = 35; 

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

typedef struct Scheduling {
  int time;
  int doing;
  int parameter[3];
}SCHEDULE;

SCHEDULE Set_Schedule[MAX_SCHEDULE] = {
  {1, 7, {0, 0, 1}},   
  {2, 7, {0, 0, 0}},
  {3, 7, {0, 1, 1}},    
  {4, 7, {0, 0, 0}},
  {5, 7, {0, 0, 1}}, 
  {6, 7, {20, 1, 0}},    
  {7, 7, {20, 1, 1}},
  {8, 7, {20, 0, 0}},    
  {9, 7, {20, 0, 1}},
  {10, 7, {50, 1, 0}},
  {11, 7, {50, 1, 1}},   
  {12, 7, {50, 0, 0}},
  {13, 7, {50, 0, 1}},    
  {14, 7, {50, 1, 0}},
  {15, 7, {50, 1, 1}},
  {16, 7, {50, 0, 0}},    
  {17, 7, {40, 0, 1}},
  {18, 7, {30, 1, 0}},    
  {19, 7, {20, 1, 0}},
  {20, 7, {10, 0, 1}},
  {21, 7, {0, 0, 1}},    
  {22, 7, {0, 1, 0}},
  {23, 7, {0, 1, 1}},    
  {24, 7, {0, 0, 0}} 
};

void aLED(int para){
  analogWrite(LED, para);
}
void aWindow(int para){
 if(para == 0) 
 angle = 0;
 else 
 angle = 90;
 myServo.write(angle);
 //delay(200);  
}
void aFan(int para){
  if(para == 0) 
  digitalWrite(FAN, 0);
  else 
  digitalWrite(FAN, 1); 
}
int SearchingDoing(){
  int i;
  for(i=0; i<MAX_SCHEDULE; i++){
    if (hour == Set_Schedule[i].time){
      return i;
    }
  }
    return -1;
}
void action(int doing, int para[]){
  int i=0;
  int iIndex;
  for(i=0;i<MAX_DO;i++){ 
    iIndex = doing & (1<<i); 
    
    switch(iIndex){
      case LED_SCH:
        aLED(para[i]);
        Serial.print(para[i]);
        Serial.print("\n");
        break;
   
      case WINDOW_SCH:
        aWindow(para[i]);
        break;
  
      case FAN_SCH:
        aFan(para[i]);
        break;
        
      default:
        break;
    }
  }
}

void ReadDS3231()   
{
  hour = Clock.getHour(h12, PM);
}

//=========================================================================================================================
void setup() {
  //시리얼, 블루투스 설정
  Serial.begin(9600);
  Serial1.begin(9600);

  Wire.begin();

  //현재 시간 세팅
  Clock.setSecond(00);
  Clock.setMinute(58);
  Clock.setHour(14);
  Clock.setDoW(5);
  Clock.setDate(9);
  Clock.setMonth(11);
  Clock.setYear(23);
  delay(10);

  //온습도 설정
  dht.begin();

  //토양수분 핀모드 설정
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  //LCD설정
  lcd.clear();
  lcd.init();
  lcd.backlight();

  //LED설정
  pinMode(LED, OUTPUT);
  //digitalWrite(LED, 50);

  //서보모터 설정
  myServo.attach(9);
  myServo.write(0);

  //환기팬 설정
  pinMode(FAN, OUTPUT);
  //digitalWrite(FAN, 0);

}

//=========================================================================================================================
void loop() {
  //LED ON
  //digitalWrite(LED, HIGH);

  //온습도
  int humidity = dht.readHumidity();
  int temperature = dht.readTemperature();

  //조도
  Photo_Value = analogRead(PHOTO);
  float pv_value = float(Photo_Value*5)/1023;
  float Rp = (10*pv_value)/(5-pv_value);
  float y = (log10(200/Rp)/0.7);
  int Ea = pow(10,y);

  //토양수분
  int soil = analogRead(A1);
  if (soil > 600) {
    digitalWrite(green, LOW);
    digitalWrite(red, HIGH);
    delay(200);  
    digitalWrite(red, LOW);
  } else {
    digitalWrite(green, HIGH);
  }

  //LCD 표시 부분
  //온도, 습도, 조도, 토양수분
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("humid=");
  lcd.print((int)humidity);
  lcd.print(" temp=");
  lcd.print((int)temperature);
  lcd.setCursor(0,1);
  lcd.print("Lx=");
  lcd.print((int)Ea);
  lcd.print(" soil=");
  lcd.print((int)soil);


  //블루투스(온도, 습도, 조도, 토양수분 보내기)
  unsigned long time2 = millis();
  delay(100);
  if ( (time2-time1) > 999) {
  Serial1.print( (int)temperature );
  Serial1.print(",");
  Serial1.print( (int)humidity );
  Serial1.print(",");
  Serial1.print( (int)Ea );
  Serial1.print(",");
  Serial1.print( (int)soil );
  //Serial1.print("\n");
  
  Serial.print((int)temperature);
  Serial.print(",");
  Serial.println((int)humidity);
  Serial.print(",");
  Serial.print((int)Ea);
  Serial.print(",");
  Serial.print((int)soil);
  Serial.print("\n");
  
  time1 = time2;

  // //LED 서서히 켰다 끄기
  // for(int i=0; i < 256; i++){
  //   analogWrite(LED, i);
  //   delay(10);
  // }
  // for(int i=0; i < 256; i++){
  //   analogWrite(LED, 255-i);
  //   delay(10);
  // }
  }


  //액츄에이터 조절(블루투스)
  if (Serial1.available()){
    int con = Serial1.read();
    Serial.println(con);

    if (con == 'a'){
      int brightness = Serial1.parseInt();
      analogWrite(LED, brightness);
      // delay(3000);
    } else if (con == 'f'){
      //Serial.print(con);
      digitalWrite(LED, HIGH);
    } else if (con == 'g'){
      //Serial.print(con);
      digitalWrite(LED, LOW);
    } else if (con == 'b'){
      digitalWrite(FAN, 1);
    } else if (con == 'c'){
      digitalWrite(FAN, 0);
    } else if (con == 'd'){
      myServo.write(90);
    } else if (con == 'e'){
      myServo.write(0);
    }
  }
  // } else {
  //   digitalWrite(LED, 50);
  //   if(humidity >= 50 || temperature >= 25){
  //     myServo.write(90);
  //     digitalWrite(FAN, 1);
  //     delay(500);
  //   } else {
  //     myServo.write(0);
  //     digitalWrite(FAN, 0);
  //     delay(500);
  //   }
  // }

  ReadDS3231();
  Serial.print(hour);

  // 자동 스케줄링
  unsigned long time4 = millis();
  delay(100);
  if ( (time4-time3) > 2999) {
    if (prehour != hour){
      int tmp = SearchingDoing();
      prehour = hour;
      
      if(tmp != -1){
        action(Set_Schedule[tmp].doing, Set_Schedule[tmp].parameter);
      }
    }

    time3 = time4;
  }
}