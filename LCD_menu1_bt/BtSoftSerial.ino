
void sendString() {
  outString += String(start) + "*" + String(vol) + "*" + String(rpm) + "*" + String(iSpeed) + "*" + String(v);
  Serial.println(outString);

}


void BT_setup() {

  Serial.begin(9600);
  mySerial.begin(9600);
}

void BT_loop() {


  if (mySerial.available()) {
    //    Serial.write(mySerial.read());
    outString = "";
    Serial.println("New Data Available ");
    //    outString=mySerial.read();


    start = mySerial.readStringUntil('*').toInt();
    Serial.print("Vent status :   ");
    Serial.println(start);

    vol = mySerial.readStringUntil('*').toInt();
    Serial.print("Tidal Volume set at :   ");
    Serial.println(vol);

    //    Serial.print("Set RPM :   ");
    rpm = mySerial.readStringUntil('*').toInt();
    Serial.print("RPM set at :   ");
    Serial.println(rpm);

    iSpeed = mySerial.readStringUntil('*').toInt();
    Serial.print("Inspiration Speed set at :   ");
    Serial.println(iSpeed);

    v = mySerial.readString().toInt();
    Serial.print("I:V set at :   1:");
    Serial.println(v);
    sendString();
  }

}
