void SerialChange (int baudrate)  {
  // Set new baud rate of nextion to 38400, but is temporal. Next time nextion is
  // power on, it will retore to default baud of 9600.
  // To take effect, make sure to reboot the arduino (reseting arduino is not enough).
  // If you want to change the default baud, send the command as "bauds=38400", instead of "baud=38400".
  // If you change default, everytime the nextion is power on is going to have that baud rate.

  Serial.print("baud=" + String(baudrate));
  delay(500);  // This dalay is just in case the nextion display didn't start yet, to be sure it will receive the following command.
  Serial.print("baud=" + String(baudrate));
  Serial.write(0xff);  // We always have to send this three lines after each command sent to nextion.
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.end();  // End the  Serial comunication of baud=9600.
  Serial.begin(baudrate);  // Start  Serial comunication at baud=38400.
}

void sendData(String ref, String value) {
  Serial.print(ref + "=");
  int len = ref.length();
  if (ref.substring(len - 3, len).equals("txt")) Serial.print("\"");
  Serial.print(value);
  if (ref.substring(len - 3, len).equals("txt")) Serial.print("\"");
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}

void visible(String name, bool vis) {
  Serial.print("vis " + name + ",");
  Serial.print(vis);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}
