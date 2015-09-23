#include <IRremote.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3f,16,2);

int A = 8;
int B = 9;
int C = 10;
int D = 11;
int IR = 7;
int LED = 6;
int buzzer = 12;

//motor left
int IN1 = 2;
int IN2 = 3;
//motor right
int IN3 = 4;
int IN4 = 5;

int sound = 12;

IRrecv irrecv(IR);
decode_results results;

int pinOrder[] = {
  A, B, C, D
};

void powerStep(int brake) {
  brake = brake < 5000 ? 5000 : brake;
  for(int i=0;i<4;i++) {
    int offPin = i + 3; //for some reason i+3 % 4 give me 4,5,6, have no idea why...
    int onPin = i + 1;
    if(offPin >= 4) offPin -= 4;
    if(onPin >= 4) onPin -= 4;
    digitalWrite(pinOrder[offPin], LOW); //3 0
    digitalWrite(pinOrder[i], HIGH); //0 1
    digitalWrite(pinOrder[onPin], HIGH); //1 2
    delayMicroseconds(brake);
  }
}

int pinOrderReverse[] = {
  D, C, B, A
};

void powerStepReverse(int brake) {
  brake = brake < 5000 ? 5000 : brake;
  for(int i=0;i<4;i++) {
    int offPin = i + 3; //for some reason i+3 % 4 give me 4,5,6, have no idea why...
    int onPin = i + 1;
    if(offPin >= 4) offPin -= 4;
    if(onPin >= 4) onPin -= 4;
    digitalWrite(pinOrderReverse[offPin], LOW); //3 0
    digitalWrite(pinOrderReverse[i], HIGH); //0 1
    digitalWrite(pinOrderReverse[onPin], HIGH); //1 2
    delayMicroseconds(brake);
  }
}

void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  Serial.print("Dump ");
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

void buzz(int targetPin, long frequency, long length) {
  digitalWrite(13,HIGH);
  long delayValue = 1000000/frequency/2; // calculate the delay value between transitions
  //// 1 second's worth of microseconds, divided by the frequency, then split in half since
  //// there are two phases to each cycle
  long numCycles = frequency * length/ 1000; // calculate the number of cycles for proper timing
  //// multiply frequency, which is really cycles per second, by the number of seconds to 
  //// get the total number of cycles to produce
  for (long i=0; i < numCycles; i++){ // for the calculated length of time...
    digitalWrite(targetPin,HIGH); // write the buzzer pin high to push out the diaphram
    delayMicroseconds(delayValue); // wait for the calculated delay value
    digitalWrite(targetPin,LOW); // write the buzzer pin low to pull back the diaphram
    delayMicroseconds(delayValue); // wait again or the calculated delay value
  }
  digitalWrite(13,LOW);

}

//5x6 bit
uint8_t heart[8] = {0x15, 0xa, 0x15, 0xa, 0x15, 0xa, 0x15};

#if defined(ARDUINO) && ARDUINO >= 100
#define printByte(args)  write(args);
#else
#define printByte(args)  print(args,BYTE);
#endif

void displayKeyCodes(void) {
  uint8_t i = 0;
  while (1) {
    lcd.clear();
    lcd.print("Codes 0x"); lcd.print(i, HEX);
    lcd.print("-0x"); lcd.print(i+16, HEX);
    lcd.setCursor(0, 1);
    for (int j=0; j<16; j++) {
      lcd.printByte(i+j);
    }
    i+=16;
    
    delay(4000);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Tank v1");
  char buf[32] = {0};
  sprintf(buf, "%u", 'А');
  Serial.println(buf);
  sprintf(buf, "%u", 'Б');
  Serial.println(buf);
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(buzzer, OUTPUT);//buzzer

  lcd.init();
  lcd.backlight();

  lcd.createChar(0, heart);
  lcd.home();
  
  lcd.setCursor(0,0);
  lcd.print("Tank, version 1");
  lcd.printByte(0);
  lcd.setCursor(0,1);
  lcd.print("By h0x91b");

  delay(7000);

  //displayKeyCodes();

  lcd.clear();
  lcd.noBacklight();

  buzz(buzzer, 1000, 250);
  delay(250);
  buzz(buzzer, 1000, 250);

  irrecv.enableIRIn(); // Start the receiver
}

void loop() {
  // put your main code here, to run repeatedly:
//  for(int i=0;i<100;i++)
//  powerStep(500000);
  digitalWrite(LED, LOW);

  if (irrecv.decode(&results)) {
    switch(results.value) {
      case 0xFF02FD: //ok
        lcd.backlight();
        lcd.setCursor(0, 0);
        lcd.print("Fire!");
        Serial.println("Fire!");
        digitalWrite(LED, HIGH);
        buzz(buzzer, 200, 150);
        delay(300);
        lcd.clear();
        lcd.noBacklight();
        break;
      case 0xFF22DD: //left
        Serial.println("Tower left");
        for(int i=0;i<50;i++)
          powerStepReverse(500000);
        break;
      case 0xFFC23D: //right
        Serial.println("Tower right");
        for(int i=0;i<50;i++)
          powerStep(500000);
        break;
      case 0xFF629D: //up
      case 0xFF9867: //#2
        Serial.println("Move forward");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN3, HIGH);
        delay(500);
        break;
      case 0xFFA857: //down
      case 0xFF38C7: //#8
        Serial.println("Move backward");
        digitalWrite(IN2, HIGH);
        digitalWrite(IN4, HIGH);
        delay(500);
        break;
      case 0xFF30CF: //#4
        Serial.println("Left");
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        delay(600);
        break;
      case 0xFF7A85: //#6
        Serial.println("RIGHT");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN4, HIGH);
        delay(600);
        break;
      default: 
        Serial.print("Got signal: ");
        Serial.println(results.value, HEX);
        dump(&results);
      break;
    }
    irrecv.resume();
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  digitalWrite(A, LOW);
  digitalWrite(B, LOW);
  digitalWrite(C, LOW);
  digitalWrite(D, LOW);
}
