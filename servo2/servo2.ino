#include <Servo.h>

Servo s1;
Servo s2;
int angle = 15;

int min = 900;
int max = 905;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  s1.attach(3);
  s2.attach(5);
}

void loop() {
  if (Serial.available() > 0) {  
    angle = Serial.parseInt();
    angle = angle < 0 ? 0 : angle;
    angle = angle > 100 ? 100 : angle;
    angle = map(angle, 0, 100, 15, 155);
  }
  s1.write(angle);
  int cur = analogRead(0);
  Serial.println(cur);
  if(cur < min) min = cur;
  if(cur > max) max = cur;
  s2.write(map(cur, min, max, 15, 155));
}
