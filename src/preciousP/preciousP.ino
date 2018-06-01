/********************************************************
   Precious Plastic Temperature regulator
   operted using PID control and Nextion display

                          written by Giancarlo Bacchio
 ********************************************************/

#include <PID_v1.h>


#define DATAUPDATE_MTBF 1e3  //mean time between data update transmission
#define CALCUPDATE_MTBF 0.25e3  //mean time between data update transmission
#define CHARTUPDATE_MTBF 2e3  //mean time between data update transmission
#define OVERT_LIM 100  // delta Temp max


// Variables used to operate pid control
double Setpoint1=0, temp1=0, Output1=0;
double Setpoint2=0, temp2=0, Output2=0;


unsigned long lastCalc, lastData, lastChart; //last actions time

//PID definition (P_ON_M specifies that Proportional on Measurement be used)
PID myPID1(&temp1, &Output1, &Setpoint1, 1, 5, 1, P_ON_M, DIRECT);//251
PID myPID2(&temp2, &Output2, &Setpoint2, 1, 5, 1, P_ON_M, DIRECT);
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

int contHeatPic = 0;
int contOverPic = 0;
int heat1 = 3;    // heater 1 connected to pin 3
int heat2 = 5;    // heater 1 connected to pin 5
int setTemp1 = 200;
int setTemp2 = 160;
int setPause1 = 0;
int setPause2 = 0;


String mode; //(heating, ready, overT, cooling)
String command; //(start, stop, pause)

void setup()
{
  Serial.begin(9600);
  myPID1.SetMode(AUTOMATIC);
  myPID2.SetMode(AUTOMATIC);
  Serial.println("setpoint1: " + String(Setpoint1));
  Serial.println("setpoint2: " + String(Setpoint2));
  mode = "cooling";
  command = "stop";
  displayInit();
}

void loop()
{
  if ( millis() - lastCalc > CALCUPDATE_MTBF ) { // Update and send only after 1 seconds
    heatingMng();
    readData();
    lastCalc = millis();
  }
  if ( millis() - lastChart > CHARTUPDATE_MTBF ) { // Update and send only after 1 seconds
    chartDraw();
    lastChart = millis();
  }
  if ( millis() - lastData > DATAUPDATE_MTBF ) { // Update and send only after 1 seconds
    setModePic();
    dataTransfer();
    lastData = millis();
  }
}

