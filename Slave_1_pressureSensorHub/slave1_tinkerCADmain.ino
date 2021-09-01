// C++ code
//
#include <Wire.h>
// ----- SETUP DATA STRUCTURES ------
struct Data {
  float venous_pressure_value; //Venous pressure value
  float inflow_pressure_value; //Inflow pressure value
  float arterial_pressure_value; //Arterial pressure value
  
  char C; //keeping this here in case we need to transmit more complicated information accross in the future 
} info {
  10,
  100,
  'd'
};

struct Data sensor_data;

// ------ CONSTANT VARIABLES -----------
const int on_switch_pin = 2; // ON BUTTON PIN - ensures sensors aent running until machine starts
//Initialize Pins
const int buzzer = 6;
const int venous_sensor = A0;
const int arterial_sensor = A1;
const int inflow_sensor = A2;
const int venous_LED = 3;
const int inflow_LED = 5;
const int arterial_LED = 4;

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
  pinMode(buzzer, OUTPUT);
  pinMode(venous_LED, OUTPUT);
  pinMode(inflow_LED, OUTPUT);
  pinMode(arterial_LED, OUTPUT);

  /*
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
  */
}

// ------ ALARM FUNCTION ------
//Initilize sensor threshholds
const float venous_pressure_lth = 1; //Venous pressure lower threshold
const float venous_pressure_hth = 4; //Venous pressure higher threshold
const float inflow_pressure_lth = 1; //Inflow pressure lower threshold
const float inflow_pressure_hth = 4; //Inflow pressure higher threshold
const float arterial_pressure_lth = 1; //Arterial pressure lower threshold
const float arterial_pressure_hth = 4; //Arterial pressure higher threshold

void alarm(struct Data sensor_data) 
{
  //if an alarm was triggered, this will be set to 1 and the buzzer and LED will NOT reset
  int alarm_triggered = 0;
  delay(5);
  //check all values
  if(venous_pressure_lth > sensor_data.venous_pressure_value || venous_pressure_hth < sensor_data.venous_pressure_value)
  {
    //turn on alarm
    tone(buzzer, 92);
    //turn on LED to indicate sensor toggling alarm
    analogWrite(venous_LED, 1023);
    alarm_triggered = 1;
  }
  if (arterial_pressure_lth > sensor_data.arterial_pressure_value || arterial_pressure_hth < sensor_data.arterial_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(arterial_LED, 1023);
    alarm_triggered = 1;
  }
  if (inflow_pressure_lth > sensor_data.inflow_pressure_value || inflow_pressure_hth < sensor_data.inflow_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(inflow_LED, 1023);
    alarm_triggered = 1;
  }
  if (alarm_triggered == 0)
  {
    noTone(buzzer);
    analogWrite(venous_LED, 0);
    analogWrite(arterial_LED, 0);
    analogWrite(inflow_LED, 0);
  }
  delay(5);
}

// ------ PRESSURE FUNCTION ------
float section_area = 2; //assume area of force sensor is 2

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
  // updating values
  sensor_data.venous_pressure_value = venous_pressure();
  sensor_data.inflow_pressure_value = inflow_pressure();
  sensor_data.arterial_pressure_value = arterial_pressure();
  
  alarm(sensor_data); //trigger alarm if fails thershols check
  
  /*
  // transmiting the array as bytes
  Wire.beginTransmission(4); //send to I2C address 4 to as an event to be flagged and read by master
  Wire.write((byte*)&sensor_data, sizeof(sensor_data));
  Wire.endTransmission();
  */
  delay(10);
}
