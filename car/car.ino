#include <IRremote.h>

int D1 = 8;
int M1 = 9;
int D2 = 4;
int M2 = 5;
int INFRARED = 7;
int FRONTSENSOR = 13;

IRrecv irrecv(INFRARED);
decode_results results;



void setup() {
  // put your setup code here, to run once:
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(INFRARED, INPUT);
  pinMode(FRONTSENSOR, INPUT);
  Serial.begin(9600);
  Serial.println("start");

  irrecv.enableIRIn(); // Start the receiver
}

void test() 
{
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  analogWrite(M1, 160);
  analogWrite(M2, 160);

  delay(500);
  analogWrite(M1, 0);
  analogWrite(M2, 0);
  delay(3000);
}

void right() 
{
  digitalWrite(D2, LOW);
  analogWrite(M2, 150);
  delay(300);
  analogWrite(M2, 0);
}

int speed = 0;
int turn = 0;

void getIRCommands() {
  if (irrecv.decode(&results))
  {
    //FF6897 - 1
    //FF9867 - 2
    //FFB04F - 3
    //FF02FD - ok
    //FF4AB5 - 0
    Serial.println(results.value, HEX);
    switch(results.value) {
      case 0xFFC23D: 
        turn++;
        if(turn > 3) turn = 3;
        Serial.println(turn);
        break;
      case 0xFF22DD: 
        turn--;
        if(turn < -3) turn = -3;
        Serial.println(turn);
        break;
      case 0xFF629D: 
        speed++;
        if(speed > 3) speed = 3;
        Serial.println(speed);
        break;
      case 0xFFA857:
        speed--;
        if(speed < -3) speed = -3;
        Serial.println(speed);
        break;
    }
    irrecv.resume();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //test();
//  if(digitalRead(FRONTSENSOR) == HIGH)
//  {
//    Serial.println("Something on front");
//  }

  getIRCommands();

  int deltaLeft = 0;
  int deltaRight = 0;

  if(turn < 0) {
    deltaLeft = -turn * 50;
  } else if(turn > 0) {
    deltaRight = turn * 50;
  }

  if(speed > 0) {
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    
    analogWrite(M1, speed*50 + 105 - deltaRight);
    analogWrite(M2, speed*50 + 105 - deltaLeft);
  } else if( speed < 0) {
    digitalWrite(D1, HIGH);
    digitalWrite(D2, HIGH);

    analogWrite(M1, 255 - (-speed*50 + 105));
    analogWrite(M2, 255 - (-speed*50 + 105));
  } else {
    digitalWrite(D1, LOW);
    digitalWrite(D2, LOW);
    analogWrite(M1, 0);
    analogWrite(M2, 0);
  }
}
