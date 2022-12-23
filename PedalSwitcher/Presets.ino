 //determine if LEDs will need to be turned on or off, then update current pedal array
void updatePedals() {
  static bool sameSwitch = false;
  if (presetChanged == currentPreset) {
    sameSwitch = true;
    if ((twoPresetsPrev == presetChanged)) {
      if (presetsOff) {
        presetsOff = false;
      }
      else {
        presetsOff = true;
      }
    }
    else {
      presetsOff = true;
    }
  }
  else {
    sameSwitch = false;
    if (presetChanged > 0 && currentPreset > 0) {
      presetsOff = true;
    }
    else {
      presetsOff = false;
    }

    for (int i = 0; i < maxPedals; i++) {
      currentPedals[i] = presets[presetChanged][i];
    }
  }
//  Serial.print("presetChanged: ");
//  Serial.println(presetChanged);
//  Serial.print("currentPreset: ");
//  Serial.println(currentPreset);
  //    Serial.print("presetsOff: ");
  //    Serial.println(presetsOff);
  //  Serial.print("twoPresetsPrev: ");
  //  Serial.println(twoPresetsPrev);
}

//set LEDs and multiplexors
void changePreset() {
  if (!presetMode) {

  }
  else {
    allOff();

    //single LED on at a time for the selected preset
    if (presetsOff) {
      if (bank == 1) {
          setLED(8, 0, 0, 0);
          setLED(7, 0, 0, 0);
          setLED(6, 0, 0, 50);
        }
        else if (bank == 2) {
          setLED(6, 0, 0, 0);
          setLED(8, 0, 0, 0);
          setLED(7, 0, 50, 0);
        }
        else if (bank == 3) {
          setLED(6, 0, 0, 0);
          setLED(7, 0, 0, 0);
          setLED(8, 50, 0, 0);
        }
        else {
          setLED(6, 0, 0, 0);
          setLED(7, 0, 0, 0);
          setLED(8, 0, 0, 0);
        }
        lastBank = bank;
      if (currentPreset != presetChanged) {
        //turn on a preset and turn off another
        setLED(currentPreset - (numPresetSwitches * (currentBank - 1)) - 1, 0, 0, 0);
        setLED(presetChanged - (numPresetSwitches * (bank - 1)) - 1, pedalColors[presetChanged][0], pedalColors[presetChanged][1], pedalColors[presetChanged][2]);
      }
      else {
        setLED(currentPreset - (numPresetSwitches * (bankChanged - 1)) - 1, 0, 0, 0);
      }
    }
    else {
      setLED(presetChanged - (numPresetSwitches * (bank - 1)) - 1, pedalColors[presetChanged][0], pedalColors[presetChanged][1], pedalColors[presetChanged][2]);
    }
    if (presetChanged == currentPreset && presetsOff) {
      //bypass
      allOff();
      mute = true;
//      bypassPreset(currentBank);
    }
    else {
      //any preset
      //allOff();//? TODO
      //send MIDI PC message (set for Kemper performances right now, need to generalize in the future TODO)
      //send program change message on channel 1 (C0)
      midiOutput1.sendProgramChange(MIDIPreset[presetChanged - 1] - 1, 1);
      //midiProg( 0xC0, MIDIPreset[presetChanged - 1] - 1);
      //if 6 is pressed turn morph on, if not turn it off
      Serial.print("PresetChanged: ");
      Serial.println(presetChanged);
      if (presetChanged % 6 == 0) {
        delay(50);
        Serial.println("MORPH ON");
        midiOutput1.sendControlChange(11, 127, 1);
      }
      else {
        midiOutput1.sendControlChange(11, 0, 1);
      }
      muxOn();
      mute = false;
    }
    //      delay(100);
  }
  twoPresetsPrev = currentPreset;
  currentPreset = presetChanged;
  currentBank = bankChanged;
  presetChanged = 0;
  if (!mute) {
    //if not bypassed, change the preset
    preset(currentBank, currentPreset, presetNames[currentPreset], false);
  }
  else {
    tuner();
  }
}
