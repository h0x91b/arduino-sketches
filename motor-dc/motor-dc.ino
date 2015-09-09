#define motor1 9
void setup() {
  // put your setup code here, to run once:
  pinMode(motor1, OUTPUT);
}

void loop() {
  for(int i=50;i<255;i++) {
    analogWrite(motor1, i);
    delay(5);
  }
  for(int i=255;i>50;i--) {
    analogWrite(motor1, i);
    delay(5);
  }
}
