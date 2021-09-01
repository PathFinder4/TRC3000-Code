/* Written by Michelle Lee 31576443
 * Last edited by Michelle Lee
 * Edited on 01/09/2021
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

// Body Temperature Sensor
int body_temp_sensor_input; // Temperature Sensor Input
int body_temp_sensor_pin = A0; // Temperature Sensor Pin 
float body_temp_celsius; // Temperature in Celsius (Output)
float body_temp_sensor_voltage; // Temperature Sensor Input Voltage 

// Dialysate Temperature Sensor 
int dialysate_temp_sensor_input; // Temperature Sensor Input
int dialysate_temp_sensor_pin = A1; // Temperature Sensor Pin 
float dialysate_temp_celsius; // Temperature in Celsius (Output)
float dialysate_temp_sensor_voltage; // Temperature Sensor Input Voltage 

float body_temp_sensor()
{
  // Receive Input
  body_temp_sensor_input = analogRead(body_temp_sensor_pin);
  
  // Convert Input to Voltage
  body_temp_sensor_voltage = (body_temp_sensor_input * 5.0)/1024.0;
  
  // Convert Voltage to Temperature in Celsius
  // 10 mV/Degree with 500 mV offset
  // Formula: Temperature = (Voltage - 500 mV) * 100
  body_temp_celsius = (body_temp_sensor_voltage - 0.5) * 100 ;  
                                               
  return body_temp_celsius;
}

float dialysate_temp_sensor()
{
  // Receive Input
  dialysate_temp_sensor_input = analogRead(dialysate_temp_sensor_pin);
  
  // Convert Input to Voltage
  dialysate_temp_sensor_voltage = (dialysate_temp_sensor_input * 5.0)/1024.0;
  
  // Convert Voltage to Temperature in Celsius
  // 10 mV/Degree with 500 mV offset
  // Formula: Temperature = (Voltage - 500 mV) * 100
  dialysate_temp_celsius = (dialysate_temp_sensor_voltage - 0.5) * 100 ;  
                                               
  return dialysate_temp_celsius;
}
