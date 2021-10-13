/* Written by Glen Chen 30572355
 *  Last edited by Dom
 *  Edited on 13/10/2021
 *  
 *  Code to program the slave arduino, transmitter 
 *  Designed to modularise reading of all miscellaneous safety sensors in the control cluster and triggering of alarm if thresholds are passed
 */

#include <Wire.h>
#include <Keypad.h>

// ------ CONSTANT VARIABLES -----------
//const int on_switch_pin = 2; // ON BUTTON PIN - ensures sensors aent running until machine starts
const int user_input_delay = 100;

// ----- SETUP DATA STRUCTURES ------
struct Cluster_Data {
  float blood_leakage_value; //Venous pressure value
  float body_temp_value; //Inflow pressure value
  float dialysate_temp_value; //Arterial pressure value
  float urea_sensor_value;
};

struct Cluster_Data cluster_sensor_data;

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
byte pinC[numC] = {3, 1, 0}; 

Keypad keypad = Keypad(makeKeymap(keys), pinR, pinC, numR, numC);

//Initilize sensor threshholds
float body_temp_lth = 0;
float body_temp_hth = 0;
float dialysate_temp_lth = 0;
float dialysate_temp_hth = 0;
float urea_lth = 0;
float urea_hth = 0;
float blood_leakage_minimum = 1; //doesnt require change of threshhold
float air_bubble_minimum = 1;

void initialize_variables()
{
  // User Input on Keypad
  char key = keypad.getKey();
  
  //initialize hash count
  int hash_count = 0;
  
  //initlialize alarm value pointers
  float *body_temp_hth_ptr = &body_temp_hth;
  float *body_temp_lth_ptr = &body_temp_lth;
  float *dialysate_temp_hth_ptr = &dialysate_temp_hth;
  float *dialysate_temp_lth_ptr = &dialysate_temp_lth;
  float *urea_hth_ptr = &urea_hth;
  float *urea_lth_ptr = &urea_lth;
  
  float* val_array[6] = {body_temp_hth_ptr, body_temp_lth_ptr, dialysate_temp_hth_ptr, dialysate_temp_lth_ptr, urea_hth_ptr, urea_lth_ptr};
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
          inputString = "";                  // clear input
          if (hash_count > 8) //dont update values for this arduino until Arduino 1 is completed
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

//Initialize Pins
const int buzzer = 7;
const int dialysate_LED = 2;
const int body_temp_LED = 8;
const int blood_leak_LED = 4;
const int air_bubble_LED = 5;
const int urea_LED = 6;
const int body_temp_sensor_pin = A0; // Temperature Sensor Pin
const int dialysate_temp_sensor_pin = A1; // Temperature Sensor Pin 
const int blood_leakage_pin = A2;
const int urea_sensor_pin = A3;

void setup()
{
  // ---- SETUP DEVICES ------
  //assign pins
  pinMode(body_temp_sensor_pin,INPUT);
  pinMode(dialysate_temp_sensor_pin,INPUT);
  pinMode(blood_leakage_pin,INPUT);
  pinMode(urea_sensor_pin, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(dialysate_LED, OUTPUT);
  pinMode(body_temp_LED, OUTPUT);
  pinMode(blood_leak_LED, OUTPUT);
  pinMode(air_bubble_LED, INPUT);
  pinMode(urea_LED, OUTPUT);
  initialize_variables();
  //SPI communciation protocol
  Serial.begin(400000); // 400k full sppeed baud rate
  Wire.begin(9);                // join i2c bus with address #8
  Wire.onRequest(requestEvent);
}

// ------ TEMPERATURE SENSOR FUNCTION -----
// Body Temperature Sensor
int body_temp_sensor_input; // Temperature Sensor Input
float body_temp_celsius; // Temperature in Celsius (Output)
float body_temp_sensor_voltage; // Temperature Sensor Input Voltage 

// Dialysate Temperature Sensor 
int dialysate_temp_sensor_input; // Temperature Sensor Input
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

// ------ BLOOD LEAKAGE FUNCTION -----
float blood_leakage_sensor()
{
  return analogRead(blood_leakage_pin);
}

// ------ UREA SENSOR FUNCTION ------
float urea_sensor()
{
  float urea_value = analogRead(urea_sensor_pin);
  urea_value = map(urea_value, 306, 750, 0, 100); //map values *CHANGE LATER?*
  return urea_value;
}

// ------ ALARM FUNCTION ------
void alarm(struct Cluster_Data cluster_sensor_data) 
{
  //if an alarm was triggered, this will be set to 1 and the buzzer and LED will NOT reset
  int alarm_triggered = 0;
  delay(5);
  //check all values
  if(body_temp_lth > cluster_sensor_data.body_temp_value || body_temp_hth < cluster_sensor_data.body_temp_value)
  {
    //turn on alarm
    tone(buzzer, 92);
    //turn on LED to indicate sensor toggling alarm
    analogWrite(body_temp_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(body_temp_LED, 0);
  }
  if (dialysate_temp_lth > cluster_sensor_data.dialysate_temp_value || dialysate_temp_hth < cluster_sensor_data.dialysate_temp_value)
  {
    tone(buzzer, 92);
    analogWrite(dialysate_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(dialysate_LED, 0);
  }
  if (cluster_sensor_data.blood_leakage_value > blood_leakage_minimum)
  {
    tone(buzzer, 92);
    analogWrite(blood_leak_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(blood_leak_LED, 0);  
  }
  if(urea_lth > cluster_sensor_data.urea_sensor_value || urea_hth < cluster_sensor_data.urea_sensor_value)
  {
    //turn on alarm
    tone(buzzer, 92);
    //turn on LED to indicate sensor toggling alarm
    analogWrite(urea_LED, 1023);
    alarm_triggered = 1;
  }
  else
  {
    analogWrite(urea_LED, 0);
  }
  if (alarm_triggered == 0)
  {
    noTone(buzzer);
  }
  delay(5);
}

void loop()
{
  // updating values
  cluster_sensor_data.blood_leakage_value = blood_leakage_sensor();
  cluster_sensor_data.body_temp_value = body_temp_sensor();
  cluster_sensor_data.dialysate_temp_value = dialysate_temp_sensor();
  cluster_sensor_data.urea_sensor_value = urea_sensor();
  alarm(cluster_sensor_data);
  
  delay(10);
}

void requestEvent(){
  Wire.write((byte*)&cluster_sensor_data, sizeof(cluster_sensor_data));
  
}
