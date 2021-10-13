/* Written by Glen Chen 30572355
 *  Last edited by glen chen
 *  Edited on 22/08/2021
 *  
 *  Code to program the master ardiuno
 *  managing the blood pumps and l electrical systems integral to the performance of the dialysis machine
 */

#include <Wire.h>


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
const int on_switch_pin = 2; // ON BUTTON PIN - ensures machine doesn't run until the doctor is ready and wants to start the dialysis process
const int ideal_bp_value = 9999; // idk what units or value to use rn but here's the var
const int margin = 1; //how precise we want adjustments to be for the pressure control loop
const int control_loop_delay = 10; //ms delay between each control loop adjustment

// ------ GLOBAL VARIABLES -----------
int blood_pump_speed = 165; // duty cycle 0 255
int heparin_pump_speed = 10; // duty cycle 0 255
bool on_state = false;
struct Data sensor_data; //instantiate struct to hold data


//DC motor (blood Pump) pins (PWM only)
const int bp_pump_pin = 6;

//DC motor (heparin pump) pins (PWM only)
const int hp_pump_pin = 10;


void setup()
{
  //SETUP I2C PROTOCOL
  Wire.begin(4); // begin I2Cprotocol with arbitrary device 4
  Wire.onReceive(receiveEvent); // changes update paramater to recieve info from slave as soon as the event request is made from the slave
  Serial.begin(400000); // 400k full sppeed baud rate
  

  // ---- SETUP DEVICES ----
  analogWrite(bp_pump_pin, blood_pump_speed);
  analogWrite(hp_pump_pin, heparin_pump_speed);
  
  //---- BUFFER LOOP ------
  //to run until sensors should start being run 
  while(on_state == false)
  {
    if (on_switch_pin == HIGH) {
    // turn LED on:
    on_state = true;
  } else 
  {
    // turn LED off:
    on_state = false;
  }
  }
}


void loop()
{
  // feedback control for motor control
  if(sensor_data.venous_pressure_value>ideal_bp_value+margin){
    blood_pump_speed--;  
  }
  else if(blood_pump_speed<255 && sensor_data.venous_pressure_value>ideal_bp_value-margin ){
    blood_pump_speed++;
  }

  //update pump allocated duty cycle on pins to update bp pump speeds
  analogWrite(bp_pump_pin, blood_pump_speed);

  
  delay(control_loop_delay); // need to change this value to change the step speed
  Serial.print(sensor_data.venous_pressure_value);
}


void receiveEvent(int howMany)
{ 
  // receiving data as bytes and storing it at the address of 'info'
  Wire.readBytes((byte*) &sensor_data, sizeof(sensor_data));

  // ---- UPDATE MONTIOR CODE HERE -----
  

}
