

void play_pause() {

  parameterSet();
  outString = "";

  sendString();
  Serial.println(outString);

  playPauseFlag = !playPauseFlag;
  
  playPauseFlag ? display_play() : display_pause();   //display whether playing or Pausing
  delay(1500);
}
