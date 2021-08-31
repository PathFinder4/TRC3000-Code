/* Written by Dominic Borcena 30572469
 * Last edited 31/08/2021
 * INPUT: No input
 * OUTPUT: Trigger alarm
 * PURPOSE: To check if any thresholds are crossed and to trigger alarm if so
 */

//Initilize sensor threshholds
const float venous_pressure_lth = 1; //Venous pressure lower threshold
const float venous_pressure_hth = 4; //Venous pressure higher threshold
const float inflow_pressure_lth = 1; //Inflow pressure lower threshold
const float inflow_pressure_hth = 4; //Inflow pressure higher threshold
const float arterial_pressure_lth = 1; //Arterial pressure lower threshold
const float arterial_pressure_hth = 4; //Arterial pressure higher threshold
const float air_detector_lth = 1; //Air detector lower threshold
const float air_detector_hth = 4; //Air detector higher threshold
const float blood_leakage_lth = 1; //Blood leakage lower threshold
const float blood_leakage_hth = 4; //Blood leakage higher threshold

void alarm() 
{
  //if an alarm was triggered, this will be set to 1 and the buzzer and LED will NOT reset
  int alarm_triggered = 0;
  delay(5);
  //check all values
  if(venous_pressure_lth > venous_pressure_value || venous_pressure_hth < venous_pressure_value)
  {
    //turn on alarm
    tone(buzzer, 92);
    //turn on LED to indicate sensor toggling alarm
    analogWrite(venous_LED, 1023);
    alarm_triggered = 1;
  }
  if (arterial_pressure_lth > arterial_pressure_value || arterial_pressure_hth < arterial_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(arterial_LED, 1023);
    alarm_triggered = 1;
  }
  if (inflow_pressure_lth > inflow_pressure_value || inflow_pressure_hth < inflow_pressure_value)
  {
    tone(buzzer, 92);
    analogWrite(inflow_LED, 1023);
    alarm_triggered = 1;
  }
  if (air_detector_lth > air_detector_value || air_detector_hth < air_detector_value)
  {
    tone(buzzer, 92);
    analogWrite(air_detector_LED, 1023);
    alarm_triggered = 1;
  }
  if (blood_leakage_lth > blood_leakage_value || blood_leakage_hth < blood_leakage_value)
  {
    tone(buzzer, 92);
    analogWrite(blood_leak_LED, 1023);
    alarm_triggered = 1;
  }
  if (alarm_triggered == 0)
  {
    noTone(buzzer);
    analogWrite(venous_LED, 0);
    analogWrite(arterial_LED, 0);
    analogWrite(inflow_LED, 0);
    analogWrite(air_detector_LED, 0);
    analogWrite(blood_leak_LED, 0);
  }
  delay(5);
}
