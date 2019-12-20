//  Send a two byte midi message  
//send program change message data on channel 1 (C0)
void midiProg(char status, int data ) {
  Serial1.write(status);
  Serial1.write(data);
}


//  Send a CC midi message  
void midiCC(char status, int data , int val) {
  Serial1.write(status);
  Serial1.write(data);
  Serial1.write(val);
}
