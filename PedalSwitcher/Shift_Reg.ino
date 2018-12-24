void allOff() {
//  digitalWrite(16, HIGH);
//  delay(10);
  digitalWrite(latchPin, 0);
  shiftOut5(1);
  for (int SR = 0; SR < 15; SR++) {
    shiftOut5(0);
  }
  digitalWrite(latchPin, 1);
}

void setMux() {
  /*
  Example: 11->1->6:
  Last pedal is 6
  Go to slot 6 and give it 31
  Next moving left is 1
  Go to slot 1 and give it 6's data value (13)
  Next moving left is 11
  Go to slot 11 and give it 1's data value (3)
  Slot 0 is 11's data value (23)
  */

  //initialize shiftOut5 calls to 0
  int shiftVals[16] = {0};
  int _numPedals = 0;
  //count number of pedals in setting
  for (int i = 13; i >=0; i--) {
    if (currentPedals[i] > 0) {
      _numPedals++;
    }
  }
  //set necessary values in shiftVals array
  for (int i = _numPedals - 1; i >=0; i--) {
    if (i == _numPedals - 1) {
      //first slot is always OUT=31=pedalDataValues[0]
		shiftVals[currentPedals[_numPedals - 1]] = pedalDataValues[0];
    }
    else {
      int pedal = currentPedals[i];
      int lastPedal = currentPedals[i + 1];
      shiftVals[pedal] = pedalDataValues[lastPedal];
    }
  }
  //last slot is the data value of the first pedal
  shiftVals[0] = pedalDataValues[currentPedals[0]];
  
  //set shift registers
  digitalWrite(latchPin, 0);
  for (int i = 15; i >= 0; i--) {
    shiftOut5(shiftVals[i]);
	Serial.println(shiftVals[i]);
  }
  digitalWrite(latchPin, 1);
  digitalWrite(16, LOW);
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
