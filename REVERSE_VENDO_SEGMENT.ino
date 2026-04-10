#include <SoftwareSerial.h>
SoftwareSerial ss(10, 11); // RX, TX

#include <Servo.h>
Servo srvDoor;  // create servo object to control a servo
Servo srvDoor2;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

#define l_step   8
#define l_dir    9
#define l_ena    12

#define ones1 2
#define ones2 3
#define ones3 4
#define ones4 5
#define onesC 16

#define tens1 6
#define tens2 7
#define tensE 15

const int lmtHome = 13;

int lDelay, steps;
int glassCtr, petCtr, canCtr, rejCtr, wstCtr;
int tens, ones, points;

void setup() {
  // put your setup code here, to run once:
  delay(100);
  Serial.begin(9600); Serial.println("SEGMENT");
  ss.begin(9600); ss.listen();

  lcd.init();                      // initialize the lcd
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("Reverse Vending");
  lcd.setCursor(0, 1); lcd.print("Machine");

  srvDoor.attach(14);
  srvDoor2.attach(17);
  close_door();

  pinMode(l_step, OUTPUT); digitalWrite(l_step, 1);
  pinMode(l_dir, OUTPUT); digitalWrite(l_dir, 1);
  pinMode(l_ena, OUTPUT); digitalWrite(l_ena, 1);
  pinMode(lmtHome, INPUT_PULLUP);

  pinMode(ones1, OUTPUT); digitalWrite(ones1, 0);
  pinMode(ones2, OUTPUT); digitalWrite(ones2, 0);
  pinMode(ones3, OUTPUT); digitalWrite(ones3, 0);
  pinMode(ones4, OUTPUT); digitalWrite(ones4, 0);

  pinMode(tens1, OUTPUT); digitalWrite(tens1, 0);
  pinMode(tens2, OUTPUT); digitalWrite(tens2, 0);

  pinMode(onesC, OUTPUT); digitalWrite(onesC, 1);
  pinMode(tensE, OUTPUT); digitalWrite(tensE, 0);

  wstCtr = 0; points = 0;
  lDelay = 25000;

//  tests();

  step_home();

  lcd.clear();
}

void loop() {
  // put your main code here, to run repeatedly:

  lcd.setCursor(0, 0); lcd.print("Press Start Button");
  lcd.setCursor(0, 1); lcd.print("to Start Transaction");

  if (ss.available()) {
    char rx = ss.read();
    if (rx == 'A') { // start button was pressed
      lcd.clear();
      while (1) {
        lcd.setCursor(0, 0); lcd.print("Insert Item or Press");
        lcd.setCursor(0, 1); lcd.print("Stop Button to");
        lcd.setCursor(0, 2); lcd.print("Stop Transaction");
        lcd.setCursor(0, 3); lcd.print("Points: "); lcd.print(wstCtr);

        if (ss.available()) {
          char rx = ss.read();
          if (rx == 'B') { // stop button was pressed
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Total Points is "); lcd.print(wstCtr);
            Serial.print("Points: "); Serial.println(wstCtr);
            wstCtr = 0; display_count(wstCtr);
            delay(3000); break;
          }
          else if (rx == '1') { //pet bottle detected
            if (petCtr < 30) {
              petCtr++; lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item is ");
              lcd.setCursor(0, 1); lcd.print("PET Bottle");
              goto_bin(210); delay(500); open_door(); delay(2000);
              close_door(); step_home(); lcd.clear();
              wstCtr+=5; display_count(wstCtr);
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item's bin");
              lcd.setCursor(0, 1); lcd.print("is full");
              delay(500); open_door(); delay(2000);
              close_door(); step_home(); lcd.clear();
            }

          }
          else if (rx == '2') { //alum can detected
            if (canCtr < 15) {
              canCtr++; lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item is ");
              lcd.setCursor(0, 1); lcd.print("Aluminum Can");
              goto_bin(430); delay(500); open_door(); delay(2000);
              close_door(); step_home(); lcd.clear();
              wstCtr+=3; display_count(wstCtr);
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item's bin");
              lcd.setCursor(0, 1); lcd.print("is full");
              delay(500); open_door(); delay(2000);
              close_door(); step_home(); lcd.clear();
            }
          }
          else if (rx == '3') { //glass bottle detected
            if (glassCtr < 15) {
              glassCtr++; lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item is ");
              lcd.setCursor(0, 1); lcd.print("GLASS Bottle");
              goto_bin(650); delay(500); open_door(); delay(2000);
              close_door(); step_home(); lcd.clear();
              wstCtr+=10; display_count(wstCtr);
            }
            else {
              lcd.clear();
              lcd.setCursor(0, 0); lcd.print("Inserted Item's bin");
              lcd.setCursor(0, 1); lcd.print("is full");
              delay(500); open_door(); delay(3000);
              close_door(); step_home(); lcd.clear();
            }

          }
          else if (rx == '4') { //Material is Not Determined
            lcd.clear();
            lcd.setCursor(0, 0); lcd.print("Inserted Item is");
            lcd.setCursor(0, 1); lcd.print("Undetermined and");
            lcd.setCursor(0, 1); lcd.print("Rejected");
            delay(500); open_door(); delay(2000);
            close_door(); step_home(); lcd.clear();
          }

        }
      }

    }
    else if (rx == 'P') { // pet bottle reset counter
      petCtr = 0;
    }
    else if (rx == 'C') { // alum can reset counter
      canCtr = 0;
    }
    else if (rx == 'G') { // glass bottle reset counter
      glassCtr = 0;
    }
  }

  //  if (Serial.available()) {
  //    char rx = Serial.read();
  //    if (rx == 'N') {
  //      int num = Serial.parseInt();
  //      tens = num / 10;
  //      ones = num % 10;
  //      Serial.print(tens); Serial.print("  "); Serial.println(ones);
  //      choose_segment_ones(ones);
  //      choose_segment_tens(tens);
  //    }
  //  }
}
void tests()
{
  Serial.println("TEST");
  while (1) {

//            Serial.println(digitalRead(lmtHome)); delay(200);

//    for (int num = 0 ; num < 40; num++) {
//      tens = num / 10;
//      ones = num % 10;
//      Serial.print(tens); Serial.print("  "); Serial.println(ones);
//      choose_segment_ones(ones);
//      choose_segment_tens(tens);delay(1000);
//    }

    if (Serial.available()) {
      char rx = Serial.read();
      Serial.println(rx);
      if (rx == 'q') {
        int num = Serial.parseInt();
        goto_bin(num);
      }
      if (rx == 'w') {
        step_home();
      }
      if (rx == 'e') {
        int num = Serial.parseInt();
        srvDoor.write(num);
      }
      if (rx == 'a') {
        open_door();
      }
      if (rx == 's') {
        close_door();
      }
      if (rx == 'n') {
        int num = Serial.parseInt();
        tens = num / 10;
        ones = num % 10;
        Serial.print(tens); Serial.print("  "); Serial.println(ones);
        choose_segment_ones(ones);
        choose_segment_tens(tens);
      }
      Serial.println("OK");
    }
  }
}
void display_count(int num)
{
  tens = num / 10;
  ones = num % 10;
  Serial.print(tens); Serial.print("  "); Serial.println(ones);
  choose_segment_ones(ones);
  choose_segment_tens(tens);
}
void choose_segment_tens(int num)
{
  digitalWrite(tensE, 0);
  switch (num) {
    case 0:
      on_tens(0, 0);
      break;
    case 1:
      on_tens(0, 1);
      break;
    case 2:
      digitalWrite(tensE, 1);
      on_tens(1, 0);
      break;
    case 3:
      on_tens(1, 1);
      break;
  }
}
void on_tens(int a, int b)
{
  digitalWrite(tens1, a); digitalWrite(tens2, b);
}
void choose_segment_ones(int num)
{
  digitalWrite(onesC, 1);
  switch (num) {
    case 0:
      on_ones(0, 0, 0, 0);
      break;
    case 1:
      on_ones(0, 0, 0, 1);
      break;
    case 2:
      digitalWrite(onesC, 0);
      on_ones(0, 0, 1, 0);
      break;
    case 3:
      on_ones(0, 0, 1, 1);
      break;
    case 4:
      on_ones(0, 1, 0, 0);
      break;
    case 5:
      on_ones(0, 1, 0, 1);
      break;
    case 6:
      on_ones(0, 1, 1, 0);
      break;
    case 7:
      on_ones(0, 1, 1, 1);
      break;
    case 8:
      on_ones(1, 0, 0, 0);
      break;
    case 9:
      on_ones(1, 0, 0, 1);
      break;
  }
}
void on_ones(int a, int b, int c, int d)
{
  digitalWrite(ones1, a); digitalWrite(ones2, b);
  digitalWrite(ones3, c); digitalWrite(ones4, d);
}
