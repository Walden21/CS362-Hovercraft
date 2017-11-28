#include <SoftwareSerial.h>
#include <Servo.h>

// servo circuit reference: http://www.ibrahimlabs.com/2013/08/how-to-interface-servo-motor-with-pic.html
// -> Brown to ground, red to VCC (or nothing if independently powered), orange to data
// servo code reference: https://www.arduino.cc/en/Tutorial/Knob
// Electronic Speed Control (ESC) code based on http://www.instructables.com/id/ESC-Programming-on-Arduino-Hobbyking-ESC/

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

Servo backServo1;
Servo backServo2;
const int BACKSERVO1_PIN = 4;
const int BACKSERVO2_PIN = 5;

#define CONNECTION_RATE 9600 //rate of servant module

const String numberAlphabet = "abCdEFghIJ";

void setup() {
  Serial.begin(38400);
  btModule.begin(CONNECTION_RATE);

  backServo1.attach(BACKSERVO1_PIN);
  backServo2.attach(BACKSERVO2_PIN);
  
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
  //x: -10 is left, 10 is right
  //y: 10 is up, -10 is down
  int x = input.substring(0, commaIndex).toInt() - 10;
  int y = input.substring(commaIndex+1).toInt() - 10;

  int mapX = map(x, -10, 10, 0, 180);
  backServo1.write(mapX);
  backServo2.write(180-mapX); //turn in opposite direction
  

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
