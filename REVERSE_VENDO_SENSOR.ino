
#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"
#define TX_PIN 7 // Arduino transmit  
#define RX_PIN 8 // Arduino receive   
SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

#include <SoftwareSerial.h>
SoftwareSerial ss(10, 11); // RX, TX

const int btnStart = 3;
const int btnStop  = 2;
const int btnPet   = 4;
const int btnCan   = 5;
const int btnGlss  = 6;

const int petSensPin  = 14;
const int canSensPin  = 15;
const int irSensPin = 17;

#define ledPet  9
#define ledCan  18
#define ledGlss 19

int startFl, points, adcFsr;
int glassCtr, petCtr, canCtr, rejCtr, wstCtr;

void setup() {
  // put your setup code here, to run once:
  delay(100);
  Serial.begin(9600); Serial.println("SENSOR");
  mySerial.begin(9600);  // Initialize SoftwareSerial
  printer.begin();        // Init printer (same regardless of serial type)

  ss.begin(9600); ss.listen();

  pinMode(ledPet, OUTPUT); digitalWrite(ledPet, 0);
  pinMode(ledCan, OUTPUT); digitalWrite(ledCan, 0);
  pinMode(ledGlss, OUTPUT); digitalWrite(ledGlss, 0);

  pinMode(petSensPin, INPUT);
  pinMode(canSensPin, INPUT);
  pinMode(irSensPin, INPUT);

  pinMode(btnStart, INPUT_PULLUP);
  pinMode(btnStop, INPUT_PULLUP);
  pinMode(btnPet, INPUT_PULLUP);
  pinMode(btnCan, INPUT_PULLUP);
  pinMode(btnGlss, INPUT_PULLUP);

//  tests();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(btnStart) == 0) {
    delay(20); while (digitalRead(btnStart) == 0);
    Serial.println("A"); ss.print("A");
    startFl = 1;
  }
  if (digitalRead(btnStop) == 0) {
    delay(20); while (digitalRead(btnStop) == 0);
    Serial.println("B"); ss.print("B");
    printReceipt(); startFl = 0; points = 0;
  }
  if (digitalRead(btnPet) == 0) {
    delay(20); while (digitalRead(btnPet) == 0);
    Serial.println("P"); ss.print("P");
    digitalWrite(ledPet, 0);
    petCtr = 0;
  }
  if (digitalRead(btnCan) == 0) {
    delay(20); while (digitalRead(btnCan) == 0);
    Serial.println("C"); ss.print("C");
    digitalWrite(ledCan, 0);
    canCtr = 0;
  }
  if (digitalRead(btnGlss) == 0) {
    delay(20); while (digitalRead(btnGlss) == 0);
    Serial.println("G"); ss.print("G");
    digitalWrite(ledGlss, 0);
    glassCtr = 0;
  }

  if (startFl == 1) {
    if (digitalRead(irSensPin) == 0) { // item detected
      delay(3000);
      adcFsr = read_fsr();

      if (digitalRead(canSensPin) == 0) { // alum can detected
        Serial.println("2"); ss.print("2"); delay(6000);
        if (canCtr < 30) {
          canCtr++; points += 3;
        }
        else {
          digitalWrite(ledCan, 1);
        }
      }
      else if (digitalRead(petSensPin) == 1) { // pet bottle detected
        Serial.println("1"); ss.print("1"); delay(5000);
        if (petCtr < 15) {
          petCtr++; points += 5;
        }
        else {
          digitalWrite(ledPet, 1);
        }
      }

      else if (adcFsr < 500 and digitalRead(petSensPin) == 1) { // glass bottle detected
        Serial.println("3"); ss.print("3"); delay(7000);
        if (glassCtr < 15) {
          glassCtr++; points += 10;
        }
        else {
          digitalWrite(ledGlss, 1);
        }
      }
      else {
        Serial.println("4"); ss.print("4"); delay(5000);
      }
    }
  }

}

void tests()
{
  while (1) {
    adcFsr = read_fsr();
//    Serial.print(digitalRead(btnStart)); Serial.print(" ");
//    Serial.print(digitalRead(btnStop)); Serial.print(" ");
//    Serial.print(digitalRead(btnPet)); Serial.print(" ");
//    Serial.print(digitalRead(btnCan)); Serial.print(" ");
//    Serial.print(digitalRead(btnGlss)); Serial.println(" ");
//
//    digitalWrite(ledPet, digitalRead(btnPet));
//    digitalWrite(ledCan, digitalRead(btnCan));
//    digitalWrite(ledGlss, digitalRead(btnGlss));

        Serial.print(digitalRead(petSensPin)); Serial.print(" "); //T 1
        Serial.print(digitalRead(canSensPin)); Serial.print(" "); //T 0
        Serial.print(digitalRead(irSensPin)); Serial.print(" ");  //T 0
        Serial.print(adcFsr); Serial.println(" ");
    delay(200);

//    if (digitalRead(btnStart) == 0) {
//      printReceipt();
//    }
  }
}
int read_fsr()
{
  int times = 500, adc;
  unsigned long tAdc = 0;
  for (int x = 1; x <= times; x++)
  {
    adc = analogRead(A2); tAdc += adc; delayMicroseconds(50);
  }
  adc = tAdc / times;
  return adc;
}
void printReceipt()
{
  mySerial.listen(); mySerial.flush();
  printer.print(F(""));
  printer.setSize('M');
  printer.doubleHeightOn();

  printer.justify('C'); printer.println(F("Total Points is:"));
  printer.println(String(points));
  printer.justify('M'); printer.println(F("--------------------------------"));

  printer.println(F(""));
  printer.feed(5);
  ss.listen(); ss.flush();
}
