// master and servant code based off of https://github.com/rwaldron/johnny-five/wiki/Getting-Started-with-Johnny-Five-and-JY-MCU-Bluetooth-Serial-Port-Module
// and https://github.com/rwaldron/johnny-five/wiki/Getting-Started-with-Johnny-Five-and-HC-05-Bluetooth-Serial-Port-Module

#include <SoftwareSerial.h>

#define MASTER_NAME "Controller1"
#define SERVANT_NAME "Craft1"

#define PIN_PSWD "1234" //be sure that pairings have different pins

//#define HC06_ID "2016,12,128353" //result from AT+INQ in master node for pairing 1
//if result of AT+INQ is +INQ:2016:12:128353,1F00,7FFF, 
//the ID is 2016,12,128353 (replace colons with commas)
#define HC06_ID "2016,11,21314" //+INQ:2016:11:21314,1F00,7FFF for pairing two

#define MASTER_SPEED 38400 //default for HC-05
#define SERVANT_SPEED 9600 //default for HC-06

// Swap RX/TX connections on bluetooth chip
//   Pin 10 --> Bluetooth TX
//   Pin 11 --> Bluetooth RX
SoftwareSerial masterSerial(10, 11); // RX, TX


// Swap RX/TX connections on bluetooth chip
//   Pin 4 --> Bluetooth TX
//   Pin 5 --> Bluetooth RX
SoftwareSerial servantSerial(4, 5); // RX, TX

void setup() {
  String result;
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin, edge pin closest to 5V) HIGH to switch module to AT mode
  digitalWrite(9, HIGH);
  
  result = initMaster();
  if(hasError(result, "")){
    return;
  }
  result = initServant();
  if(hasError(result, "")){
    return;
  }

  result = pair(HC06_ID);
  if(hasError(result, "")){
    return;
  }


  Serial.println("\nDone with full setup");
}

bool hasError(String result, String expected){
  if(result != expected){
    Serial.print(result);
    return true;
  }else{
    return false;
  }
}

String initMaster(){
  masterSerial.begin(MASTER_SPEED);
  delay(1000);
  String result;

  // Should respond with OK
  result = masterSend("AT");
  if(result != "OK\r\n"){
    Serial.print(result);
    return ("Error: Can't activate master AT Mode");
  }

  // Should respond with its version
//  masterSend("AT+VERSION");

  //init pin to 0000
  result = masterSend("AT+PSWD=" + String(PIN_PSWD));
  if(result != "OK\r\n"){
    Serial.print(result);
    return ("Error: Can't set master PIN");
  }

  return "";
}

String initServant(){
  servantSerial.begin(SERVANT_SPEED);
  delay(1000);

  String result;

  // Should respond with OK
  result = servantSend("AT", true);
  if(result != "OK"){
    Serial.print(result);
    return ("Error: Can't activate AT Mode");
  }

  // Should respond with its version
//  servantSend("AT+VERSION", true);

  result = servantSend("AT+PIN" + String(PIN_PSWD), true);
  if(result != "OKsetPIN"){
    Serial.print(result);
    return ("Error: Can't set servant PIN");
  }
  
  return "";
}

String pair(String target){
  masterSerial.begin(MASTER_SPEED);
  //pair code based off of http://www.martyncurrey.com/connecting-2-arduinos-by-bluetooth-using-a-hc-05-and-a-hc-06-pair-bind-and-link/
  masterSend("AT+RMAAD");
  masterSend("AT+ROLE=1");
  masterSend("AT+RESET");

  masterSend("AT+CMODE=0");
  masterSend("AT+INQM=0,5,9");
  masterSend("AT+INIT");
  masterSend("AT+INQ");

  masterSend("AT+RNAME?" + target);

  masterSend("AT+PAIR=" + target + ",9");

  masterSend("AT+BIND=" + target);
  masterSend("AT+CMODE=1");
  masterSend("AT+LINK=" + target);
  return "";
}

String masterSend(String message){
  Serial.println("Sending master message: " + message);
  masterSerial.print(message + "\r\n");
  return waitForResponse(masterSerial);
}

String servantSend(String message, bool doWait){
  Serial.println("Sending servant message: " + message);
  servantSerial.print(message);
  if(doWait)
    return waitForResponse(servantSerial);
  else
    return "";
}

String waitForResponse(SoftwareSerial &mySerial) {
    delay(1000);
    Serial.println("Response: ");
    String result = "";
    while (mySerial.available()) {
      char input = mySerial.read();
      Serial.write(input);
      result += String(input);
    }
    Serial.write("\n");
    return result;
}

void loop() {}
