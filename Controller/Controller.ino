#include <SoftwareSerial.h>

// Pin 10 --> Bluetooth TX
// Pin 11 --> Bluetooth RX
SoftwareSerial btModule(10, 11);

#define CONNECTION_RATE 9600 //rate of servant module

void setup() {
  Serial.begin(9600);
  btModule.begin(CONNECTION_RATE);

  // other init code go here  
  
  Serial.println("Ready");
}

void sendString(String data){
  Serial.println("Sending data: " + data);
  int len = data.length();
  for(int i = 0; i < len; ++i){
    delay(1);
    btModule.write(data.charAt(i));
  }
}

void sendFromSerial(){
  if(Serial.available()){
    String s = "";
    delay(50); //wait for message to fully load into Serial
    while(Serial.available() > 0){
      s += (char)Serial.read();
    }
    sendString(s);
  }
}

void loop() {
  sendFromSerial();

  //other input handlers go here

}
