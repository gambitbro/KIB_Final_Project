#include <SoftwareSerial.h>
#include <DS3231.h>   // RTC 라이브러리
#include "DHT.h"
#include <Wire.h>
#include "LiquidCrystal_I2C.h"
#include "Servo.h"

#define FAN_PIN 32 //환기팬 핀
#define PHOTO A0  //조도 핀
#define LED 4   //LED 핀
//=========================================================================================================================
//RTC
DS3231 Clock;
bool Century,h12,PM;
int year, month, date, DoW, hour, minute, second,temperature;

//서보모터
Servo myServo;

//millis
unsigned long time1 = 0;

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

//=========================================================================================================================
void setup() {
  //시리얼, 블루투스 설정
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(13, OUTPUT);

  Wire.begin();

  //현재 시간 세팅
  // Clock.setSecond(00); //초 세팅 
  // Clock.setMinute(58);  //분 세팅 
  // Clock.setHour(14);  //Set the hour c
  // Clock.setDoW(5);    //Set the day of the week
  // Clock.setDate(9);  //Set the date of the month
  // Clock.setMonth(11); //Set the month of the year
  // Clock.setYear(23);  //Set the year 
	
  delay(10);

  //온습도 설정
  dht.begin();

  //조도 설정

  //토양수분 핀모드 설정
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);

  //LCD설정
  lcd.clear();
  lcd.init();
  lcd.backlight();

  //LED설정
  pinMode(LED, OUTPUT);

  //서보모터 설정
  myServo.attach(9);
  myServo.write(0);

  //환기팬 설정
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, 0);

}

//=========================================================================================================================
void loop() {
  // //LED 서서히 켰다 끄기
  // for(int i=0; i < 256; i++){
  //   analogWrite(LED, i);
  //   delay(10);
  // }
  // for(int i=0; i < 256; i++){
  //   analogWrite(LED, 255-i);
  //   delay(10);
  // }

  //시간
  //ReadDS3231();

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


  //블루투스 테스트(온도, 습도, 조도, 토양수분 보내기)
  unsigned long time2 = millis();
  delay(100);
  if ( (time2-time1)>999) {
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
  
  time1=time2;
  }

  // if(humidity >= 46 || temperature >= 23){
  //   myServo.write(90);
  //   digitalWrite(FAN_PIN, 1);
  //   delay(1000);
  // } else {
  //   myServo.write(0);
  //   digitalWrite(FAN_PIN, 0);
  //   delay(1000);
  // }

  //액츄에이터 조절(블루투스)
  if (Serial1.available()){
    int led_con = Serial1.read();
    Serial.println(led_con);

    if (led_con == 'a'){
      int brightness = Serial1.parseInt();
      analogWrite(LED, brightness);
      // delay(3000);
    } else if (led_con == 'f'){
      //Serial.print(led_con);
      digitalWrite(LED, HIGH);
    } else if (led_con == 'g'){
      //Serial.print(led_con);
      digitalWrite(LED, LOW);
    } else if (led_con == 'b'){
      digitalWrite(FAN_PIN, 1);
    } else if (led_con == 'c'){
      digitalWrite(FAN_PIN, 0);
    } else if (led_con == 'd'){
      myServo.write(90);
    } else if (led_con == 'e'){
      myServo.write(0);
    }
  }

  

}

void ReadDS3231()   //시간 불러오기
{
  second=Clock.getSecond();
  minute=Clock.getMinute();
  hour=Clock.getHour(h12,PM);
  DoW=Clock.getDoW();
  date=Clock.getDate();
  month=Clock.getMonth(Century);
  year=Clock.getYear();
  
  temperature=Clock.getTemperature();
 
  Serial.print("20");
  //Serial.print(year,DEC);
  Serial.print(year);
  Serial.print('-');
  Serial.print(month,DEC);
  Serial.print('-');
  Serial.print(date,DEC);
  Serial.print('(');
  Serial.print(DoW,DEC);
  Serial.print(") ");
  Serial.print(hour,DEC);
  Serial.print(':');
  Serial.print(minute,DEC);
  Serial.print(':');
  Serial.print(second,DEC);
  Serial.print('\n');
  Serial.print("Temperature=");
  Serial.print(temperature); 
  Serial.print('\n');
}


