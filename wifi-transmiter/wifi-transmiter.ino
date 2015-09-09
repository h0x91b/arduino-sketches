#include <VirtualWire.h>

void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
  //vw_set_ptt_inverted(true); // Required for DR3100
  //vw_set_rx_pin(2);
  vw_set_tx_pin(2);
  vw_setup(2000);  // Bits per sec
  pinMode(13, OUTPUT);
  
  //vw_rx_start();       // Start the receiver PLL running
}

byte count = 1;

void sendaaa() {
  Serial.print("send(");
  Serial.print(count);
  Serial.println(")");
  char msg[7] = {'h','e','l','l','o',' ','#'};

  msg[6] = count;
  vw_send((uint8_t *)msg, 7);
  //Serial.println(vx_tx_active());
  vw_wait_tx(); // Wait until the whole message is gone
  count = count + 1;
}

int loopCnt = 0;

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if(++loopCnt > 1000) {
    loopCnt = 0;
    sendaaa();
  }
  delay(1);
  
//  if (vw_get_message(buf, &buflen)) // Non-blocking
//  {
//    int i;
//
//    digitalWrite(13, true); // Flash a light to show received good message
//    // Message with a good checksum received, dump it.
//    Serial.print("Got: ");
//    
//    for (i = 0; i < buflen; i++)
//    {
//        Serial.print(buf[i], HEX);
//        Serial.print(" ");
//    }
//    Serial.println("");
//    digitalWrite(13, false);
//  }
  
}
