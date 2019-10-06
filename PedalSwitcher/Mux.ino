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

    for (int i = 0; i < 14; i++) {
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
  //TODO free play mode (consider deleting?)
  if (!presetMode) {
//    if (pedalOff) {
//      strip.setPixelColor(presetLEDs[presetChanged - (7 * bank) - 1], strip.Color(0, 0, 0));
//    }
//    else {
//      strip.setPixelColor(presetLEDs[presetChanged - (7 * bank) - 1], strip.Color(pedalColors[presets[presetChanged - 1][14]][0], pedalColors[presets[presetChanged - 1][14]][1], pedalColors[presets[presetChanged - 1][14]][2]));
//    }
//    strip.show();
//    setMux();
    //      delay(100);
    //      while (digitalReadFast(presetSwitchesPin[presetChanged - (7 * bank) - 1]) == HIGH) {
    //        delay(1);
    //      }
    //      delay(20);
  }
  else {
    //single LED on at a time for the selected preset
    if (presetsOff) {
      if (currentPreset != presetChanged) {
//        strip.setPixelColor(presetLEDs[currentPreset - (7 * (currentBank - 1)) - 1], strip.Color(0, 0, 0));
//        strip.setPixelColor(presetLEDs[presetChanged - (7 * (bank - 1)) - 1], strip.Color(pedalColors[presetChanged - 1][0], pedalColors[presetChanged - 1][1], pedalColors[presetChanged - 1][2]));
      }
      else {
//        strip.setPixelColor(presetLEDs[currentPreset - (7 * (bankChanged - 1)) - 1], strip.Color(0, 0, 0));
      }
    }
    else {
//      strip.setPixelColor(presetLEDs[presetChanged - (7 * (bank - 1)) - 1], strip.Color(pedalColors[presetChanged - 1][0], pedalColors[presetChanged - 1][1], pedalColors[presetChanged - 1][2]));
//      matrix.print(presetChanged, DEC);
//      matrix.writeDisplay();
    }
//    strip.show();
    if (presetChanged == currentPreset && presetsOff) {
      Serial.println("YES");
      allOff();
      mute = true;
//      matrix.print(0x0FF, HEX);
//  matrix.writeDigitNum(1, 0);
//  matrix.writeDisplay();
    }
    else {
//      allOff();
      digitalWrite(16, HIGH);
      delay(1000);
      setMux();
      mute = false;
//      matrix.print(presetChanged, DEC);
//      matrix.drawColon(true);
//      matrix.writeDigitRaw(0, letterB);
//      matrix.writeDigitNum(1, bank);
//      matrix.writeDisplay();
    }
    //      delay(100);
  }
  twoPresetsPrev = currentPreset;
  currentPreset = presetChanged;
  currentBank = bankChanged;
  presetChanged = 0;
}
