#include <SoftwareSerial.h>

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

#define CONNECTION_RATE 9600 //rate of servant module

const String numberAlphabet = "abCdEFghIJ";

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

//decode string from scrambled alphabet to regular numbers
String decodeNumbers(String input){
  String s = "";
  const int len = input.length();
  for(int i = 0; i < len; ++i){
    char currentChar = input.charAt(i);
    int index = numberAlphabet.indexOf(currentChar);
    //  convert input to numbers
    if(index > -1){
      currentChar = (char)index + '0';
    }
    s += currentChar;
  }
  return s;
}

void handleThrottle(String input){
  input = decodeNumbers(input);
  int commaIndex = input.indexOf(',');
  //x: -5 is left, 5 is right
  //y: 5 is up, -5 is down
  int x = input.substring(0, commaIndex).toInt() - 5;
  int y = input.substring(commaIndex+1).toInt() - 5;

  Serial.println("Throttle params: " + input + "-> " + x + "," + y);
}

String input;
void loop() {
  input = readModule();

  if(input.length() > 0){
    if(input.charAt(0) == 'T'){ //throttle handling message
      handleThrottle(input.substring(1));
    }
  }

  // input independent code goes here

}
