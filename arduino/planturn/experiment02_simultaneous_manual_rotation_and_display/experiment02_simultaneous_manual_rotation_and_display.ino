/*
 * Simultaneous Operations (Experiment 2) 
 * Manual Rotation, Periodic Rotation, and Display 
 * Sascha T. Ishikawa
 * October 14, 2022
 * 
 * Rotation stuttered whenever a timed event, i.e., updating the display, occurs on the same timestep
 * Solved by displaying static "ROTATING..." screen while rotating, blocking any re-renders
 */

#include <arduino-timer.h>

// OLED pins
#define OLED_RESET 4

// Stepper motor pins
#define stepPin 2
#define dirPin 3
#define disablePin 4
#define stepsPerRevolution 6400

// Global variables

long stepperInterval = 450000; // 7.5 minutes
long displayInterval = 1000;  // 1 second
long currentTick = stepperInterval/1000;

bool isRotationActive = false;


// For timed intervals
auto timer = timer_create_default();
 
void setup() {  
  
  // Open serial port, set the baud rate to 9600 bps
  Serial.begin(9600); 

  // Set up components
  setupDisplay();
  setupRotaryEncoder();
  setupStepperMotor();

  // Set up timers
  timer.every(stepperInterval, moveStepperMotor);
  timer.every(displayInterval, updateDisplay);
}
 
void loop() {
  // Disable stepper motor between rotations
  digitalWrite(disablePin, !isRotationActive);
  
  // Respond to changes in rotary encoders
  handleRotaryEncoderUpdates();

  // Keep running motor, if needed
  runStepperMotor();

  // Advance timer
  timer.tick();
} 
