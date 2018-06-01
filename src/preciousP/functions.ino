
void displayInit()  {
  // logo init

  visible("cool", true);
  visible("heating0", false);
  visible("heating1", false);
  visible("heating2", false);
  visible("heating3", false);
  visible("start", true);
  visible("stop", false);
  visible("pause", false);

  visible("start1", false);
  visible("stop1", true);
  visible("pause1", true);
  visible("overT1", false);
  visible("overT", false);
  visible("ready", false);

  sendData("valtemp1.txt", String(temp1) + "*C");
  sendData("valtemp2.txt", String(temp2) + "*C");
  sendData("temp1.val", String((int)temp1 / 3));
  sendData("temp2.val", String((int)temp2 / 3));
  sendData("settemp1.txt", String(setTemp1));
  sendData("settemp2.txt", String(setTemp2));
}


void setModePic()  {
  if (mode.equals("heating"))  {
    visible("heating" + String(contHeatPic), false);
    contHeatPic++;
    if (contHeatPic > 3) contHeatPic = 0;
    visible("heating" + String(contHeatPic), true);
  }
  else  visible("heating" + String(contHeatPic), false);
  if (mode.equals("ready")) visible("ready", true);
  else   visible("ready", false);
  if (mode.equals("cooling"))   visible("cool", true);
  else   visible("cool", false);
  if (mode.equals("overT")) {
    contOverPic = -1 * contOverPic + 1;
    visible("overT" + String(contOverPic), true);
  }
  else  visible("overT" + String(contOverPic), false);
}


void dataTransfer() {
  sendData("valtemp1.txt", String((int)temp1) + "*C");
  sendData("valtemp2.txt", String((int)temp2) + "*C");
  sendData("temp1.val", String((int)(Output1 / 256 * 100))); //(temp1 / 3)));
  sendData("temp2.val", String((int)(Output2 / 256 * 100))); //(temp2 / 3)));
  sendData("settemp1.txt", String(setTemp1));
  sendData("settemp2.txt", String(setTemp2));

}

void chartDraw()  {

  // Serial.print("add 4,0,");
  // Serial.print(String((int)(temp1 / 3)));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("add 4,0,");
  Serial.print(String((int)(temp1 / 3)));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("add 4,1,");
  Serial.print(String((int)(temp2 / 3)));
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

}

void heatingMng() {
  analogWrite(heat1, 0);
  analogWrite(heat2, 0);
  Output1 = 0;
  Output2 = 0;
  temp1 = (1100 - analogRead(0)) / 4.2;
  temp2 = (1100 - analogRead(2)) / 4.2;
  if (!command.equals("stop")) {
    if (command.equals("pause")) {
      Setpoint1 = setPause1;
      Setpoint2 = setPause2;
    }
    if (command.equals("start")) {
      Setpoint1 = setTemp1;
      Setpoint2 = setTemp2;
    }
    //  Serial.println("temp1: " + String(temp1) + ", temp2: " + String(temp2));
    myPID1.Compute();
    Output1 = Output1;
    analogWrite(heat1, Output1);
    // Serial.println("heater1: " + String(Output1));
    myPID2.Compute();
    Output2 = Output2 / 1.5;
    analogWrite(heat2, Output2);
    // Serial.println("heater2: " + String(Output2));
  }
  if (Output1 > 0 || Output2 > 0) mode = "heating";
  if (temp1 < setTemp1 + 2 && temp1 > setTemp1 - 2) {
    if (temp2 < setTemp2 + 2 && temp2 > setTemp2 - 2) mode = "ready";
  }
  if (temp1 > Setpoint1 + (double)OVERT_LIM) mode = "overT";
  if (temp2 > Setpoint2 + (double)OVERT_LIM) mode = "overT";
  if (command.equals("stop") && !mode.equals("overT")) mode = "cooling";
  if (mode.equals("overT")) {
    command = "stop";
    visible("start", true);
    visible("stop", false);
    visible("pause", false);
    visible("start1", false);
    visible("stop1", true);
    visible("pause1", true);
  }
}



void readData() {
  int incomingByte;
  while (Serial.available()) { // wait for the serial port to senr data
    incomingByte = Serial.read();
    Serial.println(incomingByte, HEX);
    if ( String(incomingByte, HEX).equals("5")) {
      command = "start";
      visible("start", false);
      visible("stop", true);
      visible("pause", true);
      visible("start1", true);
      visible("stop1", false);
      visible("pause1", false);
    }
    if ( String(incomingByte, HEX).equals("6")) {
      command = "stop";
      visible("start", true);
      visible("stop", false);
      visible("pause", false);
      visible("start1", false);
      visible("stop1", true);
      visible("pause1", true);
    }
    if ( String(incomingByte, HEX).equals("7")) {
      command = "pause";
      setPause1 = temp1;
      setPause2 = temp2;
      visible("start", true);
      visible("stop", true);
      visible("pause", false);
      visible("start1", false);
      visible("stop1", false);
      visible("pause1", true);
    }
    if ( String(incomingByte, HEX).equals("10")) {
      setTemp1++;
    }
    if ( String(incomingByte, HEX).equals("11")) {
      setTemp1--;
    }
    if ( String(incomingByte, HEX).equals("12")) {
      setTemp2++;
    }
    if ( String(incomingByte, HEX).equals("13")) {
      setTemp2--;
    }
  }
}

