/*
 * Planturn Integration
 * Sascha T. Ishikawa
 * October 24, 2022
 * 
 * Features:
 * - Manual rotation
 * - Periodic rotation 
 * - Display
 * - Moisture sensor 
 * - Light sensor
 * - RGB lighting
 */

#include <arduino-timer.h>

/*
 * Defines the total number of steps per revolution given the current stepper motor configuration
 * Note: depends on how the stepper driver is wired, e.g., are half- or quarter-steps enabled?
 */
#define STEPS_PER_REVOLUTION 6400

/*
 * Set sensor polling intervals
 */
#define LUX_POLL_INTERVAL_MS 1000
#define MOISTURE_POLL_INTERVAL_MS 1000

#define DISPLAY_INTERVAL_MS 1000
#define STEPPER_INTERVAL_MS 30000
#define SERIAL_BAUD_RATE 9600

// GLOBAL VARIABLES

// Set application-wide verbosity
const bool VERBOSE = true;

// Initialize current time interval (in seconds)
long currentTick = STEPPER_INTERVAL_MS/1000;

int degPerMove = 15;                 // Degrees per rotation
int revPerDay = 4;                   // Revolutions per day
int lux;                             // Light measurement; dependents: display, lightSensor, RGB, stepperMotor
int luxThreshold = 150;              // Rotor disabled if lux < luxThreshold; dependents: display, RGB, stepperMotor
int currentMenuIndex = 1;            // Zero-indexed active menu on OLED display
int moisturePercent = 0;             // Initial moisture reading; dependents: display, moistureSensor, RGB
int moistureThreshold = 25;          // Percent moisture under which blue light flashes; dependents: RGB
bool isRotationActive = false;       // Boolean flag that disables motor when false; dependents: display, rotaryEncoder, stepperMotor

// For timed intervals
auto timer = timer_create_default();
 
void setup() {  
  
  // Open serial port, set the baud rate to 9600 bps
  Serial.begin(SERIAL_BAUD_RATE); 

  // Set up components
  setupRGB();            // RGB lighting
  setupRotaryEncoder();  // Rotary encoder and button
  setupStepperMotor();   // Rotor controller
  setupDisplay();        // OLED display

  // Set up timers
  timer.every(STEPPER_INTERVAL_MS, moveStepperMotor);
  timer.every(DISPLAY_INTERVAL_MS, updateDisplay);
  timer.every(MOISTURE_POLL_INTERVAL_MS, pollMoistureSensor);
  timer.every(LUX_POLL_INTERVAL_MS, pollLightSensor);
}
 
void loop() {
  // Handle updates
  updateRGB();                 // Update RGB lighting
  checkRotaryEncoderUpdate();  // Check for changes to manual rotor positioning
  checkButtonPress();          // Check for button presses
  runStepperMotor();           // Run rotor (if position not yet reached)

  // Increment global timer
  timer.tick();
} 
