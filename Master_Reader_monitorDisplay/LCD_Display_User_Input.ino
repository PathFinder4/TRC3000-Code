/* Written by Michelle Lee 31576443
 * Last edited by Michelle Lee
 * Edited on 14/09/2021
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

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int iLineNumberTop = 0;
int iLineNumberBottom = 1; 

void setup() 
{
  // Initialise LCD x2
  lcd.begin(16, 2);       
  lcd.clear();
}

void loop() 
{
  // Update Text on LCD Display
  updateLCDDisplay();      
}

void updateLCDDisplay()
{
  delay(2000); 
  lcd.clear(); 
  
  // 1) Pump Thresholds
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd.print("Pumps");
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd.print("Air:  Blood: ");

  // 2) Pressure Sensor Thresholds
  delay(2000);
  lcd.clear();
  
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd.print("Pressure");
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd.print("V:  inF:  Blood: ");

  // 3) Air Detector Threshold
  delay(2000);
  lcd.clear();
  
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd.print("Air Detector");
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd.print("Val: ");

  // 4) Temperature Sensor Threshold
  delay(2000);
  lcd.clear();
  
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd.print("Temperature");
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd.print("Dialysate: ");

  // 5) Urea Dosage Sensor Thresholds
  delay(2000);
  lcd.clear();
  
  // Top Row
  lcd.setCursor(0,iLineNumberTop);
  lcd.print("Urea Dosage");
  
  // Bottom Row 
  lcd.setCursor(0,iLineNumberBottom);
  lcd.print("Val: ");

}
