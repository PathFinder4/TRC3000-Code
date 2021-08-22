/* Written by Dominic Borcena 30572460
 * Last edited 11/08/2021
 * INPUT: No input
 * OUTPUT: Pressure sensor readings
 * PURPOSE: To read the pressure sensor and send as an output when function is called
 */

//initialize variables
const buzzer = 2;
const venous_sensor = A0; //will return a voltage
const arterial_sensor = A1;
const inflow_sensor = A2;
const section_area = 2; // assume cross sectional area of force sensor is 2m^2

void setup() 
{
  pinMode(venous_sensor,INPUT);
  pinMode(arterial_sensor,INPUT);
  pinMode(inflow_sensor,INPUT);
  pinMode(buzzer,OUTPUT);
  Serial.begin(9600);
}

float venous_pressure () {
  float val = analogRead(venous_sensor); //read sensor value
  float force_value = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = forceValue/sectionArea; //gives pressure in kPa
  
  return pressure_value;
 }

float arterialPressure () {
  float val = analogRead(arterial_sensor); //read sensor value
  float forceValue = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
 }

float inflowPressure () {
  float val = analogRead(inflow_sensor); //read sensor value
  float force_value = map(val, 0, 914, 0, 10); //map force to voltage
  float pressure_value = force_value/section_area; //gives pressure in kPa
  
  return pressure_value;
}

void loop()
{
  float venous = venous_pressure();
  float arterial = arterial_pressure();
  float inflow = inflow_pressure();
  Serial.println(venous);
  Serial.println(arterial);
  Serial.println(inflow);
}
