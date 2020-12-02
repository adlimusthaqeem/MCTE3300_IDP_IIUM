#include <SoftwareSerial.h>
SoftwareSerial Sim900(15, 14);
SoftwareSerial SUART(2, 3);

//-----------------------------------I / O Pin Assignment---------------------------------------- /
int dirSwing    = 47; //direction pin for motor swing
int dirIncline = 46;
int startManual = 53;
int startAuto = 52;
int soundSensor = 45;
int upLimit = 50;
int lowLimit = 49;
int alarm = 42;
int valtemp;
int tempPin = 1;

//----------------------------------Data variables----------------------------------------------- /
int a = 1;
int speedSwing = 100;  //speed of motor
float humidity;    //to store humidity data
float temperature; //to store temperature data
int ManualState = 0;
int AutoState = 0;
int soundAlert = 0;
int inclineDown = 0;

void setup()
{
  Sim900.begin(9600);
  Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  SUART.begin(9600); //enable SUART Port
  delay(100);

  //------------//General I/O//----------------------//
  pinMode(dirSwing,   OUTPUT);
  pinMode(dirIncline,   OUTPUT);
  pinMode(startManual, INPUT_PULLUP);
  pinMode(startAuto, INPUT_PULLUP);
  pinMode(soundSensor, INPUT);
  pinMode(upLimit, INPUT_PULLUP);
  pinMode(lowLimit, INPUT_PULLUP);
  pinMode(alarm, OUTPUT);
  pinMode(MISO, OUTPUT);

  mpuSetup();
}

void loop()
{
  reading = mpuRead();
  yawAngle2 = map(reading, 0, 360, 0, 255);
  if (a)
  {
    setYawAngle2 = yawAngle2;
    a = false;
  }

  short yawDiff = setYawAngle2 - yawAngle2;
  if ((yawDiff > 250 ) || (yawDiff < -250))
  {
    SUART.print("!!!!");
    SendMessage();
  }

  int manualPush = digitalRead(startManual);
  int autoPush = digitalRead(startAuto);
  if ((manualPush == 1) || (ManualState == 1))
  {
    ManualState = 1;
    AutoState = 0;
    manualMode();
    soundAlert = 0;
  }

  if ((autoPush == 1) || (AutoState == 1))
  {
    ManualState = 0;
    AutoState = 1;
    //inclineDown = 0;
    autoMode();
  }
}

void manualMode()
{
  int inclineUp = 0;
  if (digitalRead(soundSensor) != 1 && soundAlert != 1)
  {
    if ((digitalRead(upLimit) != 1) )//&& inclineUp == 0)
    {
      digitalWrite(dirIncline, HIGH);
      digitalWrite(dirSwing, LOW);
      //inclineUp = 1;
    }

    else
    {
      digitalWrite(dirSwing, HIGH);
      digitalWrite(dirIncline, LOW);
    }
  }

  else
  {
    soundAlert = 1;
    digitalWrite(dirIncline, LOW);
    digitalWrite(dirSwing, LOW);
    digitalWrite(alarm, HIGH);
    SendMessage();
    ManualState = 0;
    delay(1000);
    digitalWrite(alarm, LOW);
    return;
  }

}

void autoMode()
{
  if (digitalRead(soundSensor) != 1)
  {
    digitalWrite(alarm, LOW);
    digitalWrite(dirIncline, HIGH);
    digitalWrite(dirSwing, LOW);

    if ((digitalRead(lowLimit) == 1) )//&& (inclineDown == 0))
    {
      digitalWrite(dirIncline, LOW);
      digitalWrite(dirSwing, LOW);
      inclineDown = 1;
    }
  }

  else
  {
    digitalWrite(dirSwing, HIGH);
    digitalWrite(dirIncline, LOW);
    digitalWrite(alarm, HIGH);
  }

}

void SendMessage()
{
  Sim900.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  Sim900.println("AT+CMGS=\"+60138898531\"\r"); // Replace x with mobile number
  delay(1000);
  Sim900.println("Baby Wake Up");// The SMS text you want to send
  delay(100);
  Sim900.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}














