/* Written by Michelle Lee 31576443
 * Last edited by Glen Chen 30572355
 * Edited on 13/10/2021
 *
 * Code to program the LCD Display for User Input
 * INPUT: User Input
 * OUTPUT: Text and Numbers (Data) 
 *
 * Note: 
 * This LCD Display is to replicate a Display Monitor. 
 * 
 * Order of Display:
 * 1) Pump Thresholds
 * 2) Pressure Sensor Thresholds
 * 3) Air Detector Threshold
 * 4) Temperature Sensor for Dialysate Threshold
 * 5) Urea Dosage Threshold
 *
 */

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>



// ------------ LCD setup -----------
const int lcd2_pin = A2;
const int lcd3_pin = A3;
LiquidCrystal lcd(13, 11, 6, 5, 4, 3);
LiquidCrystal lcd2(13, A2, 6, 5, 4, 3);
LiquidCrystal lcd3(13, A3, 6, 5, 4, 3);
LiquidCrystal lcd4(13, 10, 6, 5, 4, 3);

int iLineNumberTop = 0;
int iLineNumberBottom = 1; 

const byte numR = 4; 
const byte numC = 3; 

char keys[numR][numC] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

// Connect to Digital Pins of Arduino 
byte pinR[numR] = {12, 9, 8, 7};
byte pinC[numC] = {2, 1, 0}; // last col removed as unused 

Keypad keypad = Keypad(makeKeymap(keys), pinR, pinC, numR, numC);

// Define Position of Threshold Value on LCD Display
byte thresholdValLength;

// ----- SETUP DATA STRUCTURES ------
struct Pressure_Data {
  float venous_pressure_value; //Venous pressure value
  float inflow_pressure_value; //Inflow pressure value
  float arterial_pressure_value; //Arterial pressure value
  float flow_value;
};

struct Cluster_Data {
  float blood_leakage_value; //Venous pressure value
  float body_temp_value; //Inflow pressure value
  float dialysate_temp_value; //Arterial pressure value
  float urea_sensor_value;
};

//Initilize sensor threshholds
float venous_pressure_lth = 0; //Venous pressure lower threshold
float venous_pressure_hth = 0; //Venous pressure higher threshold
float inflow_pressure_lth = 0; //Inflow pressure lower threshold
float inflow_pressure_hth = 0; //Inflow pressure higher threshold
float arterial_pressure_lth = 0; //Arterial pressure lower threshold
float arterial_pressure_hth = 0; //Arterial pressure higher threshold
float flow_hth = 0;
float flow_lth = 0;
float* pressure_val_array[8] = {
  &venous_pressure_hth, &venous_pressure_lth, 
  &arterial_pressure_hth, &arterial_pressure_lth, 
  &inflow_pressure_hth, &inflow_pressure_lth, 
  &flow_hth, &flow_lth
};

//Initilize sensor threshholds
float body_temp_lth = 0;
float body_temp_hth = 0;
float dialysate_temp_lth = 0;
float dialysate_temp_hth = 0;
float urea_lth = 0;
float urea_hth = 0;
float* cluster_val_array[6] = {
  &body_temp_hth, &body_temp_lth, 
  &dialysate_temp_hth, &dialysate_temp_lth, 
  &urea_hth, &urea_lth
};
  

// ------ CONSTANT VARIABLES -----------
const int on_switch_pin = A1; // ON BUTTON PIN - ensures machine doesn't run until the doctor is ready and wants to start the dialysis process
//const int LCD_brightness_pin = A2; // out of 255
//const int LCD_brightness = 255; // out of 255
const int user_input_delay = 100; // delay between safety threshold inputs
const int RXPIN = 0;
const int TXPIN = 1;


// ------ GLOBAL VARIABLES -----------
bool on_state = false;
struct Pressure_Data pressure_sensor_data;
struct Cluster_Data cluster_sensor_data;


void setup()
{
  

  // ---- SETUP DEVICES ----
  pinMode(on_switch_pin, INPUT_PULLUP);// converts an analog to work similarly to a digital with a pull up resistor
  lcd.begin(16, 2);   // initialise lcd    
  lcd.clear(); // clear the display from the current pin iformation
  lcd2.begin(16, 2);   // initialise lcd    
  lcd2.clear(); // clear the display from the current pin iformation 
  lcd3.begin(16, 2);   // initialise lcd    
  lcd3.clear(); // clear the display from the current pin iformation 
  lcd4.begin(16, 2);   // initialise lcd    
  lcd4.clear(); // clear the display from the current pin iformation 
  //digitalWrite(LCD_brightness_pin,LCD_brightness);
  //initialiseSafetyThresholds(); // --- Initialise Safety Trhesholds off User Input ---

  
  //SETUP I2C PROTOCOL
  // occurs after device setup to allow keypad use for pin 0 / 1 as GPIO pins
  Wire.begin(); // begin I2Cprotocol with arbitrary device 4
  Serial.begin(400000); // 400k full sppeed baud rate
  //SoftwareSerial mySerial(RXPIN, TXPIN,1); // RX, TX, inverted
  
}


void loop()
{ 
  //---- BUFFER LOOP ------
  on_state_checker();
  

  // --- SPI Request ---
  Wire.requestFrom(8, sizeof(pressure_sensor_data));    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    Wire.readBytes((byte*) &pressure_sensor_data, sizeof(pressure_sensor_data));
  }
  
  Wire.requestFrom(9, sizeof(cluster_sensor_data));    // request 6 bytes from slave device #8

  while (Wire.available()) { // slave may send less than requested
    Wire.readBytes((byte*) &cluster_sensor_data, sizeof(cluster_sensor_data));
  }

  // ---- LCD Display Update ----
  updateLCDDisplay(); 

}



void on_state_checker(){
  // while on state false check for reset
    if (digitalRead(on_switch_pin) == HIGH) {
      on_state = true;
    }else{
      on_state = false; // reset on_state
    }
  
  while(on_state == false)
  {
    // while on state false check for reset
    if (digitalRead(on_switch_pin) == HIGH) {
    // turn LED on:
    on_state = true;
    }else{
      //perform secondary actions required after turning off power
      
      //turn off display
    }
    
  } 
}

void initialiseSensorThreshold(char *print_string, float threshold_value){
  
  // User Input on Keypad
  char key = keypad.getKey();
  
  int thresholdValLength = 0;
  String thresholdValString = "";
 
  // 1) Pump Thresholds
  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print(print_string);
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY & key!= '*')
    {
      thresholdValString = thresholdValString + key;
      lcd.print(key);
      thresholdValLength++;
    }
  }
  
  threshold_value = thresholdValString.toFloat();
  delay(user_input_delay);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;
  
}

void initialiseSafetyThresholds()
{
  const int no_pressure_sensors = 8;
  char *PRESSURE_SENSORS[no_pressure_sensors] = {
    "Venous H (Pa):","Venous L (Pa):",
    "Arterial H (Pa):","Arterial L (Pa):",
    "Inflow H (Pa):","Inflow L (Pa):",
    "Flow H (m^3/s):", "Flow L (m^3/s):"
  };
  
  //initialise Pressure Sensors
  int initialisation_count = 0;
  while(initialisation_count < no_pressure_sensors){
  initialiseSensorThreshold(PRESSURE_SENSORS[initialisation_count], *pressure_val_array[initialisation_count])   ;
    initialisation_count++;
  }
    
  //initialise cluster Sensors
  const int no_cluster_sensors = 6;
  char *CLUSTER_SENSORS[no_cluster_sensors] = {
    "Body Temp H (C):", "Body Temp (C):",
    "Dial Temp H (C):", "Dial Temp H (C):",
    "Urea H:", "Urea L:"
  };
  //initialise Pressure Sensors
  initialisation_count = 0;
  while(initialisation_count < no_cluster_sensors){
  initialiseSensorThreshold(PRESSURE_SENSORS[initialisation_count], *cluster_val_array[initialisation_count]) ;  
    initialisation_count++;
  }
}

bool safety_check(float current_val, float safety_lth, float safety_hth)
{
  if( current_val > safety_lth & current_val<safety_hth){
    return true;
  }
  else{
    return false;
  }
}
void updateLCDDisplay()
{
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd2.setCursor(0,iLineNumberTop);
  lcd3.setCursor(0,iLineNumberTop);
  lcd4.setCursor(0,iLineNumberTop);
  
  // String Concatenation
  String lcd1String1 = String("Body:  Dialysate:");
  String lcd1String2 = String("C ");
  String lcd1StringTotal = String(cluster_sensor_data.body_temp_value) + (char)178 + lcd1String2 + String(cluster_sensor_data.dialysate_temp_value) + (char)178 + lcd1String2;;
  lcd.print(lcd1String1);
  
  String lcd2String1 = String("Blood Flow Rate:");
  String lcd2String2 = String("m3/s");
  String lcd2StringTotal = String(pressure_sensor_data.flow_value) + lcd2String2;
  lcd2.print(lcd2String1);
  
  String lcd3String1 = String("Venous: Arterial");
  String lcd3String2 = String("Pa  ");
  String lcd3StringTotal = String(pressure_sensor_data.venous_pressure_value) + lcd3String2 + String(pressure_sensor_data.arterial_pressure_value) + lcd3String2;
  lcd3.print(lcd3String1);

  String lcd4String1 = String("Inflow: Urea:");
  String lcd4String2 = String("Pa ");
  String lcd4String3 = String("mol/L");
  String lcd4StringTotal = String(pressure_sensor_data.inflow_pressure_value) + lcd4String2 + String(cluster_sensor_data.urea_sensor_value) + lcd4String3;
  lcd4.print(lcd4String1);
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd2.setCursor(0,iLineNumberBottom);  
  lcd3.setCursor(0,iLineNumberBottom);  
  lcd4.setCursor(0,iLineNumberBottom);  
  
  lcd.print(lcd1StringTotal);
  lcd2.print(lcd2StringTotal);
  if(safety_check(pressure_sensor_data.flow_value, flow_lth, flow_hth)){
    lcd2.print(" Normal ");
  }
  else{
    lcd2.print(" Failure ");
  }
  lcd3.print(lcd3StringTotal);
  lcd4.print(lcd4StringTotal);
}
  
