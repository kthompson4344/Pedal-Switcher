void preset1() {
  u8g2.clearBuffer();
  u8g2.setFontMode(0);
  u8g2.setFont(u8g2_font_helvB10_te);
  u8g2.setCursor(0, 12);
  u8g2.print(F("B: 0                P: 3"));
  u8g2.setFont(u8g2_font_helvB12_tf);
  u8g2.setCursor(0, 35);
  u8g2.print(F("SOLO"));
  u8g2.setFont(u8g2_font_open_iconic_arrow_1x_t);
  u8g2.sendBuffer();
}
