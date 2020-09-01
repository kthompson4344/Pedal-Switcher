#pragma once
const int numPresets = 99;
const int maxPedals = 13;
int currentPreset = 0;
int presetChanged = 0;
int bankChanged = 0;
int lastBank = 1;
int currentBank = 0;
bool bypass = true;
int bank = 1;
bool mute = true;
bool longClick = false;
bool program = false;
bool save = false;
int looperMode = 1;
int programBank = 50;
bool bankChange = false;
int presetToProgram = 0;
const int maxPresetName = 12;
int totalPresets;
int looperTimer;
//TODO this will be replaced with SD card
//keep 0-14 for manual eval
int presets[numPresets][maxPedals]= {0};

int currentPedals[maxPedals] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

int pedalColors[numPresets][3];

char presetNames[numPresets][maxPresetName + 1];//need +1 for /n character

int pedalDataValues[15] = {1, 3, 5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29};

int MIDIPreset[numPresets] = {1, 2, 3, 4, 5, 2, 6, 7, 8, 9, 10, 7, 11, 12, 13, 14, 15, 12, 16, 17, 18, 19, 20, 17, 21, 22, 23, 24, 25, 22, 26, 27, 28, 29, 30, 27, 31, 32, 33, 34, 35, 32};
