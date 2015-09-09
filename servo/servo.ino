#include <Servo.h>

Servo s1;
Servo s2;
int angle = 0;
int lastAngle = 0;

void setup() {
  // put your setup code here, to run once:
  s1.attach(3);
  s2.attach(10);
  Serial.begin(9600);
  rotate(&s1, 15);
  rotate(&s2, 15); //max

  analogReference(DEFAULT);
}

void rotate(Servo *servo, int angle) {
  if(angle < 15) angle = 15;
  if(angle > 155) angle = 155;
  servo->write(angle);
  Serial.print("Rotate to ");
  Serial.println(angle);
}

unsigned long time = 0;

void loop() {
  delay(10);
  if (Serial.available() > 0) {  
    int ang = Serial.parseInt();
    if(ang != lastAngle) {
      rotate(&s1, ang);
      rotate(&s2, ang);
      lastAngle = ang;
    }
    time = millis();
    angle = ang - 15;
  } else if (millis() - time > 10000){
      angle = map(analogRead(0), 0, 1024, 15, 155);
      rotate(&s1, angle);
      angle = map(analogRead(1), 0, 1024, 15, 155);
      rotate(&s2, angle);
      delay(250);
  }
}
