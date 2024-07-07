/* SmartFarm MMB BD Pro8 LED On/Off Control
   www.blueinno.co.kr 
   2020. 11. 10 
 */

#define LED 23

void setup() {
  pinMode(LED, OUTPUT);
  Serial1.begin(9600);
}

void loop() { 
  if (Serial1.available()) {
    char c = Serial1.read();
   
    switch(c){
    case'a':
    digitalWrite(LED, HIGH);
    break;
    case'b':
    digitalWrite(LED,LOW);
    break;
    }
   }
 }

 
