/*
 * Planturn Integration
 * Sascha T. Ishikawa
 * October 24, 2022
 * 
 * Features:
 * - Manual Rotation
 * - Periodic Rotation 
 * - Display
 * - Moisture Sensor 
 * - RGB lighting
 */

#include <arduino-timer.h>

// Stepper motor pins
#define disablePin 5
#define stepPin 6
#define dirPin 7
#define stepsPerRevolution 6400

// GLOBAL VARIABLES

// Set application-wide verbosity
const bool VERBOSE = true;

int degPerMove = 15;                     // Degrees per rotation
int revPerDay = 4;                       // Revolutions per day
long stepperInterval = 450000;           // 7.5 minutes
long displayInterval = 1000;             // 1 second
long moisturePollInterval = 1000;        // 1 second
long currentTick = stepperInterval/1000;

int currentMenuIndex = 1;

int moisturePercent = 0;

bool isRotationActive = false;

// For timed intervals
auto timer = timer_create_default();
 
void setup() {  
  
  // Open serial port, set the baud rate to 9600 bps
  Serial.begin(9600); 

  // Set up components
  setupRGB();            // RGB lighting
  setupRotaryEncoder();  // Rotary encoder and button
  setupStepperMotor();   // Rotor controller
  setupDisplay();        // OLED display

  // Set up timers
  timer.every(stepperInterval, moveStepperMotor);
  timer.every(displayInterval, updateDisplay);
  timer.every(moisturePollInterval, pollMoistureSensor);
}
 
void loop() {
  // Disable stepper motor between rotations
  digitalWrite(disablePin, !isRotationActive);

  // Handle updates
  updateRGB();                   // Update RGB lighting
  handleRotaryEncoderUpdates();  // Respond to manual rotor positioning
  checkButtonPress();            // Respond to button press
  runStepperMotor();             // Run rotor (if position not yet reached)

  // Advance timer
  timer.tick();
} 
