/* 
 * The code is released under the GNU General Public License.
 * Developed by Kristian Lauszus
 * This is the remote control code for my balancing robot: https://github.com/TKJElectronics/BalancingRobot
 * It uses my PS3 Controller Bluetooth library for Arduino: https://github.com/TKJElectronics/USB_Host_Shield_2.0
 * For details, see http://blog.tkjelectronics.dk/2012/02/the-balancing-robot/
 */

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
    while(1); // halt
  steer(stop); // Send stop command at startup
}
void loop()
{
  Usb.Task();
  
  if(BT.PS3BTConnected) {
    if(BT.getButton(PS)) {
      steer(stop);
      BT.disconnect();
    } else if(BT.getButton(SELECT))
      steer(shutdown);
    else if(BT.getButton(START))
      steer(resume);

    if(BT.getButton(UP)) {
      targetAngle -= 0.05;
      Serial.print("T,");
      Serial.print(targetAngle);      
      Serial.print(";");      
      while(BT.getButton(UP)) // Wait for release
        Usb.Task();
    } else if(BT.getButton(DOWN)) {
      targetAngle += 0.05;
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
  if(direction == forward) { // It should keep sending the speed
    double speed = (double)(map(BT.getAnalogHat(LeftHatY),116,0,0,5) + map(BT.getAnalogHat(RightHatY),116,0,0,5))/2; // calculate the average
    Serial.print("F,");
    Serial.print(speed);
    Serial.print(";");    
  } else if(direction == backward) { // It should keep sending the speed
    double speed = (double)(map(BT.getAnalogHat(LeftHatY),138,255,0,5) + map(BT.getAnalogHat(RightHatY),138,255,0,5))/2; // calculate the average
    Serial.print("B,");
    Serial.print(speed);
    Serial.print(";");
  } else if(direction == left && lastDirection != left)
    Serial.print("L;");
  else if(direction == leftRotate && lastDirection != leftRotate)
    Serial.print("LR;");
  else if(direction == right && lastDirection != right)
    Serial.print("R;");
  else if(direction == rightRotate && lastDirection != rightRotate)
    Serial.print("RR;");
  else if(direction == stop && lastDirection != stop)
    Serial.print("S;");
  else if(direction == shutdown && lastDirection != shutdown)
    Serial.print("A;"); // Abort
  else if(direction == resume && lastDirection != resume)
    Serial.print("C;"); // Continue

  lastDirection = direction;
  
  delay(10);
}
