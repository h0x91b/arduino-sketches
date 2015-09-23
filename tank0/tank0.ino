#include <IRremote.h>

int A = 13;
int B = 12;
int C = 11;
int D = 10;
int IR = 8;
int LED = 7;

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Tank v0");
  pinMode(A, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(C, OUTPUT);
  pinMode(D, OUTPUT);
  pinMode(LED, OUTPUT);

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
        digitalWrite(LED, HIGH);
        delay(300);
        break;
      case 0xFF22DD: //left
        for(int i=0;i<50;i++)
          powerStepReverse(500000);
        break;
      case 0xFFC23D: //right
        for(int i=0;i<50;i++)
          powerStep(500000);
        break;
      default: 
        Serial.print("Got signal: ");
        Serial.println(results.value, HEX);
        dump(&results);
      break;
    }
    irrecv.resume();
  }
}
