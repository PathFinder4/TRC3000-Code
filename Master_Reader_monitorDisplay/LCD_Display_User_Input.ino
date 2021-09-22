/* Written by Michelle Lee 31576443
 * Last edited by Michelle Lee
 * Edited on 22/09/2021
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

#include <Keypad.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 11, 5, 4, 3, 2);

int iLineNumberTop = 0;
int iLineNumberBottom = 1; 

const byte numR = 4; 
const byte numC = 4; 

char keys[numR][numC] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect to Digital Pins of Arduino 
byte pinR[numR] = {12, 10, 9, 8};
byte pinC[numC] = {7, 6, 1, 0}; 

Keypad keypad = Keypad(makeKeymap(keys), pinR, pinC, numR, numC);

// Define Position of Threshold Value on LCD Display
byte thresholdValLength;

void setup() 
{
  // Initialise LCD x2
  lcd.begin(16, 2);       
  lcd.clear();
}

void loop() 
{
  // Take Keypad and save as variable
  
  // Update Text on LCD Display
  updateLCDDisplay();      
}

void updateLCDDisplay()
{
  // User Input on Keypad
  char key = keypad.getKey();
  
  thresholdValLength = 0;
 
  // 1) Pump Thresholds
  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Air Pump");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }
    
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;
  
 while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Blood Pump");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }
  
  // 2) Pressure Sensor Thresholds
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;

  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Venous Pressure");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  } 
  
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;
  
  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("inFlow Pressure");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }
  
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;

  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Blood Pressure");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }
  
  // 3) Air Detector Threshold
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;

  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Air Detector");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }

  // 4) Temperature Sensor Threshold
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;

  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Dialysate Temp");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }

  // 5) Urea Dosage Sensor Thresholds
  delay(2000);
  lcd.clear();
  key = 0;
  thresholdValLength = 0;

  while (key != '#')
  {   
    // Top Row
    lcd.setCursor(0,iLineNumberTop);
    lcd.print("Urea Dosage");
    
    // Bottom Row 
    lcd.setCursor(thresholdValLength,iLineNumberBottom);
    key = keypad.getKey();
    
    if (key != NO_KEY)
    {
      lcd.print(key);
      thresholdValLength++;
    }
  }
  
  delay(2000); 
  lcd.clear();
  key = 0;
  thresholdValLength = 0;
}
