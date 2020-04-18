

void play_pause() {
  cmd = "";
  if (parameters_set[0] == 0) {
    //iv mode
    parameterSet();
    cmd += String((int)playPauseFlag) +
           "*" + String(parameters_set[4]) +
           "*" + String(parameters_set[3]) +
           "*" + String(parameters_set[1]) +
           "*" + String(parameters_set[2]);
    Serial.println(cmd);
  }
  else {
    //rpm mode
    parameterSet();
    cmd += String((int)playPauseFlag) +
           "*" + String(parameters_set[4]) +
           "*" + String(parameters_set[3]) +
           "*" + String(parameters_set[1]) +
           "*" + String(parameters_set[2]);
    Serial.println(cmd);
  }
  playPauseFlag ? display_play() : display_pause();
  playPauseFlag = !playPauseFlag;

  delay(1500);
}
