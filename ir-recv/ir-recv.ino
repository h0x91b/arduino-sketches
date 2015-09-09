/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 */

#include <IRremote.h>

/* 
*  Default is Arduino pin D11. 
*  You can change this to another available Arduino Pin.
*  Your IR receiver should be connected to the pin defined here
*/
int RECV_PIN = 9; 
int RED_PIN = 4;
int GREEN_PIN = 2;

bool is_turned_on = false;

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup()
{
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Start");
  irrecv.enableIRIn(); // Start the receiver
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

unsigned long lastSend = 0;

void loop() {
  unsigned long diff = micros() - lastSend;
  //Serial.println(diff);
  if(diff > 1000000) {
//  if(lastSend == 0) {
    Serial.println("send tv command");
    lastSend = micros();
    //irsend.sendNEC(0x10EFC13E, 32); //uo
    //irsend.sendNEC(0x10EF619E, 32);
    irsend.sendNEC(0x8090048A, 32);
    //irsend.sendNEC(0xFFFFFFFF, 0);
    irrecv.enableIRIn();
  }
  if (irrecv.decode(&results)) {
    //if(results.value == 0xFFFFFFFF) return; //ignore
    switch(results.value) {
      case 0x8810040A:
      case 0x8890040A:
      case 0x10EF619E:
        Serial.println("Got turn off signal");
        is_turned_on = false;
        break;
      case 0x9810060A: //24C
      case 0x9890060A: //25C
      case 0x10EFC13E:
        Serial.println("Got turn on");
        is_turned_on = true;
        break;
      default: 
        Serial.print("Got signal: ");
        Serial.println(results.value, HEX);
        dump(&results);
      break;
    }
    irrecv.resume(); // Receive the next value
  }

  if(is_turned_on) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, HIGH);
  } else {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  }
}
