// C++ code
//
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
      if (key >= '0' && key <= '9') 		// only act on numeric keys	
      {     								
        inputString += key;               // append new character to input string
      } 
      else if (key == '#') 				// end input when encountering a #
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
  }
}
// ----------------------------------------------

// ------ CONSTANT VARIABLES -----------
const int on_switch_pin = 2; // ON BUTTON PIN - ensures sensors aent running until machine starts
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
  initialize_variables();	//run initialization setup for alarms

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

void loop()
{
  // updating values
  pressure_sensor_data.venous_pressure_value = venous_pressure();
  pressure_sensor_data.inflow_pressure_value = inflow_pressure();
  pressure_sensor_data.arterial_pressure_value = arterial_pressure();
  pressure_sensor_data.flow_value = flow();
  
  alarm(pressure_sensor_data); //trigger alarm if fails thershols check
  
  // transmiting the array as bytes
  Wire.beginTransmission(4); //send to I2C address 4 to as an event to be flagged and read by master
  Wire.write((byte*)&pressure_sensor_data, sizeof(pressure_sensor_data));
  Wire.endTransmission();
  
  delay(1000);
}
