#include <SoftwareSerial.h>

// Pin 2 --> Bluetooth TX
// Pin 3 --> Bluetooth RX
SoftwareSerial btModule(2, 3);

const char scrambledAlphabet[26] = "abCdEFghIJkLmnOpQRsTuvWXyz";

const int POT_PIN = 0; //analog 0
const int RED_PIN = 10,
          GREEN_PIN = 9,
          BLUE_PIN = 8;
const int RED_VAL = 0, GREEN_VAL = 0, BLUE_VAL = 255;
bool ledIsOn = false;

#define CONNECTION_RATE 9600 //rate of servant module

void setup() {
  Serial.begin(9600);
  btModule.begin(CONNECTION_RATE);

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

void toggleLed(){
  ledIsOn = !ledIsOn;

  analogWrite(RED_PIN, ledIsOn ? RED_VAL : 0);
  analogWrite(GREEN_PIN, ledIsOn ? GREEN_VAL : 0);
  analogWrite(BLUE_PIN, ledIsOn ? BLUE_VAL : 0);
}

void loop() {
  // put your main code here, to run repeatedly:
//  sendFromSerial();

  //map strobe delay to be between 250ms and 5 seconds
  int delayTime = map(analogRead(POT_PIN), 0, 1023, 250, 5000);
  Serial.println(delayTime);

  toggleLed();

  sendString(ledIsOn ? "OFF" : "On");

  delay(delayTime);

}
