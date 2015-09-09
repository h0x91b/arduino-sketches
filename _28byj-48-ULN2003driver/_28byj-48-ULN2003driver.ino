//https://youtu.be/B86nqDRskVU?t=614

//28BYJ-48 with driver ULN2003
int pinBlue = 2; //A on driver board
int pinPurple = 3; //B on driver board
int pinYellow = 4; //C on driver board
int pinRed = 5; //D on driver board

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("start");
  pinMode(pinBlue, OUTPUT);
  pinMode(pinPurple, OUTPUT);
  pinMode(pinYellow, OUTPUT);
  pinMode(pinRed, OUTPUT);
}

int pinOrder[] = {
  pinBlue, pinPurple, pinYellow, pinRed
};

void step(int brake, bool clockwise = true) {
  brake = brake < 5000 ? 5000 : brake;
  if(clockwise) {
    for(int i=0;i<4;i++) {
      digitalWrite(pinOrder[i], HIGH);
      delayMicroseconds(brake);
      digitalWrite(pinOrder[i], LOW);
      delayMicroseconds(brake);
    }
  } else {
    for(int i=3;i>=0;i--) {
      digitalWrite(pinOrder[i], HIGH);
      delayMicroseconds(brake);
      digitalWrite(pinOrder[i], LOW);
      delayMicroseconds(brake);
    }
  }
}

//2 phase at time
void powerStep(int brake) {
  brake = brake < 5000 ? 5000 : brake;
  for(int i=0;i<4;i++) {
    int offPin = i + 3; //for some reason i+3 % 4 give me 4,5,6, have no idea why...
    int onPin = i + 1;
    if(offPin >= 4) offPin -= 4;
    if(onPin >= 4) onPin -= 4;
//    Serial.print("off ");
//    Serial.print(offPin);
//    Serial.print(" on ");
//    Serial.print(i);
//    Serial.print(" and ");
//    Serial.println(onPin);
    digitalWrite(pinOrder[offPin], LOW); //3 0
    digitalWrite(pinOrder[i], HIGH); //0 1
    digitalWrite(pinOrder[onPin], HIGH); //1 2
    delayMicroseconds(brake);
  }
}

void halfStep(int brake) {
  brake = brake < 5000 ? 5000 : brake;
  for(int i=0;i<4;i++) {
    digitalWrite(pinOrder[i], HIGH);
    delayMicroseconds(brake);
    
    digitalWrite(pinOrder[i + 1 % 4], HIGH);
    delayMicroseconds(brake);
    
    digitalWrite(pinOrder[i], LOW);
    delayMicroseconds(brake);
  }
}

int active = 0;
//not so fast...
void fastStep(int brake) {
  brake = brake < 5000 ? 5000 : brake;
  active += 2;
  digitalWrite(pinOrder[active % 4], HIGH);
  delayMicroseconds(brake);
  digitalWrite(pinOrder[active % 4], LOW);
  delayMicroseconds(brake);
}

void loop() {
  //step(5000, false);
  //halfStep(5000);
  powerStep(5000);
  //fastStep(5000);
}
