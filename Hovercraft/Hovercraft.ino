#include <SoftwareSerial.h>

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

#define CONNECTION_RATE 9600 //rate of servant module

void setup() {
  Serial.begin(38400);
  btModule.begin(CONNECTION_RATE);

  // other init code go here
  
  Serial.println("Ready");
}

String readModule(){
  String s = "";
  if(btModule.available()){
    
    delay(50); //wait for message to fully transmit
    while(btModule.available() > 0){
      s += (char)btModule.read();
    }
    Serial.println("Received: " + s);
  }
  return s;
}

String input;
void loop() {
  input = readModule();

  if(input.length() > 0){
    // input handling goes here
  }

  // input independent code goes here

}
