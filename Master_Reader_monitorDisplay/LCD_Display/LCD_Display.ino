/* Written by Michelle Lee 31576443
 * Last edited by Michelle Lee
 * Edited on 29/08/2021
 *
 * Code to program the LCD Display 
 * INPUT: Sensor Readings 
 * OUTPUT: Text and Numbers (Data) 
 *
 * Note: 
 * This LCD Display is to replicate a Display Monitor. 
 *
 */

#include <LiquidCrystal.h>
LiquidCrystal lcd1(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd2(12, 10, 2, 3, 4, 5);

int iLineNumberTop = 0;
int iLineNumberBottom = 1; 

void setup() 
{
  // Initialise LCD x2
  lcd1.begin(16, 2);       
  lcd2.begin(16, 2);
  lcd1.clear();
  lcd2.clear();
}

void loop() 
{
  // Update Text on LCD Display
  updateLCDDisplay();      
}

void updateLCDDisplay()
{
  // Top Row
  lcd1.setCursor(0,iLineNumberTop);
  lcd2.setCursor(0,iLineNumberTop);
  
  lcd1.print("Patient Temp: ");
  lcd2.print("Blood Pressure: ");
  
  // Bottom Row 
  lcd1.setCursor(0,iLineNumberBottom);
  lcd2.setCursor(0,iLineNumberBottom);  
    
  lcd1.print(" Normal ");
  lcd2.print(" Normal ");
}
