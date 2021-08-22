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
const int blood_pump_speed = 2; // pump in delay in microseconds -> should be converted to pressure calculations
const int heparin_pump_speed = 10; // pump in delay in microseconds -> should be converted to pressure calculations 


//stepper motor (blood Pump) pins
const int bp_stepper_pin1 = 6
const int bp_stepper_pin2 = 7
const int bp_stepper_pin3 = 8
const int bp_stepper_pin4 = 9

//stepper motor (heparin pump) pins
const int hp_stepper_pin1 = 10
const int hp_stepper_pin2 = 11
const int hp_stepper_pin3 = 12
const int hp_stepper_pin4 = 13

void setup()
{
  //SETUP I2C PROTOCOL
  Wire.begin(4); // begin I2Cprotocol with arbitrary device 4
  Wire.onReceive(receiveEvent); // changes update paramater to recieve info from slave as soon as the event request is made from the slave
  Serial.begin(400000); // 400k full sppeed baud rate

  // -----INSTANTIATE VARIABLES -----------
  struct Data sensor_data; //instantiate struct to hold data

  // ---- SETUP DEVICES ----
  stepper_motors_setup(bp_stepper_pin1, bp_stepper_pin2, bp_stepper_pin3, bp_stepper_pin4);
  stepper_motors_setup(hp_stepper_pin1, hp_stepper_pin2, hp_stepper_pin3, hp_stepper_pin4);
  
  //---- BUFFER LOOP ------
  //to run until sensors should start being run 
  while(on_state == false)
  {
    if (on_switch_pin == HIGH) {
    // turn LED on:
    on_state = true
  } else 
  {
    // turn LED off:
    on_state = false
  }
  }
}


void loop()
{
  stepper_motors_step(bp_stepper_pin1, bp_stepper_pin2, bp_stepper_pin3, bp_stepper_pin4, blood_pump_speed);
  stepper_motors_step(hp_stepper_pin1, hp_stepper_pin2, hp_stepper_pin3, hp_stepper_pin4, heparin_pump_speed);
  delay(arbitrary_delay); // need to change this value to change the step speed
}


void receiveEvent(int howMany)
{ 
  // receiving data as bytes and storing it at the address of 'info'
  Wire.readBytes((byte*) &sensor_data, sizeof(sensor_data));

  // ---- UPDATE MONTIOR CODE HERE -----
  

}
