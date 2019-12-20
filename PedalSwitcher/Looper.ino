void looper() {
  const int maxLoopTime = 60;
  const int numLCDSteps = 118;
  const int firstRecordStepTime = (60 * 1000) / numLCDSteps;
  int recordStepTime;
  int recordStep = 0;
  int recordStart;
  int recordTime;
  int prevTime = 0;
  int debounceWait = 200;//ms
  int debounceStart = 0;
  looperLCD(1, 0);
  while (1) {
    bankButtons[0].Update();
    if (bankButtons[0].clicks == -1) {
      preset(currentBank, currentPreset, presetNames[currentPreset], false);
      break;
    }
    if (digitalReadFast(presetSwitchesPin[0]) == LOW && millis() > debounceStart + debounceWait) {
      //START RECORDING
      debounceStart = millis();
      if (looperMode == 1) {
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        looperMode = 2;
        prevTime = millis();
        recordStart = prevTime;
      }
      //STOP RECORDING/START PLAYBACK
      else if (looperMode == 2) {
        looperMode = 3;
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        recordTime = millis() - recordStart;
        Serial.print("RecordTime: "); Serial.println(recordTime);
        recordStepTime = recordTime*0.918 / numLCDSteps;
        recordStep = 0;
        prevTime = millis();
      }
      //OVERDUB
      else if (looperMode == 3) {
        looperMode = 4;
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        looperLCD(1, 0);
        recordStep = 0;
        prevTime = millis();
      }
      else if (looperMode == 4) {
        looperMode = 3;
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  88);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        looperLCD(1, 0);
        recordStep = 0;
        prevTime = millis();
      }
    }
    if (digitalReadFast(presetSwitchesPin[1]) == LOW && millis() > debounceStart + debounceWait) {
      //STOP
      debounceStart = millis();
      if (looperMode == 3) {
        looperMode = 4;
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        looperLCD(0, 0);
        recordStep = 0;
        prevTime = millis();
      }
      //ERASE
      else if (looperMode == 4) {
        looperMode = 1;
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   1);
        midiCC(176, 99, 125);
        midiCC(176, 98,  89);
        midiCC(176,  6,   0);
        midiCC(176, 38,   0);
        looperLCD(1, 0);
      }
      delay(200);
    }
    //first record
    if (looperMode == 2) {
      if (millis() > prevTime + firstRecordStepTime) {
        looperLCD(1, recordStep);
        recordStep++;
        if (recordStep > numLCDSteps) {
          looperMode = 3;
          recordTime = 60000;
          recordStep = 0;
          prevTime = millis();
          //send stop command
        }
        prevTime = millis();
      }
    }
    //playback
    if (looperMode == 3) {
      if (millis() > prevTime + recordStepTime) {
        looperLCD(1, recordStep);
        recordStep++;
        if (recordStep > 118) {
          recordStep = 0;
        }
        prevTime = millis();
      }
    }
  }
}
