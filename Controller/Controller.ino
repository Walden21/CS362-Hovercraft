#include <SoftwareSerial.h>

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

const char scrambledAlphabet[26] = "abCdEFghIJkLmnOpQRsTuvWXyz";

// analog stick setup reference: https://brainy-bits.com/blogs/tutorials/arduino-joystick-tutorial
const int analogX_pin = 4; //VRy
const int analogY_pin = 3; //VRx
const int analogSwitch_pin = 4;

//possible range of analog stick values
#define STICK_MIN 0
#define STICK_MAX 1023

#define CONNECTION_RATE 9600 //rate of servant module

void setup() {
  Serial.begin(9600);
  btModule.begin(CONNECTION_RATE);

  pinMode(analogSwitch_pin, INPUT);
  digitalWrite(analogSwitch_pin, LOW);
  
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

//reference: https://www.arduino.cc/reference/en/language/functions/math/map/
//scale to 0-1 scale
String scaleAnalog(int value){
  int mappedValue = map(value, STICK_MIN, STICK_MAX, 0, 20);
  return String(mappedValue);
}

//encode string from regular numbers to scrambled alphabet
String encodeNumbers(String input){
  String s = "";
  const int len = input.length();
  for(int i = 0; i < len; ++i){
    char currentChar = input.charAt(i);

    //replace number with character at index
    if(isDigit(currentChar)){
      currentChar = scrambledAlphabet[(int)currentChar - '0'];
    }

    Serial.print(String(input.charAt(i)) + "->" + currentChar + "| ");

    s += currentChar;
  }
  return s;
}

void loop() {
  sendFromSerial();

  String x = scaleAnalog(analogRead(analogX_pin));
  String y = scaleAnalog(analogRead(analogY_pin));
  String throttleMsg = encodeNumbers("T" + x + "," + y);

  Serial.println("X: " + x + ", Y: " + y + "| encoded: " + throttleMsg);
  sendString(throttleMsg);
  
  delay(50);
}
