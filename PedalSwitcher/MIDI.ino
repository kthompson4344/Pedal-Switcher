//  Send a two byte midi message  
void midiProg(char status, int data ) {
  Serial1.write(status);
  Serial1.write(data);
}
