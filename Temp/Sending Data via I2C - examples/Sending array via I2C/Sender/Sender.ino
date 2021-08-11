/* Sends messages to receiver
 *  
 * Send the byte data via the data address from sender via I2C
 * Store byte data into the new address in the receiver
 * 
 * Meng Lin - 11/08/2021
 */
 
#include <Wire.h>

float info[3] = {10,100,1000};







void setup()
{
  Wire.begin();
  pinMode(pushbutton,INPUT);
}







void loop()
{
  // changing values
  info[0] += 0.01;
  info[1] += 0.1;
  info[2] += 1;
  
  // transmiting the array as bytes
  Wire.beginTransmission(4); 
  Wire.write((byte*)&info, sizeof(info));
  Wire.endTransmission();   
  delay(10);
}
