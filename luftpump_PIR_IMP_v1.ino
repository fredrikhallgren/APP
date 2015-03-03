/* 
 * //////////////////////////////////////////////////
 * //Controlling Mitsibishi airpump wit PIR and IR Remote depending on temperature reading from DHT22
 * //////////////////////////////////////////////////
 *
 * Sends a slow fan command when PIR detects motion
 * Sends a high fan command when no motion for a while
 * No motion is set by both PIR module settings and sketch
 * 
 * Code used from @author: Kristian Gohlke / krigoo (_) gmail (_) com / http://krx.at
 * @date:   3. September 2006 
 * and a tutorial at http://www.ladyada.net/learn/sensors/ir.html
 * http://creativecommons.org/licenses/by-nc-sa/2.0/de/
 * 
 */

#include "DHT.h"
#include <SoftwareSerial.h>

/////////////////////////////
//VARS
//the time we give the sensor to calibrate (10-60 secs according to the datasheet)
int calibrationTime = 30;

//the time when the sensor outputs a low impulse
long unsigned int lowIn;         

//the amount of milliseconds the sensor has to be low 
//before we assume all motion has stopped
long unsigned int pause = 600000;  

boolean lockLow = true;
boolean takeLowTime;  

int pirPin = 13;    //the digital pin connected to the PIR sensor's output
int powerPin = 12;  // Pin to power up PIR
int IRledPin =  2;    // LED connected to digital pin
#define DHTPIN 4     // what pin we're connected DHT22 to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);
SoftwareSerial mySerial(8, 9); // RX, TX

/////////////////////////////
//SETUP
void setup(){
  Serial.begin(9600);
  mySerial.begin(19200);
  pinMode(pirPin, INPUT);
  pinMode(powerPin, OUTPUT);
  pinMode(IRledPin, OUTPUT); 
  digitalWrite(pirPin, LOW);
  digitalWrite(powerPin, HIGH);
  dht.begin();
  
  //give the sensor some time to calibrate
  Serial.print("calibrating sensor ");
    digitalWrite(powerPin, HIGH);
    for(int i = 0; i < calibrationTime; i++){
      Serial.print(".");
      delay(1000);
      }
    Serial.println(" done");
    Serial.println("SENSOR ACTIVE");
    delay(50);
  }

////////////////////////////
//LOOP
void loop(){
     if(digitalRead(pirPin) == HIGH){
       if(lockLow){  
         //makes sure we wait for a transition to LOW before any further output is made:
         lockLow = false;            
         Serial.println("---");
         Serial.print("motion detected at ");
         Serial.print(millis()/1000);
         Serial.println(" sec"); 
         delay(50);
         float h = dht.readHumidity();
         // Read temperature as Celsius
         float t = dht.readTemperature();
         Serial.print("Humidity: "); 
         Serial.print(h);
         Serial.print(" %\t");
         Serial.print("Temperature: "); 
         Serial.print(t);
         Serial.print(" *C ");
         mySerial.print(t);
         mySerial.println();
         mySerial.print(h);
         mySerial.print("\t");
           if (t < 25){
             SendIRCodeWinterLow();
             }
             else SendIRCodeNormalLow();    
         }         
         takeLowTime = true;
       }

     if(digitalRead(pirPin) == LOW){       
       if(takeLowTime){
        lowIn = millis();          //save the time of the transition from high to LOW
        takeLowTime = false;       //make sure this is only done at the start of a LOW phase
        }
       //if the sensor is low for more than the given pause, 
       //we assume that no more motion is going to happen
       if(!lockLow && millis() - lowIn > pause){  
           //makes sure this block of code is only executed again after 
           //a new motion sequence has been detected
           lockLow = true;                        
           Serial.print("motion ended at ");      //output
           Serial.print((millis() - pause)/1000);
           Serial.println(" sec");
           delay(50);
           float h = dht.readHumidity();
         // Read temperature as Celsius
         float t = dht.readTemperature();
         Serial.print("Humidity: "); 
         Serial.print(h);
         Serial.print(" %\t");
         Serial.print("Temperature: "); 
         Serial.print(t);
         Serial.print(" *C ");
         mySerial.print(t);
         mySerial.println();
         mySerial.print(h);
         mySerial.print("\t");
           if (t < 25){
             SendIRCodeWinterHigh();
               }
             else SendIRCodeNormalHigh();
           }
       }
  }

// The setup() method runs once, when the sketch starts
 
// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}
 
void SendIRCodeNormalLow() {
  // This is the code for my airpump fan speed one
  Serial.print("Sending NormalLow ");
pulseIR(4400);
delayMicroseconds(4400);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1500);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(450);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(600);
delayMicroseconds(450);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(650);
delayMicroseconds(1500);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1500);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1500);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(450);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(500);
pulseIR(500);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(600);
delayMicroseconds(450);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(600);
delayMicroseconds(450);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
}

void SendIRCodeNormalHigh() {
  // This is the code for my airpump at fan speed four
  Serial.print("Sending NormalHigh ");
pulseIR(4350);
delayMicroseconds(4400);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(600);
delayMicroseconds(450);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(500);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(1550);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(1600);
pulseIR(550);
delayMicroseconds(500);
pulseIR(550);
delayMicroseconds(500);
pulseIR(600);
delayMicroseconds(1550);
pulseIR(550);
delayMicroseconds(550);
pulseIR(550);
}

void SendIRCodeWinterLow() {
  // This is the code for my airpump at fan speed four
  Serial.print("Sending WinterLow ");
pulseIR(4320);
delayMicroseconds(4380);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
}

void SendIRCodeWinterHigh() {
  // This is the code for my airpump at fan speed four
  Serial.print("Sending WinterHigh ");
pulseIR(4320);
delayMicroseconds(4380);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1540);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1580);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(520);
pulseIR(540);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
delayMicroseconds(500);
pulseIR(540);
delayMicroseconds(1560);
pulseIR(560);
delayMicroseconds(500);
pulseIR(560);
}
