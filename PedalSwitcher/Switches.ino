/*void setupSwitches() {

  for (int i = 0; i < numPresetSwitches; i++) {
    pinMode(presetSwitchesPin[i],INPUT_PULLUP);
  }
  for (int i = 0; i < numBankSwitches; i++) {
    pinMode(bankSwitchesPin[i],INPUT_PULLUP);
  }
  }*/

void setupSwitches() {

  for (int i = 0; i < numPresetSwitches; i++)
  {
    // Setup button timers (all in milliseconds / ms)
    // (These are default if not set, but changeable for convenience)
    presetButtons[i].debounceTime = 20;   // Debounce timer in ms
    presetButtons[i].multiclickTime = 0;  // Time limit for multi clicks
    presetButtons[i].longClickTime = 1000; // Time until long clicks register
  }

  for (int i = 0; i < numBankSwitches; i++) {
    // Setup button timers (all in milliseconds / ms)
    bankButtons[i].debounceTime = 20;   // Debounce timer in ms
    bankButtons[i].multiclickTime = 0;  // Time limit for multi clicks
    bankButtons[i].longClickTime = 1000; // Time until long clicks register
  }
}

void setMulticlickTime(int ms) {
  for (int i = 0; i < numPresetSwitches; i++)
  {
    // Setup button timers (all in milliseconds / ms)
    presetButtons[i].multiclickTime = ms;  // Time limit for multi clicks
  }
}

/*void checkSwitches() {
  int waitTime = 50;//ms
  for (int i = 0; i < numPresetSwitches; i++) {
    if (digitalReadFast(presetSwitchesPin[i]) == HIGH) {
      presetChanged = i + (7 * bank) + 1;
    }
  }
  if (digitalReadFast(bankSwitchesPin[0]) == HIGH) {
    bank--;
    delay(waitTime);
  }
  if (digitalReadFast(bankSwitchesPin[1]) == HIGH) {
    bank++;
    delay(waitTime);
  }
  }*/

void checkSwitches() {
  for (int i = 0; i < numPresetSwitches; i++) {
    presetButtons[i].Update();
    if (presetButtons[i].clicks == 1) {
      presetChanged = i + (6 * (bank - 1)) + 1;
      bankChanged = bank;
    }
    if (presetButtons[i].clicks == 2) {
      presetChanged = (i + (7 * (bank - 1)) + 1) + 6;
    }
    //hold preset to setup pedals
    if (presetButtons[i].clicks == -1) {
      //      if (currentPreset == 0) {
      presetToProgram = i + (7 * (bank - 1)) + 1;
      longClick = true;
      //      }
    }
  }
  for (int i = 0; i < numBankSwitches; i++) {
    bankButtons[i].Update();
    if (bankButtons[i].clicks == 1) {
      if (program) {
        programBank -= i * 2 - 1;
      }
      else {
        bank -= i * 2 - 1;
        if (bank < 1) {
          bank = 1;
        }
        if (bank > 99) {
          bank = 99;
        }
        if (mute) {
          //display bank while bypassed
//          bypassPreset(bank);
//          tuner();
        }
      }
    }
    //long click bank down to activate looper
    if (bankButtons[0].clicks == -1) {
      looper();
    }
  }
}
