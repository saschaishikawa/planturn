/*
 * Simultaneous Operations (Experiment 2) 
 * Manual Rotation, Periodic Rotation, and Display 
 * Sascha T. Ishikawa
 * October 14, 2022
 * 
 * Rotation glitches whenever a timed event, i.e., updating the display, occurs on the same timestep
 * May need to use interrupts or get clever with timing
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

int pinA = 40; // Connected to CLK on KY-040
int pinB = 41; // Connected to DT on KY-040
int encoderPosCount = 0;
int pinALast;
int aVal;
boolean bCW;

long stepperInterval = 10000; //450000; // 7.5 minutes
long displayInterval = 1000;  // 1 second
long currentTick = stepperInterval / 1000;

int encoderPosition = 0;

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
  pinMode (pinA,INPUT);
  pinMode (pinB,INPUT);
  pinALast = digitalRead(pinA);
  Serial.begin (9600);

  // Set up stepper motor
  stepper.setMaxSpeed(5000); // Set maximum speed value for the stepper
  stepper.setAcceleration(5000); // Set acceleration value for the stepper
  stepper.setCurrentPosition(0); // Set the current position to 0 steps

  // Set up timers
  timer.every(stepperInterval, moveStepperMotor);
//  timer.every(displayInterval, updateDisplay);
}

bool moveStepperMotor(void *) {
//  return true;
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

void handleEncoderUpdates() {
  
  // if the knob is rotating, we need to determine direction
  // We do that by reading pin B.
  if (digitalRead(pinB) != aVal) { // Means pin A Changed first - We're Rotating Clockwise
    encoderPosCount ++;
    bCW = true;
  } else {// Otherwise B changed first and we're moving CCW
    bCW = false;
    encoderPosCount--;
  }
  
  Serial.print ("Rotated: ");
  
  if (bCW){
    Serial.println ("clockwise");
  } else {
    Serial.println("counterclockwise");
  }
  
  encoderPosition = map(encoderPosCount, 1, 30, 1, stepsPerRevolution);

  Serial.print("Encoder Position: " + encoderPosition);
  stepper.moveTo(encoderPosition);
 
  pinALast = aVal;
  return;
}
 
void loop() {
  
  // Check for manual override
  aVal = digitalRead(pinA);
  if (aVal != pinALast){
    handleEncoderUpdates();
  }

  // Keep the timer running
  timer.tick();

  // Keep running motor, if needed
  stepper.run();

} 
