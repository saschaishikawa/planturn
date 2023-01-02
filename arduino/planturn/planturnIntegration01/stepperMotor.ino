#define DIR_PIN 7
#define DISABLE_PIN 5
#define STEP_PIN 6

#include <AccelStepper.h>


// Define the stepper motor and the pins that is connected to
AccelStepper stepper(1, STEP_PIN, DIR_PIN); // (Typeof driver: with 2 pins, STEP, DIR)

void setupStepperMotor() {
  
  if (VERBOSE) {
    Serial.print("Setting up rotor...");
  }
  
  pinMode(DISABLE_PIN, OUTPUT);
  digitalWrite(DISABLE_PIN, HIGH);  // Disable stepper motor until needed
  stepper.setMaxSpeed(5000);       // Set maximum speed value for the stepper
  stepper.setAcceleration(5000);   // Set acceleration value for the stepper
  stepper.setCurrentPosition(0);   // Set the current position to 0 steps

  if (VERBOSE) {
    Serial.println("Done.");
  }
}

/*
 * Timed rotation
 */
bool moveStepperMotor(void *) {

  // Block re-renders until rotation concluded
  if (!isRotationActive) {
    displayRotationScreen();
    isRotationActive = true;
  }

  // Only move if there is enough ambient light
  if (lux >= luxThreshold) {
    stepper.move(map(degPerMove, 1, 360, 1, STEPS_PER_REVOLUTION));
  }

  // Disable stepper motor between rotations
  digitalWrite(DISABLE_PIN, !isRotationActive);
  
  return true;
}

void runStepperMotor() {
  stepper.run();
}

void moveStepperMotorTo(int position) {
  stepper.moveTo(position);
}

int stepperMotorDistanceToGo() {
 return stepper.distanceToGo();
}
