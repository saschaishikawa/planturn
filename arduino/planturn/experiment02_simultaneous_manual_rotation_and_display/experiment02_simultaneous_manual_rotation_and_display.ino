/*
 * Simultaneous Operations (Experiment 2) 
 * Manual Rotation, Periodic Rotation, and Display 
 * Sascha T. Ishikawa
 * October 14, 2022
 * 
 * Rotation stuttered whenever a timed event, i.e., updating the display, occurs on the same timestep
 * Solved by displaying static "ROTATING..." screen while rotating, blocking any re-renders
 */

#include <AccelStepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <arduino-timer.h>

// Stepper motor pins
#define stepPin 2
#define dirPin 3
#define disablePin 4
#define stepsPerRevolution 6400

// OLED pins
#define OLED_RESET 4

// Instantiate OLED display
Adafruit_SSD1306 display(OLED_RESET);

int pinA = 18; // Connected to CLK on KY-040
int pinB = 19; // Connected to DT on KY-040
int encoderPosCount = 0;
int pinALast;
int aVal;
bool bCW;

long stepperInterval = 450000; // 7.5 minutes
long displayInterval = 1000;  // 1 second
long currentTick = stepperInterval/1000;

int encoderPosition = 0;

bool isRotationActive = false;

// Define the stepper motor and the pins that is connected to
AccelStepper stepper(1, stepPin, dirPin); // (Typeof driver: with 2 pins, STEP, DIR)
int degPerMove = 15;

// For timed intervals
auto timer = timer_create_default();
 
void setup() {  

  // Set up OLED
  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
  
  // Set up rotary encoder
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  pinALast = digitalRead(pinA);
  Serial.begin (9600);

  // Set up stepper motor
  pinMode(disablePin, OUTPUT);
  digitalWrite(disablePin, HIGH);  // Disable stepper motor until needed
  stepper.setMaxSpeed(5000);       // Set maximum speed value for the stepper
  stepper.setAcceleration(5000);   // Set acceleration value for the stepper
  stepper.setCurrentPosition(0);   // Set the current position to 0 steps

  // Set up timers
  timer.every(stepperInterval, moveStepperMotor);
  timer.every(displayInterval, updateDisplay);
}

void displayRotationScreen() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(32, 12);
  display.clearDisplay();
  display.println("ROTATING...");
  display.display();
  return; 
}

bool moveStepperMotor(void *) {
  if (!isRotationActive) {
    displayRotationScreen();
    isRotationActive = true; // Block re-renders until rotation concluded
  }
  
  stepper.move(map(degPerMove, 1, 360, 1, stepsPerRevolution));
  return true;
}

bool updateDisplay(void *) {

  // Reset countdown timer
  if (currentTick <= 0) {
    Serial.println("*** STEP ***");
    currentTick = stepperInterval/1000;
  }

  Serial.print("CURRENT TICK: ");
  Serial.println(currentTick);

  currentTick--;

    // Only display when not moving
  if (abs(stepper.distanceToGo()) > 0) {
    // DEBUG CODE
    Serial.print("DISTANCE TO GO: ");
    Serial.println(stepper.distanceToGo());
  } else {
    // Reset active rotation flag
    isRotationActive = false;

    // Update OLED buffer

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(16, 0);
    display.println("NEXT ROTATION IN");

    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(32, 10);
    display.print(currentTick);
    display.println("s");
    display.display(); 
  } 
  
  return true;
}

void handleEncoderUpdates() {

  if (!isRotationActive) {
    displayRotationScreen();
    isRotationActive = true; // Block re-renders until rotation concluded
  }
  
  if (digitalRead(pinB) != aVal) {
    // Pin A changed cirst, so rotating CW
    encoderPosCount ++;
    bCW = true;
  } else {
    // Pin B changed first, so rotating CCW
    bCW = false;
    encoderPosCount--;
  }
 
  encoderPosition = map(encoderPosCount, 1, 30, 1, stepsPerRevolution);
  stepper.moveTo(encoderPosition);
 
  pinALast = aVal;
  return;
}
 
void loop() {

  // Disable stepper motor between rotations
  digitalWrite(disablePin, !isRotationActive);
  
  // Check for manual override
  aVal = digitalRead(pinA);
  if (aVal != pinALast){
    handleEncoderUpdates();
  }

  // Advance timer
  timer.tick();

  // Keep running motor, if needed
  stepper.run();
} 
