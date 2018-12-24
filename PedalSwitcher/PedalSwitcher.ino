/*
  Pedal Switcher by Kyle Thompson
*/

#include <SPI.h>
#include <SD_pedalswitcher.h>
#include "Presets.h"
#include "Graphics.h"
//***************7 Segment Display*****************
//***i2c_t3 library added to Adafruit_LEDBackpack
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
Adafruit_7segment matrix = Adafruit_7segment();

//***************Footswitches*****************
#include <ClickButton.h>
#define numAmpSwitches 3
#define numPresetSwitches 7
#define numBankSwitches 2

const int ampSwitchesPin[numAmpSwitches] = { 2, 1, 0 };
const int presetSwitchesPin[numPresetSwitches] = { 23, 22, 21, 38, 37, 36, 35 };
const int bankSwitchesPin[numBankSwitches] = { 31, 32 };
ClickButton presetButtons[numPresetSwitches] = {
  ClickButton (presetSwitchesPin[0], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[1], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[2], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[3], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[4], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[5], HIGH, CLICKBTN_PULLUP),
  ClickButton (presetSwitchesPin[6], HIGH, CLICKBTN_PULLUP)
};

ClickButton bankButtons[numBankSwitches] = {
  ClickButton(bankSwitchesPin[0], HIGH, CLICKBTN_PULLUP),
  ClickButton(bankSwitchesPin[1], HIGH, CLICKBTN_PULLUP)
};

//***************Neopixel LEDs*****************
#include <Adafruit_NeoPixel.h>
#define numLEDs 10
#define LEDPin 33
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(numLEDs, LEDPin, NEO_GRB + NEO_KHZ800);
const int presetLEDs[numPresetSwitches] = { 9, 7, 5, 3, 2, 1, 0 };

//***************SD CARD*****************
char* filename = "Presets.txt";
File myFile;
int CS = 10;

//***************Shift Registers*****************
//Pin connected to ST_CP of 74HC595
int latchPin = 26;
//Pin connected to SH_CP of 74HC595
int clockPin = 27;
////Pin connected to DS of 74HC595
int dataPin = 28;
byte data1;
byte data2;
byte dataArray[10];
bool presetMode = true;
bool pedalOff;
bool presetsOff = false;
int numPedals = 0;
int twoPresetsPrev = 0;
void setup()
{
  Serial.begin(115200);
  //***************7 Segment Display*****************
  matrix.begin(0x70);
  // print a hex number
  matrix.writeDisplay();
  delay(100);
  //***************Neopixel LEDs*****************
  strip.begin();
  delay(100);
  strip.show(); // Initialize all pixels to 'off'
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
  pinMode(16, OUTPUT);
  digitalWrite(16, HIGH);
  delay(100);
  allOff();
  matrix.print(0x0FF, HEX);
  matrix.writeDigitNum(1, 0);
  matrix.writeDisplay();
//while(1);
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

void printPedals() {
  Serial.print("numPedals: "); Serial.println(numPedals);
  Serial.print("currentPedals: ");
  for (int i = 0; i < 14; i++) {
    Serial.print(currentPedals[i]);
  }
  Serial.println("");
}
