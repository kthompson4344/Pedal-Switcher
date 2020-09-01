void setLED(int num, int R, int G, int B) {
  if (num >= 0) {
    Serial3.print("<");
    Serial3.print(num);
    Serial3.print(",");
    Serial3.print(R);
    Serial3.print(",");
    Serial3.print(G);
    Serial3.print(",");
    Serial3.print(B);
    Serial3.print(">");
  }
  else {
    Serial.print("Error: Trying to set LED number ");
    Serial.println(num);
  }
}

void LEDsOff() {
  for (int i = 0; i < 9; i++) {
    setLED(i, 0, 0, 0);
  }
}
