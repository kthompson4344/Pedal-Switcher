//void tunerTest() {
//  // read back fundamental frequency
//  int note;
//  static int timer = millis();
//  static int timer2 = timer;
//  digitalWrite(tunerEnable, HIGH);
//  if (tuner.available()) {
//    float freq = tuner.read();
//    float prob = tuner.probability();
//    //Serial.printf("Note: %3.2f | Probability: %.2f\n", note, prob);
//    note = detectNote(freq);
//    Serial.println(notesNames[note]);
//    float diff = findDiff(note, freq);
//    //Serial.println(findDiff(note, freq));
////    Serial.println(millis() - timer);
//    timer = millis();
//    Serial.println(diff);
//    if (millis() - timer2 > (500 - abs(diff)*100)) {
//    timer2 = millis();
//    }
//  }
//  if ((millis() - timer) > 1000) {
//    Serial.println("STOPPED");
//  }
//  delay(10);
//}
//
//int detectNote(float freq) {
//  int closestFreq;
//  float closestDiff;
//  int mult;
//  float diff;
//  for (int i = 0; i < numNotes; i++) {
//    //mult = fmodf(freq, notes[i]);
//    mult = round(freq / notes[i]);
//    diff = abs((freq / mult) - notes[i]);
//
//    if (i == 0) {
//      //condition to check whether number is power of two or not
//      //performing bitwise operation
//      if ((mult != 0) && ((mult & (mult - 1)) == 0)) {
//        closestFreq = 0;
//        closestDiff = diff;
//      }
//      else {
//        closestDiff = 100;
//      }
//    }
//    else {
//      if (diff < closestDiff) {
//        if ((mult != 0) && ((mult & (mult - 1)) == 0)) {
//          closestFreq = i;
//          closestDiff = diff;
//        }
//      }
//    }
//  }
//
//  return closestFreq;
//}
//
//float findDiff(int note, float freq) {
//  float baseFreq = notes[note];
//  int mult = round(freq / baseFreq);
//  return freq - (baseFreq * mult);
//}
