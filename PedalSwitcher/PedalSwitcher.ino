/*
  Pedal Switcher by Kyle Thompson
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <SD.h>
#include <SPI.h>
#include "Presets.h"
//#include <AudioTuner.h>
//#include <Audio.h>

//***************LCD Display*****************
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 24, /* reset=*/ 25);
#define backlightRed 5
#define backlightGreen 7
#define backlightBlue 6

//***************Footswitches*****************
#include <ClickButton.h>
#define numPresetSwitches 6
#define numBankSwitches 2

const int presetSwitchesPin[numPresetSwitches] = { 38, 37, 36, 35, 34, 33};
const int bankSwitchesPin[numBankSwitches] = { 30, 29 };
ClickButton presetButtons[numPresetSwitches] = {
  ClickButton (presetSwitchesPin[0], LOW, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[1], LOW, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[2], LOW, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[3], LOW, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[4], LOW, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[5], LOW, CLICKBTN_PULLUP)
};

ClickButton bankButtons[numBankSwitches] = {
  ClickButton(bankSwitchesPin[0], LOW, CLICKBTN_PULLUP),
  ClickButton(bankSwitchesPin[1], LOW, CLICKBTN_PULLUP)
};

//***************SD CARD*****************
const int chipSelect = BUILTIN_SDCARD;
char* filename = "Presets.txt";
File myFile;

//***************Shift Registers*****************
//Pin connected to ST_CP of 74HC595
int latchPin = 28;
//Pin connected to SH_CP of 74HC595
int clockPin = 27;
////Pin connected to DS of 74HC595
int dataPin = 26;
byte data1;
byte data2;
byte dataArray[10];
bool presetMode = true;
bool pedalOff;
bool presetsOff = false;
int numPedals = 0;
int twoPresetsPrev = 0;

//**********Relays************
int tunerEnable = 31;
int bufferBypass = 32;

//**********Vactrols**********
int vactrolV1 = 22;//input in series (should be 1k ohm for unity gain)
int vactrolV1SWfix = 17;//needed because 17 is not a PWM pin is softwired to 22
int vactrolV2 = 23;//Output to Ground
int unityGain = 1075;//1k ohm vactrol resistance

//**********MIDI***********

//**********Tuner********
#include <MIDI.h>
#include <Kemper.h>
#include <KemperRemote.h>
#include <Kemper_Namespace.h>
USING_NAMESPACE_MIDI
MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, midiOutput1);
USING_NAMESPACE_KEMPER
Kemper kemper;
int tunerADC = A2;
#define cNote 32.70
#define csNote 34.65
#define dNote 36.71
#define ebNote 38.89
#define eNote 41.20
#define fNote 43.65
#define fsNote 46.25
#define gNote 49.00
#define gsNote 51.91
#define aNote 55.00
#define bbNote 58.27
#define bNote 61.74
#define numNotes 12
//#include "coeff.h"
const byte numChars = 13;
int receivedChars[numChars];
boolean newData = false;
int note;
int tune;
const float notes[numNotes] = {cNote, csNote, dNote, ebNote, eNote, fNote, fsNote, gNote, gsNote, aNote, bbNote, bNote};
const char* notesNames[numNotes] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B"};
//AudioTuner                tuner;
//AudioInputAnalog          adc1;
//AudioConnection patchCord0(adc1, 0, tuner, 0);

void setup()
{
  Serial.begin(115200);

  //***************LEDs*****************
  Serial3.begin(115200);
  pinMode(15, INPUT);
  LEDsOff();
  delay(100);

  //***************LCD Display*****************
  pinMode(backlightRed, OUTPUT);
  digitalWrite(backlightRed, LOW);
  //  analogWriteFrequency(backlightRed, 375000);
  pinMode(backlightBlue, OUTPUT);
  digitalWrite(backlightBlue, LOW);
  //  analogWriteFrequency(backlightBlue, 375000);
  pinMode(backlightGreen, OUTPUT);
  digitalWrite(backlightGreen, LOW);
  //  analogWriteFrequency(backlightGreen, 375000);
  u8g2.begin();
  u8g2.setFlipMode(1);
  u8g2.setContrast(230);
  delay(100);

  //***************Footswitches*****************
  setupSwitches();
  delay(100);

  //***************SD CARD*****************
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    preset(0, 3, "CARD ERROR", 0);
    while (1);
  }
  //  while(1);
  //  dumpFile();
  //  delay(100);
  //  loadSinglePreset(2);
  loadAllPresets();

  //***************Shift Registers*****************
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  delay(100);
  allOff();

  //**********Relays************
  pinMode(tunerEnable , OUTPUT);
  digitalWrite(tunerEnable, LOW);
  pinMode(bufferBypass, OUTPUT);
  digitalWrite(bufferBypass, LOW);//low is with input buffer

  //**********Vactrols**********
  pinMode(vactrolV1SWfix, INPUT);
  pinMode(vactrolV1, OUTPUT);
  pinMode(vactrolV2, OUTPUT);
  digitalWrite(vactrolV2, LOW);
  analogWriteResolution(12);
  analogWriteFrequency(vactrolV1, 375000);//22
  analogWrite(vactrolV1, unityGain);//unity gain 57
  delay(100);

  //**********MIDI***********
  kemper.begin();
  kemper.tunerOn();
  //**********Tuner***********
  //AudioMemory(30);
  /*  Initialize the yin algorithm's absolute
      threshold, this is good number.*/
  //use tuner.disable() to stop, call this to start back up again
  //tuner.begin(.15, fir_22059_HZ_coefficients, sizeof(fir_22059_HZ_coefficients), 2);
  //  delay(1000);
  //  dumpFile();
      setMux();
//      while(true) {
//      for (int i = 0; i < 127; i++) {
//      tunerLCD(notesNames[0], i, true);
//      delay(200);
//      }
//      }
      tuner();
}

void loop()
{
  checkSwitches();
  if (presetChanged > 0) {
    updatePedals();
  }
  if (presetChanged > 0) {
    changePreset();
  }
  if (longClick) {
    program = true;
    programPresets();
  }
}




//*************DEBUG STUFF**********************
void printPedals() {
  Serial.print("numPedals: "); Serial.println(numPedals);
  Serial.print("currentPedals: ");
  for (int i = 0; i < maxPedals; i++) {
    Serial.print(currentPedals[i]);
  }
  Serial.println("");
}

void testLEDs() {
  for (int i = 0; i < 9; i++) {
    setLED(i, 255 , 0, 0);
    delay(200);
  }
  LEDsOff();
  for (int i = 0; i < 9; i++) {
    setLED(i, 0, 255, 0);
    delay(200);
  }
  LEDsOff();
  for (int i = 0; i < 9; i++) {
    setLED(i, 0, 0, 255);
    delay(200);
  }
  LEDsOff();
}

void testSwitches() {
  for (int i = 0; i < 6; i++) {
    if (digitalReadFast(presetSwitchesPin[i]) == LOW) {
      Serial.println(i);
    }
  }
  for (int i = 0; i < 2; i++) {
    if (digitalReadFast(bankSwitchesPin[i]) == LOW) {
      Serial.println(i);
    }
  }
}

void dumpFile() {
  myFile = SD.open(filename);
  // if the file is available, write to it:
  if (myFile) {
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}
