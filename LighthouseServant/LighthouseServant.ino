#include <SoftwareSerial.h>

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

#define CONNECTION_RATE 9600 //rate of servant module

const String numberAlphabet = "abCdEFghIJ";

const int RED_PIN = 10,
          GREEN_PIN = 9,
          BLUE_PIN = 8;
const int RED_VAL = 0, GREEN_VAL = 0, BLUE_VAL = 255;

void setup() {
  Serial.begin(38400);
  btModule.begin(CONNECTION_RATE);

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

String input;
void loop() {
  input = readModule();

  if(input.length() > 0){
    bool ledIsOn = input == "On";
    analogWrite(RED_PIN, ledIsOn ? RED_VAL : 0);
    analogWrite(GREEN_PIN, ledIsOn ? GREEN_VAL : 0);
    analogWrite(BLUE_PIN, ledIsOn ? BLUE_VAL : 0);
  }

}
