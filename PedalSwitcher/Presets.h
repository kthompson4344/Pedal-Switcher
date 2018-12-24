#pragma once
const int numPresets = 99;
const int numColumns = 14;
int currentPreset = 0;
int presetChanged = 0;
int bankChanged = 0;
int currentBank = 0;
bool bypass = true;
int bank = 1;
bool mute = true;
//TODO this will be replaced with SD card
//keep 0-14 for manual eval
int presets[numPresets][numColumns] = {
  //if in presets 1-14, pedals are able to be manually turned on in the order they are pressed
	//last column is pedal color
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {12, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

int currentPedals[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int pedalColors[14][3] {
  //g,   r,    b
  {0x00, 0x55, 0x00},
  {0x00, 0x0, 0x127},
  {0x20, 0x76, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x6b, 0x43},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
  {0x00, 0x55, 0x00},
};

int pedalDataValues[15] = {31, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};

