/* Receives messages from sender
 *  
 * Send the byte data via the data address from sender via I2C
 * Store byte data into the new address in the receiver
 * 
 * Meng Lin - 11/08/2021
 */

#include <Wire.h>

float info[3];






void setup()
{
  Wire.begin(4);                
  Wire.onReceive(receiveEvent); 
  Serial.begin(400000);
}








void loop()
{
  delay(100);
}








void receiveEvent(int howMany)
{ 
  // receiving data as bytes and storing it at the address of 'info'
  Wire.readBytes((byte*) &info, sizeof(info));
  
  // printing values
  Serial.println(info[0]);
  Serial.println(info[1]);
  Serial.println(info[2]);
  Serial.println();
}
