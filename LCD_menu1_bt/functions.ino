

void play_pause() {

  parameterSet();
  outString = "";

  sendString();
  Serial.println(outString);

  playPauseFlag ? display_play() : display_pause();   //display whether playing or Pausing

  playPauseFlag = !playPauseFlag;


  delay(1500);
}
