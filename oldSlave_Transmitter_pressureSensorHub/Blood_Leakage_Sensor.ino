/* Written by [NAME] [STU. NO.]
 * Last edited [DATE]
 * INPUT: No input
 * OUTPUT: Blood Leakage Sensor
 * PURPOSE: To read blood leak sensor and to output information from blood leak sensor
 */

float blood_leakage_sensor ()
{
  return analogRead(blood_sensor);
}
