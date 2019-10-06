/*
  Pedal Switcher by Kyle Thompson
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <SPI.h>
#include "Presets.h"
#include <AudioTuner.h>
#include <Audio.h>

//***************LCD Display*****************
U8G2_ST7565_NHD_C12864_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 10, /* dc=*/ 24, /* reset=*/ 25);
#define backlightRed 8
#define backlightGreen 7
#define backlightBlue 6

//***************Footswitches*****************
#include <ClickButton.h>
#define numPresetSwitches 6
#define numBankSwitches 2

const int presetSwitchesPin[numPresetSwitches] = { 38, 37, 36, 35, 34, 33};
const int bankSwitchesPin[numBankSwitches] = { 30, 29 };
ClickButton presetButtons[numPresetSwitches] = {
  ClickButton (presetSwitchesPin[0], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[1], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[2], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[3], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[4], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[5], HIGH, CLICKBTN_PULLUP)
};

ClickButton bankButtons[numBankSwitches] = {
  ClickButton(bankSwitchesPin[0], HIGH, CLICKBTN_PULLUP),
  ClickButton(bankSwitchesPin[1], HIGH, CLICKBTN_PULLUP)
};

//***************Neopixel LEDs*****************
#include <Adafruit_NeoPixel.h>
#define numLEDs 1
#define LEDPin 39
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, LEDPin, NEO_GRB + NEO_KHZ800);
const int presetLEDs[numPresetSwitches] = { 9, 7, 5, 3, 2, 1};

uint32_t color = 0x222222;      // 'On' color (starts red)

//***************SD CARD*****************
char* filename = "Presets.txt";
File myFile;
int CS = 10;

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
int vactrolV1 = 22;//input in series (should be 1k for unity gain)
int vactrolV1SWfix = 17;//needed because 17 is not a PWM pin is softwired to 22
int vactrolV2 = 23;//Output to Ground

//**********MIDI***********

//**********Tuner********
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
#include "coeff.h"
const float notes[numNotes] = {cNote, csNote, dNote, ebNote, eNote, fNote, fsNote, gNote, gsNote, aNote, bbNote, bNote};
const char* notesNames[numNotes] = {"C", "C#", "D", "Eb", "E", "F", "F#", "G", "G#", "A", "Bb", "B"};
//AudioTuner                tuner;
//AudioInputAnalog          adc1;
//AudioConnection patchCord0(adc1, 0, tuner, 0);

void setup()
{
  //  Serial.begin(115200);
  //***************LCD Display*****************
  u8g2.begin();
  u8g2.setFlipMode(1);
  u8g2.setContrast(255);
  preset1();
  delay(100);
  //***************LEDs*****************
  Serial3.begin(115200);
  pinMode(15, INPUT);
  delay(100);
  //***************Footswitches*****************
  setupSwitches();
  delay(100);
  //***************SD CARD*****************
  //  if (!SD.begin(CS)) {
  //    return;
  //  }

  //***************Shift Registers*****************
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  delay(100);
  //  allOff();

  //  matrix.print(0x0FF, HEX);
  //  matrix.writeDigitNum(1, 0);
  //  matrix.writeDisplay();

  //**********Relays************
  pinMode(tunerEnable , OUTPUT);
  digitalWrite(tunerEnable, LOW);
  pinMode(bufferBypass, OUTPUT);
  digitalWrite(bufferBypass, LOW);

  //**********Vactrols**********
  pinMode(vactrolV1SWfix, INPUT_PULLUP);
  pinMode(vactrolV1, OUTPUT);
  digitalWrite(vactrolV1, 0);
  pinMode(vactrolV2, OUTPUT);
  digitalWrite(vactrolV2, LOW);
  analogWriteFrequency(vactrolV1, 375000);
  analogWrite(vactrolV1, 57);//unity gain 57
  delay(100);

  //**********MIDI***********

  //**********Tuner***********
  //AudioMemory(30);
  /*
      Initialize the yin algorithm's absolute
      threshold, this is good number.
  */
  //use tuner.disable() to stop, call this to start back up again
  //tuner.begin(.15, fir_22059_HZ_coefficients, sizeof(fir_22059_HZ_coefficients), 2);
  //bypassOut();//TODO for testing without switches, should be allOff()
  //
  bypassOutwGND();
  LEDsOff();
//  delay(10);
//  setLED(6, 0, 0, 127);
  while(1) {
    testLEDs();
  }
//  delay(1000);
//  for (int i = 0; i < 9; i++) {
//  setLED(i, 255, 255, 255);
//  delay(100);
//  }
  currentPedals[0] = 5;
  currentPedals[1] = 7;
  setMux();

  
  while(1);
  //  allOff();
  //  while(1);
  //  while(1) {
  //    for (int i = 0; i < 14; i++) {
  //      currentPedals[i] = 0;
  //    }
  //    setMux();
  //    delay(1000);
  //    for (int i = 1; i < 13; i++) {
  //      currentPedals[i-1] = i;
  //    }
  //    setMux();
  //    delay(1000);
  //  }

  //  while (1) {
  //    if (Serial.available() > 0) {
  //      int val = Serial.read() - '0';
  //      for (int i = 0; i < 14; i++) {
  //        currentPedals[i] = presets[val][i];
  //      }
  //      setMux();
  //      Serial.print("Preset: "); Serial.println(val);
  //    }
  //    tunerTest();
}

void loop()
{
  checkSwitches();

  if (!presetMode) {
    /*
      //single click TODO
      if (presetChanged > 0) {
    	int pedalOffLocation;
    	currentPreset = presetChanged;
    	bypass = false;
    	pedalOff = false;
    	//check if pedal is being turned on or off
    	for (int j = 0; j < numPedals; j++) {
    		if (currentPedals[j] == presetChanged) {
    			pedalOff = true;
    			pedalOffLocation = j;
    			break;
    		}
    		else {
    			pedalOff = false;
    		}
    	}
    	if (pedalOff) {
    		//only one pedal is on it is turned off
    		//        Serial.println("INFO");
    		//        Serial.println(currentPedals[0]);
    		//        Serial.println(presetChanged);
    		//        Serial.println(numPedals);
    		if (numPedals == 1 && currentPedals[0] == presetChanged) {
    			currentPedals[0] = 0;
    			currentPreset = 0;
    			numPedals--;
    			printPedals();
    			// matrix.print(currentPreset, DEC);
    			matrix.print(8888, DEC);
    			matrix.writeDisplay();
    		}
    		//remove pedal and compress array
    		else {
    			for (int j = pedalOffLocation; j < 13; j++) {
    				currentPedals[j] = currentPedals[j + 1];
    			}
    			currentPedals[13] = 0;
    			numPedals--;
    			printPedals();
    		}
    	}
    	//add new pedal to next in chain
    	else {
    		//matrix.print(presetChanged, DEC);
    		matrix.print(8888, DEC);
    		matrix.writeDisplay();
    		currentPedals[numPedals] = presetChanged;
    		numPedals++;
    		printPedals();
    	}
      }
      //long click
      //        if (presetSwitches[i].clicks == -1) {
      //
      //          //check if pedal is being turned on or off
      //          if (!bypass) {
      //            bypass = true;
      //            for (int j = 0; j < numPresetSwitches; j++) {
      //                strip.setPixelColor(presetLEDs[j], strip.Color(0,0,0));
      //            }
      //            allOff;
      //          }
      //          else {
      //            bypass = false;
      //            for (int j = 0; j < numPresetSwitches; j++) {
      //              if (currentPedals[j] > 0) {
      //                strip.setPixelColor(presetLEDs[j], strip.Color(pedalColors[j][0],pedalColors[j][1],pedalColors[j][2]));
      //              }
      //            }
      //            setMux();
      //          }
      //        }
    */
  }
  else {
    //single click
    if (presetChanged > 0) {
      updatePedals();
    }
  }

  if (presetChanged > 0) {
    changePreset();
  }
}

//*************DEBUG STUFF**********************
void printPedals() {
  Serial.print("numPedals: "); Serial.println(numPedals);
  Serial.print("currentPedals: ");
  for (int i = 0; i < 14; i++) {
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
