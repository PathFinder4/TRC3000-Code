/* Written by Michelle Lee 31576443
 * Last edited by Michelle Lee
 * Edited on 22/08/2021
 *
 * Code to program the Temperature Sensor 
 * INPUT: Voltage Reading 
 * OUTPUT: Temperature Reading 
 *
 * Note:
 * Due to the limitations of TinkerCAD, a TMP36 has been used to replicate TMP117 Temperature Sensor
 *
 */

#include <Wire.h>

int temp_sensor_input; // Temperature Sensor Input
int temp_sensor_pin = A0; // Temperature Sensor Pin 
float temp_celsius; // Temperature in Celsius (Output)
float temp_sensor_voltage; // Temperature Sensor Input Voltage 

float temp_sensor()
{
  // Receive Input
  temp_sensor_input = analogRead(temp_sensor_pin);
  
  // Convert Input to Voltage
  temp_sensor_voltage = (temp_sensor_input * 5.0)/1024.0;
  
  // Convert Voltage to Temperature in Celsius
  // 10 mV/Degree with 500 mV offset
  // Formula: Temperature = (Voltage - 500 mV) * 100
  temp_celsius = (temp_sensor_voltage - 0.5) * 100 ;  
                                               
  return temp_celsius;
}
