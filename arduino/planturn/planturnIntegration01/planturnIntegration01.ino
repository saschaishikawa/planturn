/*
 * Planturn Integration
 * Sascha T. Ishikawa
 * October 16, 2022
 * 
 * Features:
 * - Manual Rotation
 * - Periodic Rotation 
 * - Display
 * - Moisture Sensor 
 */

#include <arduino-timer.h>

// Stepper motor pins
#define stepPin 2
#define dirPin 3
#define disablePin 4
#define stepsPerRevolution 6400

// GLOBAL VARIABLES

long stepperInterval = 450000;           // 7.5 minutes
long displayInterval = 1000;             // 1 second
long moisturePollInterval = 1000;//60000;       // 1 minute
long currentTick = stepperInterval/1000;

int currentMenuIndex = 0;

int moisturePercent = 0;

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
  timer.every(moisturePollInterval, pollMoistureSensor);
}
 
void loop() {
  // Disable stepper motor between rotations
  digitalWrite(disablePin, !isRotationActive);
  
  // Respond to changes in rotary encoders
  handleRotaryEncoderUpdates();

  // Keep running motor, if needed
  runStepperMotor();

  // Has button been pressed?
  checkButtonPress();
  
  // Advance timer
  timer.tick();
} 
