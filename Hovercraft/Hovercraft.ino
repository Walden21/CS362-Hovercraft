#include <SoftwareSerial.h>
#include <Servo.h>

// servo circuit reference: http://www.ibrahimlabs.com/2013/08/how-to-interface-servo-motor-with-pic.html
// -> Brown to ground, red to VCC (or nothing if independently powered), orange to data
// servo code reference: https://www.arduino.cc/en/Tutorial/Knob
// Electronic Speed Control (ESC) code based on http://www.instructables.com/id/ESC-Programming-on-Arduino-Hobbyking-ESC/
// ESC has a regulated 5v~ output for standard receivers. Use that as the power jump to the servo.
// PWM pin reference: https://www.arduino.cc/reference/en/language/functions/analog-io/analogwrite/


// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

Servo backServo1;
Servo backServo2;
const int BACKSERVO1_PIN = 5;
const int BACKSERVO2_PIN = 6;

Servo thrustMotor, inflationMotor;
const int THRUST_PIN = 9;
const int INFLATION_PIN = 10;

//bool notPressed = true; //change when analog stick value has changed from 0

#define CONNECTION_RATE 9600 //rate of servant module

const String numberAlphabet = "abCdEFghIJ";

void setup() {
  Serial.begin(38400);
  btModule.begin(CONNECTION_RATE);

  backServo1.attach(BACKSERVO1_PIN);
  backServo2.attach(BACKSERVO2_PIN);

  thrustMotor.attach(THRUST_PIN);
  inflationMotor.attach(INFLATION_PIN);
  
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

//speed range is 0 to 10, thrust range is 700 to 1500
void setThrustSpeed(int speed){
  int thrust = map(speed,0,10,700,1500);
  Serial.println("Setting thrust to " + String(thrust));
  thrustMotor.writeMicroseconds(thrust);
}

void handleThrottle(String input){
  input = decodeNumbers(input);
  int commaIndex = input.indexOf(',');
  //x: -10 is left, 10 is right
  //y: 10 is up, -10 is down
  int x = input.substring(0, commaIndex).toInt() - 10;
  int y = input.substring(commaIndex+1).toInt() - 10;

  int mapX = map(x, -10, 10,20, 160);
  backServo1.write(mapX);
  backServo2.write(mapX); //turn in opposite direction

  if(y > 0){
    inflationMotor.writeMicroseconds(1500);
    thrustMotor.writeMicroseconds(1500);
  }else{
    inflationMotor.writeMicroseconds(700);
    thrustMotor.writeMicroseconds(700);
  }
  

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
