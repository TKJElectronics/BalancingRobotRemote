#include <PS3BT.h>
#include "steer.h"

USB Usb;
PS3BT BT(&Usb);

double targetAngle = 90;
int lastDirection = stop;

void setup()
{
  Serial.begin(115200);  
  if (Usb.Init() == -1) 
    while(1); //halt    
}
void loop()
{
  Usb.Task();
  
  if(BT.PS3BTConnected) {
    if(BT.getButton(PS))
    {
      steer(shutdown);
      BT.disconnect();      
    }
    else if(BT.getButton(SELECT))
      steer(shutdown);
    else if(BT.getButton(START))
      steer(resume);

    if(BT.getButton(UP)) {
      targetAngle += 0.05;
      Serial.print("T,");
      Serial.print(targetAngle);      
      Serial.print(";");      
      while(BT.getButton(UP)) // Wait for release
        Usb.Task();
    } else if(BT.getButton(DOWN)) {
      targetAngle -= 0.05;
      Serial.print("T,");
      Serial.print(targetAngle);
      Serial.print(";");
      while(BT.getButton(DOWN)) // Wait for release
        Usb.Task();
    }

    if(BT.getAnalogHat(LeftHatY) > 137) {
      if(BT.getAnalogHat(RightHatY) > 137)
        steer(backward);
      else if(BT.getAnalogHat(RightHatY) < 117)
        steer(leftRotate);
      else
        steer(left);
    } else if(BT.getAnalogHat(LeftHatY) < 117) {
      if(BT.getAnalogHat(RightHatY) < 117)
        steer(forward);
      else if(BT.getAnalogHat(RightHatY) > 137)
        steer(rightRotate);
      else
        steer(right);   
    } else if(BT.getAnalogHat(RightHatY) > 137)  {
        steer(right);   
    } else if(BT.getAnalogHat(RightHatY) < 117) {
        steer(left);   
    } else 
        steer(stop);    
  }
}

void steer(steerDirection direction) {
  if(direction == forward && lastDirection != forward)
    Serial.print("F");
  else if(direction == backward && lastDirection != backward)
    Serial.print("B");
  else if(direction == left && lastDirection != left)
    Serial.print("L");
  else if(direction == leftRotate && lastDirection != leftRotate)
    Serial.print("LR");
  else if(direction == right && lastDirection != right)
    Serial.print("R");
  else if(direction == rightRotate && lastDirection != rightRotate)
    Serial.print("RR");
  else if(direction == stop && lastDirection != stop)
    Serial.print("S");
  else if(direction == shutdown && lastDirection != shutdown)
    Serial.print("A"); // Abort
  else if(direction == resume && lastDirection != resume)
    Serial.print("C"); // Continue

  lastDirection = direction;
}


