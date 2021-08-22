/* Written by Dominic Borcena 30572460
 * Last edited 11/08/2021
 * INPUT: No input
 * OUTPUT: Pressure sensor readings
 * PURPOSE: To read the pressure sensor and send as an output when function is called
 */

//initialize variables
const int buzzer = 2;
const int venous_sensor = A0; //will return a voltage
const int arterial_sensor = A1;
const int inflow_sensor = A2;
const int section_area = 2; // assume cross sectional area of force sensor is 2m^2

float venous = 0;
float arterial = 0;
float inflow = 0;

void setup_pressure_sensors() {
  pinMode(venous_sensor,INPUT);
  pinMode(arterial_sensor,INPUT);
  pinMode(inflow_sensor,INPUT);
  pinMode(buzzer,OUTPUT);
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
  venous = venous_pressure();
  arterial = arterial_pressure();
  inflow = inflow_pressure();
}
