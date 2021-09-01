/* Written by Glen Chen 30572355
 * Last edited 1/09/21
 * INPUT: single digital pin
 * OUTPUT: single analog pin reading and digital pin to check if transmitter is emitting
 * PURPOSE: To read air bubble sensor to see if there are air bubbles or not using ultrasonic measurements
 */

// initialise pin for piezo transmitter
const int ultrasonic_transmitter = 9; //fix to pin no. 9

//initialise pin for detecting if the 555 chip cycle is on
const int air_detector_pulse_on;

//initialise pin for piezo reciever
const int ultrasonic_reciever = A1;


//specify pin ouput vs input for arduino
void setup_air_detector_sensors()
{
  pinMode(ultrasonic_transmitter, OUTPUT);
  pinMode(ultrasonic_receiver, INPUT);
  pinMode(air_detector_pulse_on, INPUT); 
}



void air_detector(){
  //designed to read input of sensor value ONLY IF the 555 chip cycle is pulsing at the time of reading
  if(air_detector_pulse_on == HIGH){
    float val = analogRead(inflow_sensor); //read sensor value
    float air_detector = map(val, 0, 914, 0, 10); //map air value to voltage
  }
}
