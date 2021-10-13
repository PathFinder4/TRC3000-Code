/* Written by Glen Chen 30572355
 *  Last edited by Dom
 *  Edited on 13/10/2021
 *  
 *  Code to program the slave arduino, transmitter 
 *  Designed to modularise reading of all pressure/flow sensors and triggering of alarm if thresholds are passed
 *  Also designed to control the pumps by using feedback from the pressure sensors
 */

#include <Wire.h>
#include <Keypad.h>
// ----- SETUP DATA STRUCTURES ------
struct Pressure_Data {
  float venous_pressure_value; //Venous pressure value
  float inflow_pressure_value; //Inflow pressure value
  float arterial_pressure_value; //Arterial pressure value
  float flow_value;
};

struct Pressure_Data pressure_sensor_data;

// ------- KEY PAD INITIALIZE ------- 
const byte numR = 4; 
const byte numC = 3; 

char keys[numR][numC] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connect to Digital Pins of Arduino 
byte pinR[numR] = {12, 11, 10, 9};
byte pinC[numC] = {8, 7, 0}; 

Keypad keypad = Keypad(makeKeymap(keys), pinR, pinC, numR, numC);

//Initilize sensor threshholds
float venous_pressure_lth = 0; //Venous pressure lower threshold
float venous_pressure_hth = 0; //Venous pressure higher threshold
float inflow_pressure_lth = 0; //Inflow pressure lower threshold
float inflow_pressure_hth = 0; //Inflow pressure higher threshold
float arterial_pressure_lth = 0; //Arterial pressure lower threshold
float arterial_pressure_hth = 0; //Arterial pressure higher threshold
float flow_hth = 0;
float flow_lth = 0;


// ------ CONSTANT VARIABLES -----------
const int ideal_flow_rate = 400/1000/60/1000; //(m^3/s) -> 300-500mL/min is safe - idk what flow rate sensor units r in but leaving as is
const int margin = 100/1000/60/1000; //how precise we want adjustments to be for the pressure control loop
const int control_loop_delay = 10; //ms delay between each control loop adjustment
const int user_input_delay = 100;

// ------ GLOBAL VARIABLES -----------
int blood_pump_speed = 155; // duty cycle 0 255
int heparin_pump_speed = 10; // duty cycle 0 255

//Initialize Pins
const int buzzer = 6;
const int venous_sensor = A0;
const int arterial_sensor = A1;
const int inflow_sensor = A2;
const int flow_sensor = A3;
const int venous_LED = 2;
const int inflow_LED = 1;
const int arterial_LED = 4;
const int flow_LED = 13;
const int bp_pump_pin = 5;//DC motor (blood Pump) pins (PWM only)
const int hp_pump_pin = 3;//DC motor (heparin pump) pins (PWM only)


void initialize_variables()
{
  // User Input on Keypad
  char key = keypad.getKey();
  
  //initialize hash count
  int hash_count = 0;
  
  //initlialize alarm value pointers
  float *venous_pressure_hth_ptr = &venous_pressure_hth;
  float *venous_pressure_lth_ptr = &venous_pressure_lth;
  float *arterial_pressure_hth_ptr = &arterial_pressure_hth;
  float *arterial_pressure_lth_ptr = &arterial_pressure_lth;
  float *inflow_pressure_hth_ptr = &inflow_pressure_hth;
  float *inflow_pressure_lth_ptr = &inflow_pressure_lth;
  float *flow_hth_ptr = &flow_hth;
  float *flow_lth_ptr = &flow_lth;
  float* val_array[8] = {venous_pressure_hth_ptr, venous_pressure_lth_ptr, arterial_pressure_hth_ptr, arterial_pressure_lth_ptr, inflow_pressure_hth_ptr, inflow_pressure_lth_ptr, flow_hth_ptr, flow_lth_ptr};
  int val_count = 0;
  float val_value = 0;
  String inputString;
  
  //stay in loop until hash has been pressed 12 times
  //allows both initialization loops in ard 1 and ard 3 to complete same time
  while (hash_count < 14)
  {
    key = keypad.getKey();
    if (key) //so that it reads key input
      {
      //obtain keypad input and place into variable
      if (key >= '0' && key <= '9')     // only act on numeric keys 
      {                     
        inputString += key;               // append new character to input string
      } 
      else if (key == '#')        // end input when encountering a #
      {
        if (inputString.length() > 0) 
        {
          hash_count++;
          val_value = inputString.toFloat(); // YOU GOT AN INTEGER NUMBER
          inputString = "";                // clear input
          if (hash_count < 9) //dont update values after 8th hash
          {
            *val_array[val_count] = val_value;// assign input to variable using *val_array
            val_count++;
          }
        }
      }
      else if (key == '*') 
      {
        inputString = "";                 // clear input
      }
    }
    delay(user_input_delay); //delay matches the master cd to ensure that there is no variability in input due to minute LCD delays
  }
}
// ----------------------------------------------

void setup()
{
  // SETUP I2C PROTOCOL
  //Wire.begin();
  
  // -----INSTANTIATE VARIABLES -----------
  bool on_state = false;

  // ---- SETUP DEVICES ------
  //assign pins
  pinMode(venous_sensor,INPUT);
  pinMode(arterial_sensor,INPUT);
  pinMode(inflow_sensor,INPUT);
  pinMode(flow_sensor,INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(venous_LED, OUTPUT);
  pinMode(inflow_LED, OUTPUT);
  pinMode(arterial_LED, OUTPUT);
  pinMode(flow_LED,OUTPUT);
  analogWrite(bp_pump_pin, blood_pump_speed);  // writes the pwm pulse
  analogWrite(hp_pump_pin, heparin_pump_speed); // writes the pwm pulse
  initialize_variables(); //run initialization setup for alarms
//SPI communciation protocol
  Serial.begin(400000); // 400k full sppeed baud rate
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);
}

// ------ ALARM FUNCTION ------
void alarm(struct Pressure_Data pressure_sensor_data) 
{
  //if an alarm was triggered, this will be set to 1 and the buzzer and LED will NOT reset
  int alarm_triggered = 0;
  delay(5);
  //check all values
  if(venous_pressure_lth > pressure_sensor_data.venous_pressure_value || venous_pressure_hth < pressure_sensor_data.venous_pressure_value)
  {
    //turn on alarm
    tone(buzzer, 92);
    //turn on LED to indicate sensor toggling alarm
    analogWrite(venous_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(venous_LED, 0);
  }
  if (arterial_pressure_lth > pressure_sensor_data.arterial_pressure_value || arterial_pressure_hth < pressure_sensor_data.arterial_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(arterial_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(arterial_LED, 0);
  }
  if (inflow_pressure_lth > pressure_sensor_data.inflow_pressure_value || inflow_pressure_hth < pressure_sensor_data.inflow_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(inflow_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(inflow_LED, 0);  
  }
  if (flow_lth > pressure_sensor_data.flow_value || flow_hth < pressure_sensor_data.flow_value)
  {
    tone(buzzer, 92);
    analogWrite(flow_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(flow_LED, 0);  
  }
  if (alarm_triggered == 0)
  {
    noTone(buzzer);
  }
  delay(5);
}

// ------ PRESSURE FUNCTION ------
float section_area = 2; //assume area of force sensor is 2

float venous_pressure () {
  int val = analogRead(venous_sensor); //read sensor value
  int force_value_int = map(val, 0, 914, 0, 10); //map force to voltage
  float force_value = float(force_value_int);
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
 }

float arterial_pressure () {
  int val = analogRead(arterial_sensor); //read sensor value
  int force_value_int = map(val, 0, 914, 0, 10); //map force to voltage
  float force_value = float(force_value_int);
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
 }

float inflow_pressure () {
  int val = analogRead(inflow_sensor); //read sensor value
  int force_value_int = map(val, 0, 914, 0, 10); //map force to voltage
  float force_value = float(force_value_int);
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
}

// ------ FLOW FUNCTION ------
float flow() {
  int val = analogRead(flow_sensor); //read sensor value
  int force_value_int = map(val, 0, 914, 0, 10); //map force to voltage
  float force_value = float(force_value_int);
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  //initalize constants to calculate flow from pressure
  const float cd = 1; //discharge ratio = A1/A2. depends on orifice from sensor.
  const float rho = 1000; //fluid density of blood
  const float d2 = 1; //orifice plate - depends on flow sensor being used
  const float d1 = 1; //diameter of tube
  const float d = d2/d1;
  const float pi = 3.14;
  const float two = 2;
  const float four = 4;
  const float one = 1;
  //calculate flow (m3/s)
  float sqrt_value = (two*(pressure_value)/(rho*one-pow(d,four)));
  float flow_value = cd*(pi/four)*pow(d2,two)*sqrt(sqrt_value);
  return flow_value;
 }

void motor_loop(struct Pressure_Data pressure_sensor_data){ 
  // ---- feedback control for motor control ---
  //emergency compensation
  if(blood_pump_speed>0 &&pressure_sensor_data.flow_value>ideal_flow_rate+margin){
    blood_pump_speed=0;  
  }
  else if(blood_pump_speed<255 && pressure_sensor_data.flow_value<ideal_flow_rate-margin ){
    blood_pump_speed=255;
  }
  //minute adjustments
  else if(blood_pump_speed>0 &&pressure_sensor_data.flow_value>ideal_flow_rate){
    blood_pump_speed--;  
  }
  else if(blood_pump_speed<255 && pressure_sensor_data.flow_value<ideal_flow_rate ){
    blood_pump_speed++;
  }

  //update pump allocated duty cycle on pins to update bp pump speeds
  analogWrite(bp_pump_pin, blood_pump_speed);
}

void loop()
{
  // updating values
  pressure_sensor_data.venous_pressure_value = venous_pressure();
  pressure_sensor_data.inflow_pressure_value = inflow_pressure();
  pressure_sensor_data.arterial_pressure_value = arterial_pressure();
  pressure_sensor_data.flow_value = flow();
  
  alarm(pressure_sensor_data); //trigger alarm if fails thershols check
  
  motor_loop(pressure_sensor_data); //run control loop for motors
  
  delay(control_loop_delay); //delay on the control loop exageratted due to tinkerCAD limitations
}

void requestEvent(){ //run the upload on request
  Wire.write((byte*)&pressure_sensor_data, sizeof(pressure_sensor_data)); 
}
