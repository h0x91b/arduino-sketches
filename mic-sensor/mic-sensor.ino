void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  Serial.println("Start mic");
}

int average = 0;

void loop() {
  // put your main code here, to run repeatedly:
  int micVal = analogRead(A0);
  average = (micVal + average)/2 + 1;
  if(micVal > 519) {
    digitalWrite(13, HIGH);
  }
  else
    digitalWrite(13, LOW);

  if(micVal <= 519) return;

  Serial.print("Average: ");
  Serial.print(average);
  Serial.print(" Current: ");
  Serial.println(micVal);
}
