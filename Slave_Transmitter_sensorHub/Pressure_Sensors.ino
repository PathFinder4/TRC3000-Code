/* Written by Dominic Borcena 30572460
 * Last edited 11/08/2021
 * INPUT: No input
 * OUTPUT: Pressure sensor readings
 * PURPOSE: To read the pressure sensor and send as an output when function is called
 */

//initialize pins for alarm
const int buzzer = 2;
const int venous_sensor = A0; //will return a voltage
const int arterial_sensor = A1;
const int inflow_sensor = A2;
const int blood_sensor = ; 
const int infrared_light = ;
const int venous_LED = ;
const int inflow_LED = ;
const int arterial_LED = ;
const int blood_leak_LED = ;
const int air_detector_LED = ;
const int section_area = 2; // assume cross sectional area of force sensor is 2m^2

//Initialize sensor values
float venous_pressure_value = ;
float inflow_pressure_value = ;
float arterial_pressure_value = ;
float air_detector_value = ;
float blood_leakage_value = ;

void setup_pressure_sensors() {
  pinMode(venous_sensor,INPUT);
  pinMode(arterial_sensor,INPUT);
  pinMode(inflow_sensor,INPUT);
  pinMode(blood_sensor, INPUT);

  pinMode(buzzer,OUTPUT);
  pinMode(infrared_light, OUTPUT);
  Serial.begin(9600);
}

float venous_pressure () {
  float val = analogRead(venous_sensor); //read sensor value
  float force_value = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
 }

float arterial_pressure () {
  float val = analogRead(arterial_sensor); //read sensor value
  float force_value = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
 }

float inflow_pressure () {
  float val = analogRead(inflow_sensor); //read sensor value
  float force_value = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
}

void loop()
{
  venous_pressure_value = venous_pressure();
  arterial_pressure_value = arterial_pressure();
  inflow_pressure_value = inflow_pressure();
}
