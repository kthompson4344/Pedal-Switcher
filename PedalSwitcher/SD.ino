void loadSinglePreset(int number) {
  myFile = SD.open(filename);
  // if the file is available, write to it:
  if (myFile) {
    int recNum = 0; // We have read 0 records so far
    while (myFile.available())
    {
      String list = myFile.readStringUntil('\n');
      recNum++; // Count the record

      if (recNum == number)
      {
        char inputCsvString[128];
        char presetName[maxPresetName];
        char * partOfString; // this is used by strtok_r() as an index
        list.toCharArray(inputCsvString, 128);

        //read preset number
        partOfString = strtok (inputCsvString, ","); // get the first part - the string
        atoi(partOfString);    // copy it to inputCsvString

        //read pedals in preset
        for (int i = 0; i < maxPedals; i++) {
          partOfString = strtok (NULL, ","); // this continues where the previous call left off
          presets[number][i] = atoi(partOfString);     // convert this part to an integer
        }

        //read RGB LED colors
        for (int i = 0; i < 3; i++) {
          partOfString = strtok (NULL, ","); // this continues where the previous call left off
          pedalColors[number][i] = atoi(partOfString);     // convert this part to an integer
        }

        //read and trim preset name string
        partOfString = strtok (NULL, ",");
        strcpy(presetNames[number], partOfString);



        //read MIDI setting TODO
        partOfString = strtok (NULL, ","); // this continues where the previous call left off
        atoi(partOfString);   // convert this part to a float
      }
    }
    myFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void loadAllPresets() {
  myFile = SD.open(filename);
  // if the file is available, write to it:
  if (myFile) {
    int recNum = 0; // We have read 0 records so far
    while (myFile.available())
    {
      String list = myFile.readStringUntil('\n');
      recNum++; // Count the record

      char inputCsvString[128];
      char presetName[maxPresetName + 1];
      char * partOfString; // this is used by strtok_r() as an index
      list.toCharArray(inputCsvString, 128);
      //      Serial.print("PresetNum: "); Serial.println(recNum);
      //read preset number
      partOfString = strtok (inputCsvString, ","); // get the first part - the string
      atoi(partOfString);    // copy it to inputCsvString

      //read pedals in preset
      for (int i = 0; i < maxPedals; i++) {
        partOfString = strtok (NULL, ","); // this continues where the previous call left off
        presets[recNum][i] = atoi(partOfString);     // convert this part to an integer
//        Serial.println(presets[recNum][i]);
      }

      //      Serial.println("LEDs");
      //read RGB LED colors
      for (int i = 0; i < 3; i++) {
        partOfString = strtok (NULL, ","); // this continues where the previous call left off
        pedalColors[recNum][i] = atoi(partOfString);   // convert this part to an integer
        //        Serial.println(pedalColors[recNum-1][i]);
      }

      //read and trim preset name string
      partOfString = strtok (NULL, ",");
      strcpy(presetNames[recNum], partOfString);
//      Serial.println(presetNames[recNum]);



      //read MIDI setting TODO
      partOfString = strtok (NULL, ","); // this continues where the previous call left off
      //MIDIPreset[recNum] = atoi(partOfString);   // convert this part to a float

    }
    totalPresets = recNum;
    myFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
}

void programPresets() {
  bool arrange = true;
  int presetToProgramLocal = presetToProgram;
  int bankToProgramLocal = bank;
  longClick = false;
  bool firstTime = true;
  int saveStep = 0;
  int charPlace = 0;
  int LEDPlace = 0;
  int colors[3];
  colors[0] = pedalColors[presetToProgramLocal][0];
  colors[1] = pedalColors[presetToProgramLocal][1];
  colors[2] = pedalColors[presetToProgramLocal][2];
  programBank = 50;
  int prevBank = bank;
  setMulticlickTime(250);
  preset(currentBank, presetToProgramLocal, presetNames[presetToProgramLocal], true);
  setLED(presetToProgramLocal - (numPresetSwitches * (bankToProgramLocal - 1)) - 1, pedalColors[presetToProgramLocal][0], pedalColors[presetToProgramLocal][1], pedalColors[presetToProgramLocal][2]);
  while (program) {
    checkSwitches();
    if (longClick && presetToProgram == presetToProgramLocal) {
      if (saveStep == 1 && save) {
        saveStep++;
        pedalColors[presetToProgramLocal][0] = colors[0];
        pedalColors[presetToProgramLocal][1] = colors[1];
        pedalColors[presetToProgramLocal][2] = colors[2];
        Serial.println("Save to SD");
      }
      if (saveStep == 0 && save) {
        saveStep++;
        presetNames[presetToProgramLocal][charPlace] = presetNames[presetToProgramLocal][charPlace] + programBank - 50;
        Serial.println("LED COLOR");
        firstTime = true;
      }
      if (arrange) {
        arrange = false;
        Serial.println("Save Mode");
        save = true;
      }
      longClick = false;
      presetToProgram = 0;
    }
    if (arrange) {
      //single click to turn that pedal on/off
      //double click to turn that pedalx2 on/off
      //display current pedal order on screen
      if (presetChanged > 0) {
        presetChanged -= ((bank-1) * 6);
        int _numPedals = 0;
        bool inPreset = false;
        //count number of pedals
        for (int i = maxPedals - 1; i >= 0; i--) {
          if (currentPedals[i] > 0) {
            _numPedals++;
            if (currentPedals[i] == presetChanged) {
              inPreset = true;
            }
          }
        }
        //check if pedal is already in preset, if it is then remove, if not add to end
        if (inPreset) {
          bool shift = false;
          if (_numPedals == 1) {
            shift = true;
          }
          for (int i = 0; i < _numPedals; i++) {
            if (currentPedals[i] == presetChanged) {
              shift = true;
            }
            if (shift) {
              currentPedals[i] = currentPedals[i + 1];
            }
          }
          if (shift) {
            currentPedals[_numPedals] = 0;
          }
        }
        else {
          currentPedals[_numPedals] = presetChanged;
        }
        allOff();
        printPedals();
        preset(currentBank, currentPreset, presetNames[currentPreset], true);
        muxOn();
        presetChanged = 0;
      }
    }
    if (save) {
      int character = 0;
      //longClick
      if (programBank != prevBank) {
        firstTime = true;
      }
      //doubleClick
      if (presetChanged == presetToProgramLocal + numPresetSwitches) {

      }
      //presetName
      if (saveStep == 0) {
        if (longClick && presetToProgram == presetToProgramLocal) {
          programBank = 50;
          firstTime = true;
        }
        if (presetChanged == presetToProgramLocal) {
          presetNames[presetToProgramLocal][charPlace] = presetNames[presetToProgramLocal][charPlace] + programBank - 50;
          charPlace++;
          if (charPlace >= maxPresetName) {
            charPlace = maxPresetName - 1;
          }
          firstTime = true;
          programBank = 50;
        }
        if (firstTime) {
          char letter = presetNames[presetToProgramLocal][charPlace] + programBank - 50;
//                    Serial.println(int(letter));
//                    Serial.println(programBank);
          if (int(letter) == 0) {
            letter = 65;
            presetNames[presetToProgramLocal][charPlace] = 65;
          }
          if (letter < 32) {
            letter = 32;
          }
          if (letter > 126) {
            letter = 126;
          }
          preset(currentBank, currentPreset, presetNames[currentPreset], true);
          u8g2.setFontMode(1);
          u8g2.setDrawColor(1);
          u8g2.setFont(u8g2_font_helvB12_tf);
          u8g2.setCursor(0, 32);
          for (int i = 0; i < charPlace; i++) {
            u8g2.print(char(presetNames[presetToProgramLocal][i]));
          }
          u8g2.setFontMode(0);
          u8g2.setDrawColor(1);
          u8g2.drawBox(u8g2.tx, u8g2.ty - 12, 11, 14);
          u8g2.setDrawColor(0);
          u8g2.setFont(u8g2_font_helvB12_tf);
          u8g2.setCursor(u8g2.tx, 32);
          u8g2.print(letter);
          u8g2.sendBuffer();
          firstTime = false;
          presetChanged = 0;
        }
      }
      //presetColor
      else if (saveStep == 1) {
        if (presetChanged == presetToProgramLocal) {
          LEDPlace++;
          if (LEDPlace > 2) {
            LEDPlace = 0;
          }
          firstTime = true;
        }
        if (digitalReadFast(bankSwitchesPin[1]) == LOW) {
          colors[LEDPlace]--;
          delay(1);
          firstTime = true;
          if (colors[LEDPlace] < 0) {
            colors[LEDPlace] = 0;
          }
        }
        if (digitalReadFast(bankSwitchesPin[0]) == LOW) {
          colors[LEDPlace]++;
          delay(1);
          firstTime = true;
          if (colors[LEDPlace] > 255) {
            colors[LEDPlace] = 255;
          }
        }
        if (firstTime) {
          setLED(presetToProgramLocal - (numPresetSwitches * (bankToProgramLocal - 1)) - 1, colors[0], colors[1], colors[2]);
          //          setLCDLED(colors[0], colors[1], colors[2]);
          displayRGB(bankToProgramLocal, presetToProgramLocal, colors[0], colors[1], colors[2], LEDPlace);
          firstTime = false;
          presetChanged = 0;
        }
      }
      //save to SD
      else {
        //save currentPedals to preset array
        for (int i = 0; i < maxPedals; i++) {
          presets[presetToProgramLocal][i] = currentPedals[i];
        }
        SD.remove("presets.txt");
        // open the file. note that only one file can be open at a time,
        // so you have to close this one before opening another.
        myFile = SD.open("presets.txt", FILE_WRITE);

        // if the file opened okay, write to it:
        if (myFile) {
          for (int presetToSave = 1; presetToSave <= numPresets; presetToSave++) {
            myFile.print(presetToSave);
            myFile.print(",");
            for (int i = 0; i < maxPedals; i++) {
              myFile.print(presets[presetToSave][i]);
              myFile.print(",");
            }
            for (int i = 0; i < 3; i++) {
              myFile.print(pedalColors[presetToSave][i]);
              myFile.print(",");
            }
            myFile.print(presetNames[presetToSave]);
            myFile.print(",");
            myFile.println(MIDIPreset[currentPreset]);
          }
          myFile.close();
          Serial.println("done.");
          dumpFile();
        }
        preset(currentBank, currentPreset, presetNames[currentPreset], false);
        program = false;
      }
      prevBank = programBank;
    }
  }
  setMulticlickTime(0);
  longClick = false;
  program = false;
  save = false;
}
