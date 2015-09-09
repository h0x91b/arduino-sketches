#define rxd     2   // RX data


#define delta   200
#define mintime 200
#define maxtime 2000

unsigned int time,oldtime,count,log0,log1;
byte data[8];


void setup() {                
  initPins();                       // Настроить входы-выходы
                    // Настроить трансивер на 433.920 модуляция AM
  attachInterrupt(0,pinint,CHANGE); // Настроить прерывание
  Serial.begin(115200);             // Настроить UART
  Serial.println("Starline 64 bit  receiver from Astronom");
  interrupts();
}
void initPins(void){
  
  pinMode(rxd,INPUT);
}

  

void pinint(){
  if(digitalRead(rxd)==HIGH)oldtime=micros();
  else{
    time=micros()-oldtime;
    if(time<mintime) return;
    if(time>maxtime) {
      //oldtime=micros();
      return;
    }
    Serial.println(time);
//    if(constrain(time,log0-delta,log0+delta)==time)savebit(false);
//    else if(constrain(time,log1-delta,log1+delta)==time)savebit(true);
//    else{
//      log1=time/2;
//      log0=time/4;
//      if(count==64){
//        printpack();
//      }
//      count=0;
//    }
  }
}
void savebit(boolean val){
  for(int i=7;i>=0;i--){
    data[i]=data[i]>>1;
    if(i>0)if(data[i-1]&1)data[i]|=0x80;
  }
  if(val)data[0]|=0x80;
  count++;
}
void printhex(byte val){
  if(val<=0x0F)Serial.print("0");
  Serial.print(val,HEX);
}
void printpack(void){
  Serial.print("STARLINE ");
  for(int i=7;i>=0;i--)printhex(data[i]);
  Serial.println("");
}
void loop(){}
