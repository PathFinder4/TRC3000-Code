/* Written by Glen Chen [30572355]
 * Last edited 22/08/21
 * INPUT: 4 Arduino Input pins 
 * OUTPUT: Stepper Motor actuation
 * PURPOSE: To operate blood pump and heparin pump
 * blood pumpto maintain constant pressure of 18kPa
 */


#include <Stepper.h>

// ---- PINS USED ----

void stepper_motor_setup(int pin1, int pin2, int pin3, int pin4) { 
  //function Purpose: Setup stepper motor to be ready for looping
  
  // Setup pins
  const int A = pin1;
  const int Abar = pin2;
  const int B = pin3;
  const int Bbar = pin4;

  pinMode(A, OUTPUT);
  pinMode(Abar, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(Bbar, OUTPUT);

  
  
}

void stepper_motor_step(int pin1, int pin2, int pin3, int pin4, int delay_microseconds) { 
  //function Purpose: Setup stepper motor to be ready for looping
  
  // Setup pins
  const int A = pin1;
  const int Abar = pin2;
  const int B = pin3;
  const int Bbar = pin4;

  digitalWrite(A, HIGH);
  digitalWrite(Abar, LOW);
  digitalWrite(B, HIGH);
  digitalWrite(Bbar, LOW);
  delayMicroseconds(delay_microseconds);

  digitalWrite(A, LOW);
  digitalWrite(Abar, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(Bbar, LOW);
  delayMicroseconds(delay_microseconds);

  digitalWrite(A, LOW);
  digitalWrite(A_bar, HIGH);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, HIGH);
  delayMicroseconds(delay_microseconds);
  digitalWrite(A, HIGH);
  digitalWrite(A_bar, LOW);
  digitalWrite(B, LOW);
  digitalWrite(B_bar, HIGH);
  delayMicroseconds(delay_microseconds);
  
}
