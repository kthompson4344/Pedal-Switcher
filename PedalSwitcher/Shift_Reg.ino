void bypassOutwGND() {
  digitalWrite(latchPin, 0);
  delay(500);
  shiftOut5(1);
  for (int SR = 0; SR < 15; SR++) {
    shiftOut5(31);
  }
  digitalWrite(latchPin, 1);
}

void bypassOut() {
  digitalWrite(latchPin, 0);
  delay(500);
  shiftOut5(1);
  for (int SR = 0; SR < 15; SR++) {
    shiftOut5(0);
  }
  digitalWrite(latchPin, 1);
}

void allOff() {
  digitalWrite(latchPin, 0);
  for (int SR = 0; SR < 15; SR++) {
    shiftOut5(0);
  }
  shiftOut5(1);
  digitalWrite(latchPin, 1);
}

void setMux() {
  //printPedals();
  /*
      bypass: shiftOut5(1), shiftOut5(0) 15x
  */
  for (int i = unityGain; i > 0; i-=2) {
    analogWrite(vactrolV1, i);
    delayMicroseconds(100);
  }
  analogWrite(vactrolV1, 0);
  //initialize shiftOut5 calls to 0
  int shiftVals[16] = {0};
  int _numPedals = 0;
  numPedals = 0;
  //count number of pedals in setting
  for (int i = maxPedals-1; i >= 0; i--) {
    if (currentPedals[i] > 0) {
      _numPedals++;
      numPedals++;
    }
  }
  //set necessary values in shiftVals array
  for (int i = _numPedals - 1; i >= 0; i--) {
      //silkscreen numbering is backwards, the substraction corrects it
      int pedal = maxPedals - currentPedals[i];
      int lastPedal = currentPedals[i - 1];
      //last pedal goes to 1 (input)
      if (i == 0) {
        lastPedal = 1;
        shiftVals[15-pedal] = 1;
      }
      else {
        shiftVals[15-pedal] = pedalDataValues[lastPedal]; 
      }
  }
  //last slot is the data value of the last pedal
  //first slot is GND so disable
  shiftVals[15] = 0;
  //put buffer value in last slot
  shiftVals[0] = 29;
  //put pedal in 2nd to last (buffer's) slot
  shiftVals[1] = pedalDataValues[currentPedals[_numPedals-1]];

  //set shift registers
  digitalWrite(latchPin, 0);

  for (int i = 0; i <= 15; i++) {
    shiftOut5(shiftVals[i]);
  }
  digitalWrite(latchPin, 1);
  for (int i = 0; i < unityGain; i+=2) {
    analogWrite(vactrolV1, i);
    delayMicroseconds(100);
  }
  analogWrite(vactrolV1, unityGain);
}





void shiftOut5(byte myDataOut) {
  // This shifts 8 bits out MSB first,
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i = 0;
  int pinState;

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(dataPin, 0);
  digitalWrite(clockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights.
  for (i = 4; i >= 0; i--)  {
    digitalWrite(clockPin, 0);

    //if the value passed to myDataOut and a bitmask result
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(dataPin, pinState);
    //register shifts bits on upstroke of clock pin
    digitalWrite(clockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(dataPin, 0);
  }

  //stop shifting
  digitalWrite(clockPin, 0);
}
