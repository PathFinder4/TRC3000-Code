/* Written by Glen Chen 30572355
 *  Last edited by glen chen
 *  Edited on 22/08/2021
 *  
 *  Code to program the slave arduino, transmitter 
 *  Designed to modularise reading of all sensors and triggering of alarm if thresholds are passed
 *  
 */

#include <Wire.h>

// ----- SETUP DATA STRUCTURES ------
struct Data {
  float air_detector_value; //Air detector value
  float blood_leakage_value; //Blood leakage value
  float venous_pressure_value; //Venous pressure value
  float inflow_pressure_value; //Inflow pressure value
  float arterial_pressure_value; //Arterial pressure value
  
  char C; //keeping this here in case we need to transmit more complicated information accross in the future 
} info {
  10,
  100,
  'd'
};



// ------ CONSTANT VARIABLES -----------
const int on_switch_pin = 2; // ON BUTTON PIN - ensures sensors aent running until machine starts

void setup()
{
  // SETUP I2C PROTOCOL
  Wire.begin();
  
  // -----INSTANTIATE VARIABLES -----------
  struct Data sensor_data;
  bool on_state = false;

  //---- BUFFER LOOP ------
  //to run until sensors should start being run 
  while(on_state == false)
  {
    if (on_switch_pin == HIGH)
    {
      // turn sensors on:
      on_state = true;
    } else 
    {
      // turn sensors off:
      on_state = false;
    }
  }
  
  

  
}






void loop()
{
  // updating values
  air_detector_value = air_detector();
  blood_leakage_value = blood_leakage();
  venous_pressure_value = venous_pressure();
  inflow_pressure_value = inflow_pressure();
  arterial_pressure_value = arterial_presure();

  while (on_state == true)
  {
    alarm() //trigger alarm if fails thershols check
  }
  
  // transmiting the array as bytes
  Wire.beginTransmission(4); //send to I2C address 4 to as an event to be flagged and read by master
  Wire.write((byte*)&sensor_data, sizeof(sensor_data));
  Wire.endTransmission();   
  delay(10);
}
