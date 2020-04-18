#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

//Input & Button Logic
const int numOfInputs = 5;
const int inputPins[numOfInputs] = {4, 5, 6, 7, 8};   //menu, incr, decr, set, play_pause
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {1, 1, 1, 1, 1};
bool inputFlags[numOfInputs] = {1, 1, 1, 1, 1};
long lastDebounceTime[numOfInputs] = {0, 0, 0, 0, 0};
int debounceDelay = 10;

//LCD Menu Logic
const int numOfScreens = 5;
int currentScreen = 0;
String screens[numOfScreens][2] = {{"Select Mode", " * 0=IV & 1=RPM" }, {"SET INS speed", "/unit"}, {"SET IE ratio", "Unit"}, {"SET RPM", "Unit"}, {"SET Volume", "Unit"} };

float parameters[numOfScreens] = { 0, 1.0, 2, 8, 0};

//global variable for set values
float parameters_set[numOfScreens] = { 0, 1.0, 2, 8, 0}; //mode(0~1) ins_speed(1.0-11.0), ratio(2-5), rpm(8-20), volume(0-100)%

//relevant menu items in accordance with the respective modes
int IV_menus [] =  {true, true, true, false, true };    //mood_select(✔), Ins_speed(✔), IE_ratio(✔), RPM(✖), Volume(✔)
int RPM_menus [] = {true, false, false, true, true };   //mood_select(✔), Ins_speed(✖), IE_ratio(✖), RPM(✔), Volume(✔)

bool playPauseFlag = true;

///////////////////////BT Codes//////////
#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10);

//Parameters to be sent
#define start   playPauseFlag
#define vol     parameters_set[4]     //Tidal volume. Range 0-600
#define rpm     parameters_set[3]     //Breathing cycle. Range 8-20
#define iSpeed  parameters_set[1]     //Inhalation Speed.. Range 0.0-11.0
#define v       parameters_set[2]     //IV Ratio. Range 2-10

String outString = "";                //Data string that goes to slave module

///////////////////BT///////////////////



void setup() {
  BT_setup();

  for (int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT_PULLUP);
  }
  lcd.init();   //if u get error, change to lcd.begin()
  lcd.backlight();

  lcd.clear();
  lcd.print("Project");
  lcd.setCursor(0, 1);
  lcd.print(" Ventilator");
  delay(1000);
  printScreen();
}

void loop() {
  ///////////////////BT///////////////////
  BT_loop();
  ///////////////////BT///////////////////

  setInputFlags();
  resolveInputFlags();

}

void setInputFlags() {
  for (int i = 0; i < numOfInputs; i++) {
    int reading = digitalRead(inputPins[i]);
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
        inputState[i] = reading;
        if (inputState[i] == LOW) {
          inputFlags[i] = LOW;
        }
      }
    }
    lastInputState[i] = reading;
  }
}

void resolveInputFlags() {
  for (int i = 0; i < numOfInputs; i++) {
    if (inputFlags[i] == LOW) {
      inputAction(i);
      inputFlags[i] = HIGH;
      printScreen();
    }
  }
}
//button actions
void inputAction(int button) {
  //left button for screen change
  if (button == 0) {
    if (currentScreen >= numOfScreens - 1) {
      currentScreen = 0;
    } else {
      currentScreen = get_nextMenu(++currentScreen);
    }

    resetScreen();        //reset screen parameters upon page change
  }
  //up button for increment
  else if (button == 1) {
    parameterChange(0);
  }
  //down button for decrement
  else if (button == 2) {
    parameterChange(1);
  }
  //right button for value set
  else if (button == 3) {
    parameterSet();
  }
  //middle button for play pause
  else if (button == 4) {
    play_pause();
  }

}

void parameterChange(int key) {
  float increment, TH1, TH2;
  if (currentScreen == 0 ) {
    //mode
    increment = 1;
    TH1 = 1;
    TH2 = 0;
  }
  if (currentScreen == 1) {
    //inspiration
    increment = 0.5;
    TH1 = 11;
    TH2 = 1;
  }
  if (currentScreen == 2 ) {
    //IE ratio
    increment = 1;
    TH1 = 5;
    TH2 = 2;
  }
  if (currentScreen == 3) {
    //rpm
    increment = 1;
    TH1 = 20;
    TH2 = 8;
  }
  if (currentScreen == 4) {
    //Tidal Volume
    increment = 10;
    TH1 = 100;
    TH2 = 0;
  }
  if (key == 0 && parameters[currentScreen] < TH1) {
    parameters[currentScreen] += increment;
  }
  else if (key == 1 && parameters[currentScreen] > TH2) {
    parameters[currentScreen] -= increment;
  }
}
//set the value on screen to global variables
void parameterSet() {
  parameters_set[currentScreen] = parameters[currentScreen];
  display_confirm();

}
//show a confirmation dialogue
void display_confirm() {
  lcd.clear();
  lcd.print("Parame Set OK");
  lcd.setCursor(0, 1);
  lcd.print(parameters_set[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][0]);
  delay(1000);
}
//reset screen
void resetScreen() {
  for (int i = 0; i < numOfScreens; i++) {
    parameters[i] = parameters_set[i];

  }
}

//display play pause action
void display_play() {
  lcd.clear();
  lcd.print("Starting Vent");
  lcd.setCursor(0, 1);
  lcd.print("BPM:");
  lcd.print(parameters_set[0]);
  lcd.print(" VOL:");
  lcd.print(parameters_set[1]);
}
void display_pause() {
  lcd.clear();
  lcd.print("Stopping Vent");
  lcd.setCursor(0, 1);
  lcd.print("Last Mod:");
  lcd.print(parameters_set[0]);
  lcd.print(",V:");
  lcd.print(parameters_set[1]);
}

void printScreen() {
  lcd.clear();
  lcd.print(screens[currentScreen][0]);
  lcd.setCursor(0, 1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
}
