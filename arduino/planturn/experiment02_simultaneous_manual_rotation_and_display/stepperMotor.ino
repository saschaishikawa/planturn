#include <AccelStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper(1, stepPin, dirPin); // (Typeof driver: with 2 pins, STEP, DIR)

int degPerMove = 15;

void setupStepperMotor() {
  pinMode(disablePin, OUTPUT);
  digitalWrite(disablePin, HIGH);  // Disable stepper motor until needed
  stepper.setMaxSpeed(5000);       // Set maximum speed value for the stepper
  stepper.setAcceleration(5000);   // Set acceleration value for the stepper
  stepper.setCurrentPosition(0);   // Set the current position to 0 steps
}

bool moveStepperMotor(void *) {
  if (!isRotationActive) {
    displayRotationScreen();
    isRotationActive = true; // Block re-renders until rotation concluded
  }
  
  stepper.move(map(degPerMove, 1, 360, 1, stepsPerRevolution));
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