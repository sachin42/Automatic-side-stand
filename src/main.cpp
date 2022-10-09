#include <Arduino.h>

const int forward = 6;
const int reverse = 7;

const int footpad = 8;
const int speedsensor = 9;

const int power = 4;

const int mswitch = 2;
const int motor = 5;
const int motorreverse = 10;
const int motorforword = 11;



float rps = 0;
float timeDiff = 0;
float mps = 0;

boolean footpadstatus = false;

void setup()
{
  Serial.begin(9600);
  pinMode(footpad, INPUT);

  pinMode(forward, OUTPUT);
  pinMode(reverse, OUTPUT);

  pinMode(speedsensor, INPUT);

  pinMode(power, INPUT_PULLUP);

  pinMode(mswitch, INPUT_PULLUP);

  pinMode(motor,OUTPUT);
  pinMode(motorforword,OUTPUT);
  pinMode(motorreverse,OUTPUT);

}

int prev_reading = 0;
unsigned long prev_time = 0;

void speedometer()
{
  int current_reding = digitalRead(speedsensor);
  unsigned long now = millis();

  if (prev_reading == 0 && current_reding == 1)
  {
    timeDiff = (now - prev_time);
    rps = 1.0 / timeDiff * 250;
    mps = rps * 2 * 3.1415926 * 0.080;
    prev_time = now;
  }
  prev_reading = current_reding;
}

void loop()
{

  if (digitalRead(mswitch)== LOW && digitalRead(power)== HIGH)
  {
    analogWrite(motor,100);
    digitalWrite(motorforword,HIGH);
    digitalWrite(motorreverse,LOW);
  }else
  {
    digitalWrite(motorforword,LOW);
    digitalWrite(motorreverse,LOW);
  }
  
  
  speedometer();

  if ((digitalRead(footpad) == LOW) && rps > 0 && digitalRead(power)== HIGH)
  {
    if (!footpadstatus)
    {
      Serial.println("Going up");
      digitalWrite(forward, LOW);
      digitalWrite(reverse, HIGH);
      delay(1250);
      digitalWrite(forward, LOW);
      digitalWrite(reverse, LOW);
      footpadstatus = true;
    }
  }


  if ((digitalRead(footpad) == HIGH) && digitalRead(power) == LOW)
  {
    if (footpadstatus)
    {
      Serial.println("Going down");
      digitalWrite(forward, HIGH);
      digitalWrite(reverse, LOW);
      delay(1250);
      digitalWrite(forward, LOW);
      digitalWrite(reverse, LOW);
      footpadstatus = false;
    }
  }

  rps = 0;
}
