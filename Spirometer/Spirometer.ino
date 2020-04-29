//Final Project: Spirometer
//Aimee McVey and Adrienne Hawkes

#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();
#define WHITE 0x7
#include <EEPROM.h>

//initialize variables
int pin = A0;
float flowrate = 0;
int Vo;
int var1[300];
int var2[300];
float Kp = 0;
float Km = 0;
long offset = 0;
int state;
int buttonstate;
int substate = 0;
float IC = 0;
float EC = 0;
float FRmax = 0;
float FRmin = 0;
float Vt = 0;
int eeAddress = 16;
float Kp1;
float Km1;
float Kp2;
float Km2;
float Kp3;
float Km3;

void setup() {
  lcd.begin(16, 2); //begin LCD
  lcd.setBacklight(WHITE);
  lcd.clear();
  pinMode(pin, INPUT); //take in A0 - analog input
  Serial.begin(9600);
  state = 0;
  buttonstate = 0;
}

void loop() {

  if (state == 0) {     //select calibration
    manualcal();
  }

  if (state == 1) {
    offsetcalc();           //offset calculation
    if (substate == 1)   {  //allows for offset checking
      calibrate();          //manual calibration
    }
  }

  if (state == 2) {
    offsetcalc();   //offset calculation if auto calibration
  }

  if (state == 3) {
    disp();         //continually displays flow until started
  }

  if (state == 4) {
    test1();        //normal breath - Vt
  }

  if (state == 5) {
    test2();        //full capacity breath - IC, EC
  }

  if (state == 6) {
    test3();        //forceful breath - peak flow
  }

  if (state == 7) {
    results();      //display results
  }

  if (state == 8) {
    repeat();       //select restart, retest, results
  }
}

void manualcal() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Calibrate??");
  lcd.setCursor(0, 1);
  lcd.print("L: no  R: yes");
  uint8_t buttons = lcd.readButtons();
  uint8_t i = 0;
  buttons = lcd.readButtons();
  int buttonstate2 = 0;
  delay(200);

  if (buttons) {
    if (buttons & BUTTON_RIGHT) { //yes - manual calibration
      state = 1;
    }
    if (buttons & BUTTON_LEFT) { //no - auto calibration
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Pick pneum");   //select pneumotach used
      lcd.setCursor(0, 1);
      lcd.print("L:1  R:2  U:User");
      delay(200);
      while (buttonstate2 == 0) {
        buttons = lcd.readButtons();
        if (buttons) {
          if (buttons & BUTTON_LEFT) { //pneum 1
            delay(100);
            //retrieve stored values for E1
            EEPROM.get(0, Kp1);
            Kp = Kp1 * 4;
            EEPROM.get(4, Km1);
            Km = Km1 * 4;
            buttonstate2 = 1;
          }

          if (buttons & BUTTON_RIGHT) { //pneum 2
            delay(100);
            //retrieve stored values for E2
            EEPROM.get(8, Kp2);
            Kp = Kp2 * 4;
            EEPROM.get(12, Km2);
            Km = Km2 * 4;
            buttonstate2 = 1;
          }

          if (buttons & BUTTON_UP) { //user created vals for any pneum
            delay(100);
            //retrieve stored values for E3
            EEPROM.get(16, Kp3);
            Kp = Kp3 * 4;
            EEPROM.get(20, Km3);
            Km = Km3 * 4;
            Serial.println("Kp");
            Serial.println(Kp * 1000);
            Serial.println("Km");
            Serial.println(Km * 1000);
            buttonstate2 = 1;
          }
        }
        state = 2;
      }
    }
  }
}

void offsetcalc() {
  substate = 0;
  //take average of 20 readings for offset
  for (int n = 0; n <= 19; n++) {
    offset += analogRead(pin); //read voltage out from circuit
    delay(20);
  }
  offset = offset / 20;
  Serial.println("offset");
  Serial.println(offset);
  if (offset > 530 || offset < 490) { //if offset bad, redo
    lcd.clear();
    lcd.print("offset failure");
    state = 0;
    lcd.setCursor(0, 1);
    lcd.print("restarting");
    delay(2000);
    return state;
  }

  if (state == 1) { //if manual cal, proceed to cal
    substate = 1;
  }
  if (state == 2) { //if auto cal, proceed to test
    state = 3;
  }
  delay(1000);
}

void calibrate() {
  uint8_t buttons = lcd.readButtons();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Push up for K+");
  lcd.setCursor(0, 1);
  lcd.print("when ready");
  delay(1000);
  long s1 = 0;
  long s2 = 0;
  uint8_t i = 0; //initialize buttons

  while (buttonstate == 0) {
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_UP) {
        delay(100);
        buttonstate = 1;
      }
    }
  }

  if (buttonstate == 1) {
    s1 = 0;
    lcd.clear();
    lcd.print("K+: pull 3s now");
    for (int i = 0; i <= 299; i++) {
      var1[i] = analogRead(pin); //read and store in array
      var1[i] = var1[i] - offset;
      s1 += var1[i];
      delay(10);
    }
    lcd.clear();
    lcd.print("stop pull"); // tell user to stop calibrating
    delay(1000);
    Kp = 3 / (s1 * 0.01); //calc calibration constant
    lcd.clear();
    lcd.print("K+ = ");
    lcd.print(Kp * 1000); //mult by 1000 to make it more visible
    delay(2000);
    buttonstate = 0;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Push down for K-");
  lcd.setCursor(0, 1);
  lcd.print("when ready");
  delay(1000);

  while (buttonstate == 0) {
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_DOWN) {
        delay(100);
        buttonstate = 2;
      }
    }
  }

  if (buttonstate == 2) {
    s2 = 0;
    lcd.clear();
    lcd.print("K-: push 3s now");
    for (int i = 0; i <= 299; i++) {
      var2[i] = analogRead(pin); //read and store in array
      var2[i] = var2[i] - offset;
      s2 += var2[i];
      delay(10);
    }
    lcd.clear();
    lcd.print("stop push"); // tell user to stop calibrating
    delay(1000);
    Km = 3 / (s2 * 0.01); //L/(bit*s)
    lcd.clear();
    lcd.print("K- = ");
    lcd.print(Km * 1000);
    delay(1000);
    buttonstate = 0;
  }

  //Store user-calibrated vals in EEPROM
  Kp3 = Kp / 4;
  EEPROM.put(eeAddress, Kp3);
  Serial.println(Kp3);
  Serial.println(eeAddress);
  eeAddress += sizeof(Kp3);
  Km3 = Km / 4;
  EEPROM.put(eeAddress, Km3);
  Serial.println(Km3);
  Serial.println(eeAddress);
  eeAddress += sizeof(Km3);

  int buttonstate2 = 0;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("If |vals| not");
  lcd.setCursor(0, 1);
  lcd.print("btw 10-20, redo"); //instruct user to retest if bad vals
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Recalibrate?");
  lcd.setCursor(0, 1);
  lcd.print("L: test  R: yes");
  delay(1000);

  while (buttonstate2 == 0) {
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_RIGHT) { //restart calibration
        delay(100);
        state = 1;
        buttonstate2 = 1;
      }
      if (buttons & BUTTON_LEFT) { //proceed to testing
        delay(100);
        state = 3;
        buttonstate2 = 1;
      }
    }
  }
  return state;
}

void disp() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready to test");
  delay(1000);
  uint8_t buttons = lcd.readButtons();
  uint8_t i = 0;
  while (state == 3) {
    buttons = lcd.readButtons();
    flow();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Rate:");
    lcd.print(flowrate);  //continuously display airflow on LCD (L/min)
    lcd.print(" L/min");
    lcd.setCursor(0, 1);
    lcd.print("L: Start Testing");
    delay(200);
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_LEFT) {
        delay(100);
        state = 4; //starts testing
      }
    }
  }
}

void test1() {
  //Tidal Volume (Vt): amt of air ventilated in a normal quiet breath
  lcd.clear();
  lcd.print("Begin testing");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Take 1 normal");
  lcd.setCursor(0, 1);
  lcd.print("breath in 5s");
  delay(250);
  float vol = 0;
  float flowratesum = 0;
  for (int i = 0; i < 499; i++) {
    delay(10);
    Vo = analogRead(pin) - offset; //read voltage in bits
    if (Vo > 0) {
      flowrate = Kp * Vo * 60; //L/(bit*s)*bits*60 s/min
      flowratesum += flowrate;
    }
  }
  Vt = flowratesum * 0.01 / 60; //convert flow (L/min) to vol (L)
  state = 5;
}

void test2() {
  //Inspiratory Capacity (IC): max amount of air inspired after
  //normal tidal expiration
  //Expiratory Capacity (EC): max amount of air expired after
  lcd.clear();
  lcd.print("2nd test");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 full capacity");
  lcd.setCursor(0, 1);
  lcd.print("breath in 5s");
  delay(250);
  float vol = 0;
  float volmaxinhale = 0;
  float volmaxexhale = 0;
  float flowratesuminhale = 0;
  float flowratesumexhale = 0;
  for (int i = 0; i < 499; i++) {
    delay(10);
    Vo = analogRead(pin) - offset; //read voltage in bits
    if (Vo > 0) { //inhale
      flowrate = Kp * Vo * 60;
      flowratesuminhale += flowrate;
    }
    else if (Vo < 0) { //exhale
      flowrate = -Km * Vo * 60;
      flowratesumexhale += flowrate;
    }
  }
  IC = flowratesuminhale * 0.01 / 60; //convert flow (L/min) to vol (L)
  EC = flowratesumexhale * 0.01 / 60;
  state = 6;
}

void test3() {
  //Peak flow
  lcd.clear();
  lcd.print("3rd test");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1 forceful");
  lcd.setCursor(0, 1);
  lcd.print("breath in 5s");
  delay(250);
  float maxflowrate = 0;
  float minflowrate = 0;
  float flowrateP = 0;
  float flowrateM = 0;
  for (int i = 0; i < 499; i++) {
    delay(10);
    Vo = analogRead(pin) - offset; //read voltage in bits
    //differentiate btw inhale & exhale
    if (Vo > 0) { //inhale
      flowrateP = Kp * Vo * 60;
    }
    else if (Vo < 0) { //exhale
      flowrateM = -Km * Vo * 60;
    }
    if (flowrateP > maxflowrate && Vo > 0) { //inhale
      maxflowrate = flowrateP;
    }
    else if (flowrateM < minflowrate && Vo < 0) { //exhale
      minflowrate = flowrateM;
    }
  }
  FRmax = maxflowrate;
  FRmin = minflowrate;
  state = 7;
}

void flow() {
  //Factor in offset and calibration constant to give airflow (L/min)
  Vo = analogRead(pin) - offset; //read voltage in bits
  if (Vo > 0) {   //if Vo > 0, inhale, use K+
    flowrate = Kp * Vo * 60; //L/(bit*s)*bits*60 s/min
  }
  else if (Vo < 0) {  //if Vo < 0, use K-
    flowrate = -Km * Vo * 60;
  }
}

void results() {
  lcd.clear();
  lcd.print("Results");
  delay(2000);
  lcd.clear();
  lcd.print("Tidal Volume:");
  lcd.setCursor(0, 1);
  lcd.print(Vt);
  lcd.print(" L");
  delay(2000);
  lcd.clear();
  lcd.print("Inhale Cap:");
  lcd.setCursor(0, 1);
  lcd.print(IC);
  lcd.print(" L");
  delay(2000);
  lcd.clear();
  lcd.print("Exhale Cap:");
  lcd.setCursor(0, 1);
  lcd.print(EC);
  lcd.print(" L");
  delay(2000);
  lcd.clear();
  lcd.print("Inh. Peak Flow:");
  lcd.setCursor(0, 1);
  lcd.print(FRmax);
  lcd.print(" L/min");
  delay(2000);
  lcd.clear();
  lcd.print("Ex. Peak Flow:");
  lcd.setCursor(0, 1);
  lcd.print(FRmin);
  lcd.print(" L/min");
  delay(2000);
  state = 8;
}

void repeat() {
  uint8_t buttons = lcd.readButtons();
  lcd.clear();
  lcd.print("Repeat? ");
  delay(1000);
  lcd.clear();
  lcd.print("Up:Reset");
  lcd.setCursor(0, 1);
  lcd.print("L:Results R:Test");
  delay(1000);
  uint8_t i = 0; //initialize buttons
  
  while (state == 8) {
    buttons = lcd.readButtons();
    if (buttons) {
      if (buttons & BUTTON_UP) {
        delay(100);
        state = 0; //repeat calibration
      }
      if (buttons & BUTTON_RIGHT) {
        delay(100);
        state = 3; //repeat testing
      }
      if (buttons & BUTTON_LEFT) {
        delay(100);
        state = 7; //repeat results
      }
    }
  }
}
