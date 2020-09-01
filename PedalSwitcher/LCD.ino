void preset(int bank, int preset, char* text, bool displayOrder) {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB10_te);
  u8g2.setCursor(0, 12);
  u8g2.print(F("B: "));
  u8g2.print(bank);
  u8g2.setCursor(92, 12);
  u8g2.print(F("P: "));
  u8g2.print(preset);
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setCursor(0, 32);
  u8g2.print(F(text));
  if (displayOrder) {
    displayPedals();
  }
  u8g2.sendBuffer();
}

void bypassPreset(int bank) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_helvB10_te);
  u8g2.setCursor(0, 12);
  u8g2.print(F("B: "));
  u8g2.print(bank);
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setCursor(0, 32);
  u8g2.print(F("BYPASS"));
  u8g2.sendBuffer();
}

void displayPedals() {
  bool firstPedal = true;
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
  u8g2.setCursor(0, 50);
  int displayLength = 0;
  bool secondLine = false;
  for (int i = 0; i < maxPedals; i++) {
    if (currentPedals[i] > 0) {
      if (currentPedals[i] > 9) {
        displayLength += 2;
      }
      else {
        displayLength += 1;
      }
      if (displayLength > 8 && !secondLine) {
        secondLine = true;
        u8g2.setCursor(0, 63);
      }
      if (!firstPedal) {
        u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
        u8g2.setCursor(u8g2.tx + 1, u8g2.ty);
        //ASCII conversion to right arrow
        u8g2.print("B");
        u8g2.setCursor(u8g2.tx + 1, u8g2.ty);
      }
      u8g2.setFont(u8g2_font_helvB10_tf);
      u8g2.print(currentPedals[i]);
      if (firstPedal) {
        firstPedal = false;
      }
    }
    else {
      if (i == 0) {
        u8g2.setFont(u8g2_font_helvB10_tf);
        u8g2.print("No Pedals");
      }
    }
  }
}

void displayRGB(int bank, int preset, int R, int G, int B, int LED) {
  int value;
  const int yStart = 32;
  const int yIncrement = 15;
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB10_te);
  u8g2.setCursor(0, 12);
  u8g2.print(F("B: "));
  u8g2.print(bank);
  u8g2.setCursor(92, 12);
  u8g2.print(F("P: "));
  u8g2.print(preset);
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setCursor(0, yStart);
  u8g2.print(F("RED"));
  u8g2.setCursor(60, yStart);
  u8g2.print(F(": "));
  u8g2.print(R);
  u8g2.setCursor(0, yStart + yIncrement);
  u8g2.print(F("GREEN"));
  u8g2.setCursor(60, yStart + yIncrement);
  u8g2.print(F(": "));
  u8g2.print(G);
  u8g2.setCursor(0, yStart + yIncrement * 2);
  u8g2.print(F("BLUE"));
  u8g2.setCursor(60, yStart + yIncrement * 2);
  u8g2.print(F(": "));
  u8g2.print(B);
  u8g2.setFontMode(0);
  u8g2.setDrawColor(1);
  u8g2.drawBox(69, 19 + yIncrement * LED, 28, 14);
  u8g2.setDrawColor(0);
  u8g2.setCursor(70, yStart + yIncrement * LED);
  if (LED == 0) {
    value = R;
  }
  else if (LED == 1) {
    value = G;
  }
  else if (LED == 2) {
    value = B;
  }
  u8g2.print(value);
  u8g2.sendBuffer();
}

//void setLCDLED(int R, int G, int B) {
//  int highestVal = R;
//  if (G > highestVal) {
//    highestVal = G;
//  }
//  if (B > highestVal) {
//    highestVal = B;
//  }
//  Serial.println(map(R, highestVal, 0, 0, 255));
//  Serial.println(map(G, highestVal, 0, 0, 255));
//  Serial.println(map(B, highestVal, 0, 0, 255));
//  analogWrite(backlightRed, map(R, highestVal, 0, 0, 255));
//  analogWrite(backlightGreen, map(G, highestVal, 0, 0, 255));
//  analogWrite(backlightBlue, map(B, highestVal, 0, 0, 255));
//}

void tunerLCD(char* note, float lineStart, bool blank) {
  const int middle = 60;
  const int lineLength = 15;
  const int lineEnd = 64;
  const int blockWidth = 11;
  const int numBlocks = 6;
  const int allBlocksWidth = numBlocks * 2 * blockWidth;//need to change this number in Tuner page too, TODO/
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB10_te);
  if (true) {
    u8g2.setCursor(middle - 5, 13);
    u8g2.print(note);
    if (tune < -9) {
      u8g2.setCursor(middle - 9, 27);
    }
    else if (tune < 0) {
      u8g2.setCursor(middle - 4, 27);
    }
    else if (tune < 10) {
      u8g2.setCursor(middle - 2, 27);
    }
    else {
      u8g2.setCursor(middle - 6, 27);
    }
    u8g2.print(tune);
    //       topLX   topLY    x       y
    //    for (int i = 0; i < 8; i++) {
    //    u8g2.drawBox(diff * 2, 20, 10, 20); 
    for (int i = 0; i < allBlocksWidth - 1; i++) {
      int shift;
      int shift2;
      int i2 = i * 2;
      
      if (i + lineStart > allBlocksWidth - 1) {
        shift = lineStart - allBlocksWidth;
      }
      if (i2 + lineStart > allBlocksWidth - 1) {
        shift2 = lineStart - allBlocksWidth;
      }
      else {
        shift = lineStart;
        shift2 = lineStart;
      }
      if (i % (blockWidth * 2) < blockWidth) {
        u8g2.drawVLine(i + shift, lineEnd - lineLength, lineLength);
      }
      if (i2 % (blockWidth * 4) < (blockWidth * 2)) {
        u8g2.drawVLine(i2 + 1 + shift2 * 2, lineEnd - (2 * lineLength), lineLength);
        u8g2.drawVLine(i2 + shift2 * 2, lineEnd - (2 * lineLength), lineLength);
      }
      
    }

    //    u8g2.drawBox(lineStart, 30, 10, 35);
    //8 boxes, each 15 wide

  }
  else {
    u8g2.setCursor(middle - 7, 13);
    u8g2.print(F("----"));
  }
  u8g2.sendBuffer();
}

void looperLCD(int mode, int time) {
  u8g2.clearBuffer();
  u8g2.setFontMode(1);
  u8g2.setDrawColor(1);
  u8g2.setFont(u8g2_font_helvB10_te);
  u8g2.setCursor(35, 15);
  u8g2.print("LOOPER");
  u8g2.drawFrame(4, 20, 120, 15);
  if (looperMode == 2) {
    u8g2.setCursor(35, 55);
    u8g2.print("RECORD");
    u8g2.drawBox(4, 20, time, 14);
    looperTimer = millis();
  }
  if (time == 0) {
    Serial.print("LooperTimer: ");
    Serial.println(millis() - looperTimer);
    looperTimer = millis();
  }
  if (looperMode == 3) {
    u8g2.setCursor(30, 55);
    u8g2.print("PLAYBACK");
    u8g2.drawBox(4, 20, time, 14);
  }
  if (looperMode == 4) {

    if (mode == 0) {
      u8g2.setCursor(35, 55);
      u8g2.print("STOP");
    }
    if (mode == 1) {
      u8g2.setCursor(35, 55);
      u8g2.print("OVERDUB");
    }
  }
  if (looperMode == 5) {
    u8g2.setCursor(35, 55);
    u8g2.print("ERASE");
  }
  u8g2.sendBuffer();
}
