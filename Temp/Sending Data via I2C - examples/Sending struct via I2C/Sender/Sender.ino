/* Sends messages to receiver
 *  
 * Send the byte data via the data address from sender via I2C
 * Store byte data into the new address in the receiver
 * 
 * Meng Lin - 11/08/2021
 */
 
#include <Wire.h>

struct data {
  float A;
  float B;
  char C;
} info {
  10,
  100,
  'd'
};






void setup()
{
  Wire.begin();
}






void loop()
{
  // changing values
  info.A += 0.01;
  info.B += 0.1;
  
  // transmiting the array as bytes
  Wire.beginTransmission(4); 
  Wire.write((byte*)&info, sizeof(info));
  Wire.endTransmission();   
  delay(10);
}
