void tuner() {
  long prevTime1 = 0;
  long prevTime2 = 0;
  int interval1 = 10;
  int interval2 = 10;
  int i = 0;
  //turn off morph
  midiOutput1.sendControlChange(11, 0, 1);
  delay(100);
  kemper.tunerOn();
  while (1) {
    checkSwitches();
    if (presetChanged > 0) {
      updatePedals();
      changePreset();
      kemper.tunerOff();
      break;
    }
    if (millis() > prevTime1 + interval1) {
      kemper.getParam(124, 81);
      kemper.getParam(125, 84);
      prevTime1 = millis();
    }
    if (millis() > prevTime2 + interval2) {
      interval2 = abs(1000/tune);
      if (tune > 0) {
        i++;
        if (i > 131) i = 0;
      }
      if (tune < 0) {
        i--;
        if (i < 0) i = 131;
      }
      prevTime2 = millis();
      tunerLCD(notesNames[note % 12], i, true);//C = 36, E = 40, A = 45, D = 50, etc
    }
    recvWithStartEndMarkers();
    showNewData();
  }
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  int startMarker = 240;
  int endMarker = 247;
  int rc;

  while (Serial1.available() > 0 && newData == false) {
    rc = Serial1.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void showNewData() {
  if (newData == true) {
//    Serial.print("This just in ... ");
    //7/8 are what is requested
    //9/10 are what is sent
    if (receivedChars[7] == 124 && receivedChars[8] == 81) {
//      Serial.print("Tune: ");Serial.print(receivedChars[9]);Serial.print(" ");Serial.println(receivedChars[10]);
      tune = ((receivedChars[9]*128)+(receivedChars[10]) - 8191)/162;
    }
    if (receivedChars[7] == 125 && receivedChars[8] == 84) {
      note = receivedChars[10];
    }
    newData = false;
  }
}




/*
  void tunerTest() {
  // read back fundamental frequency
  int note;
  static int timer = millis();
  static float avgDiffSum = 0;
  static int count = 0;
  //  static int timer2 = timer;
  digitalWrite(tunerEnable, HIGH);
  if (tuner.available()) {
  //    Serial.println("HERE");
    float freq = tuner.read();
    float prob = tuner.probability();
    //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
    note = detectNote(freq);
  //    Serial.println(notesNames[note]);
    float diff = findDiff(note, freq);
    static float avgDiff = 0;

    //Serial.println(findDiff(note, freq));
    //    Serial.println(millis() - timer);
    timer = millis();
    avgDiffSum += diff;
    count++;
    if (count >= 10) {
      avgDiff = avgDiffSum / count;
      avgDiffSum = 0;
      count = 0;
    }
  //    Serial.println(avgDiff);
    tunerLCD(notesNames[note], avgDiff, false);
    if (avgDiff < 0) {
      int green = 40 + avgDiff * 20;
      if (green < 0) {
        green = 0;
      }
      setLED(6, -90 * avgDiff, 0 , 0);
      setLED(7, 0, green, 0);
      setLED(8, 0, 0 , 0);
    }
    if (diff > 0) {
      int green = 40 - avgDiff * 20;
      setLED(8, 0, 0 , 90 * avgDiff);
      setLED(7, 0, green, 0);
      setLED(6, 0, 0 , 0);
    }
    //    if (millis() - timer2 > (500 - abs(diff)*100)) {
    //    timer2 = millis();
    //    }
  }
  if ((millis() - timer) > 500) {
  //    Serial.println("STOPPED");
    tunerLCD(0, 0, true);
    avgDiffSum = 0;
    count = 0;
    setLED(8, 0, 0, 0);
    setLED(7, 0, 0, 0);
    setLED(6, 0, 0, 0);
  }
  delay(1);
  }

  int detectNote(float freq) {
  int closestFreq;
  float closestDiff;
  int mult;
  float diff;
  for (int i = 0; i < numNotes; i++) {
    //mult = fmodf(freq, notes[i]);
    mult = round(freq / notes[i]);
    diff = abs((freq / mult) - notes[i]);

    if (i == 0) {
      //condition to check whether number is power of two or not
      //performing bitwise operation
      if ((mult != 0) && ((mult & (mult - 1)) == 0)) {
        closestFreq = 0;
        closestDiff = diff;
      }
      else {
        closestDiff = 100;
      }
    }
    else {
      if (diff < closestDiff) {
        if ((mult != 0) && ((mult & (mult - 1)) == 0)) {
          closestFreq = i;
          closestDiff = diff;
        }
      }
    }
  }

  return closestFreq;
  }

  float findDiff(int note, float freq) {
  float baseFreq = notes[note];
  int mult = round(freq / baseFreq);
  return freq - (baseFreq * mult);
  }
*/
