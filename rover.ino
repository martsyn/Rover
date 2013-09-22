#include <Wire.h>
#include <HMC5883L.h>

int motor_pin1 = 4;
int motor_pin2 = 5;
int motor_pin3 = 6;
int motor_pin4 = 7;

//LED pin numbers
int greenLed = 3, redLed = 2;

HMC5883L compass;

void setup ()
{
  Serial.begin(9600);

  pinMode(motor_pin1,OUTPUT);
  pinMode(motor_pin2,OUTPUT);
  pinMode(motor_pin3,OUTPUT);
  pinMode(motor_pin4,OUTPUT);

  compass = HMC5883L();

  Serial.println("Setting scale to +/- 1.3 Ga");
  int error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));

  Serial.println("Setting measurement mode to continuous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}
void loop()
{
  forward();
  delay(1000);
  turnleft();
  
  halt();
  delay(10000);
}
 
void forward() {                            // use combination which works for you
   digitalWrite(motor_pin1,HIGH);
   digitalWrite(motor_pin2,LOW);
   digitalWrite(motor_pin3,HIGH);
   digitalWrite(motor_pin4,LOW);
   return;
 }

void backward() {
  digitalWrite(motor_pin1,LOW);
  digitalWrite(motor_pin2,HIGH);
  digitalWrite(motor_pin3,LOW);
  digitalWrite(motor_pin4,HIGH);
  delay(500);
  halt();
  return;
}

void halt () {
  digitalWrite(motor_pin1,LOW);
  digitalWrite(motor_pin2,LOW);
  digitalWrite(motor_pin3,LOW);
  digitalWrite(motor_pin4,LOW);
  delay(500);                          //wait after stopping
  return;
}

float getHeading()
{
  MagnetometerRaw dir = compass.ReadRawAxis();
  return atan2(dir.YAxis, dir.XAxis);
}

float cropAngle(float src)
{
  if (src > PI)
    return src - 2*PI;
  if (src < -PI)
    return src + 2*PI;
  return src;
}

void turnleft () {
  
  float initHeading = getHeading();
  
  digitalWrite(motor_pin1,HIGH);       //use the combination which works for you
  digitalWrite(motor_pin2,LOW);      //right motor rotates forward and left motor backward
  digitalWrite(motor_pin3,LOW);
  digitalWrite(motor_pin4,HIGH);

  for(;;)
  {
    float heading = getHeading();
    float change = heading - initHeading;
    if (change > PI)
      change -= 2*PI;
    if (change < -PI)
      change += 2*PI;
    
    Serial.print("init: ");
    Serial.print((int)(initHeading/PI*180));
    Serial.print("\tcurrent: ");
    Serial.print((int)(heading/PI*180));
    Serial.print("\tchange: ");
    Serial.print((int)(change/PI*180));
    Serial.println();
    
    if (change > PI/2)
    {
      Serial.println("great success!!!");
      break;
    }
  }
  
  halt();
  return;
}

void turnright () {
  digitalWrite(motor_pin1,LOW);       //use the combination which works for you
  digitalWrite(motor_pin2,HIGH);    //left motor rotates forward and right motor backward
  digitalWrite(motor_pin3,HIGH);
  digitalWrite(motor_pin4,LOW);
  delay(1000);                              // wait for the robot to make the turn
  halt();
  return;
}
