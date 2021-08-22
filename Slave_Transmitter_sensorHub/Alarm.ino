/* Written by Dominic Borcena 30572469
 * Last edited 11/08/2021
 * INPUT: No input
 * OUTPUT: Trigger alarm
 * PURPOSE: To check if any thresholds are crossed and to trigger alarm if so
 */

/* NOT COMPLETED
const float air_detector_lth; //Air detector lower threshold
const float air_detector_hth; //Air detector higher threshold
const float blood_leakage_lth; //Blood leakage lower threshold
const float blood_leakage_hth; //Blood leakage higher threshold
*/
const float venous_pressure_lth = 1; //Venous pressure lower threshold
const float venous_pressure_hth = 4; //Venous pressure higher threshold
const float inflow_pressure_lth = 1; //Inflow pressure lower threshold
const float inflow_pressure_hth = 4; //Inflow pressure higher threshold
const float arterial_pressure_lth = 1; //Arterial pressure lower threshold
const float arterial_pressure_hth = 4; //Arterial pressure higher threshold

void alarm() {

/* NOT COMPLETED
  if (air_detector_lth < air_detector() < air_detector_hth)
  {

  }
  else if (blood_leakage_lth < blood_leakage() < blood_leakage_hth)
  (
    
  }
*/

  delay(5);
  //check all values
  if(venous_pressure_lth > venous || venous_pressure_hth < venous)
  {
    tone(buzzer, 92);
  }
  else if (arterial_pressure_lth > arterial || arterial_pressure_hth < arterial)
  {
    tone(buzzer, 92);
  }
  else if (inflow_pressure_lth > inflow || inflow_pressure_hth < inflow)
  {
    tone(buzzer, 92);
  }
  else
  {
    noTone(buzzer);
  }
  delay(5);
}
 
