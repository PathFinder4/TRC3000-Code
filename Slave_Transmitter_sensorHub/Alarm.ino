/* Written by Dominic Borcena 30572469
 * Last edited 11/08/2021
 * INPUT: No input
 * OUTPUT: Trigger alarm
 * PURPOSE: To check if any thresholds are crossed and to trigger alarm if so
 */

float air_detector_lth; //Air detector lower threshold
float air_detector_hth; //Air detector higher threshold
float blood_leakage_lth; //Blood leakage lower threshold
float blood_leakage_hth; //Blood leakage higher threshold
float venous_pressure_lth; //Venous pressure lower threshold
float venous_pressure_hth; //Venous pressure higher threshold
float inflow_pressure_lth; //Inflow pressure lower threshold
float inflow_pressure_hth; //Inflow pressure higher threshold
float arterial_pressure_lth; //Arterial pressure lower threshold
float arterial_pressure_hth; //Arterial pressure higher threshold

void alarm() {
  if (air_detector_lth < air_detector() < air_detector_hth)
  {

  }
  else if (blood_leakage_lth < blood_leakage() < blood_leakage_hth)
  (
    
  }
  else if (venous_pressure_lth < venous_pressure() < venous_pressure_hth)
  (

  )
  else if (inflow_pressure_lth < inflow_pressure() < inflow_pressure_hth)
  (

  )
  else if (arterial_pressure_lth < arterial_pressure() < arterial_pressure_hth)
  (
    
  )
}
 
