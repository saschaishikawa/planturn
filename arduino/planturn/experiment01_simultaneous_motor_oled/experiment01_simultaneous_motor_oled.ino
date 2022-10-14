/*
 * Simultaneous Operations (Experiment 1)
 * 
 * Updates:
 * ---
 * 
 * - Map 360 degrees to total steps per revolutio in the rotary encoder (6400 steps using 16th microstep configuration)
 * - Set up correct timing for 8 revolutions during a 24-hour period using 15-degree "steps" every 7.5 minutes
 * - Display countdown timer between steps
 * 
 * Notes:
 * ---
 * 
 * Potential issues when rotating and updating display as rotation gets interrupted by the displayInterval
 * 
 */

#include <AccelStepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino-timer.h>

// Stepper motor pins
#define stepPin 2
#define dirPin 5
#define stepsPerRevolution 6400

// OLED pins
#define OLED_RESET 4

// Instantiate OLED display
Adafruit_SSD1306 display(OLED_RESET);

long stepperInterval = 450000; // 7.5 minutes
long displayInterval = 1000;  // 1 second
long currentTick = stepperInterval / 1000;

// Define the stepper motor and the pins that is connected to
AccelStepper stepper(1, stepPin, dirPin); // (Typeof driver: with 2 pins, STEP, DIR)
int degPerMove = 15;

// For timed intervals
auto timer = timer_create_default();

void setup() {
  // Set up stepper motor
  stepper.setMaxSpeed(2000); // Set maximum speed value for the stepper
  stepper.setAcceleration(500); // Set acceleration value for the stepper
  stepper.setCurrentPosition(0); // Set the current position to 0 steps

  // Set up OLED
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();

  // Set up timers
  timer.every(displayInterval, updateDisplay);
  timer.every(stepperInterval, moveStepperMotor);
}

bool moveStepperMotor(void *) {
  Serial.println("Tick.");
  stepper.move(map(degPerMove, 1, 360, 1, stepsPerRevolution));
  return true;
}

bool updateDisplay(void *) {
  if (currentTick == 1) {
    currentTick = stepperInterval/1000;
  }

  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(56, 10);
  display.clearDisplay();
  display.println(currentTick);
  display.display();
  
  Serial.println(currentTick);
  currentTick--;
  return true;
}

void loop() {
  stepper.run();
  timer.tick();
}
