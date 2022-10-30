#include <AccelStepper.h>

// Define the stepper motor and the pins that is connected to
AccelStepper stepper(1, stepPin, dirPin); // (Typeof driver: with 2 pins, STEP, DIR)

void setupStepperMotor() {
  
  if (VERBOSE) {
    Serial.print("Setting up rotor...");
  }
  
  pinMode(disablePin, OUTPUT);
  digitalWrite(disablePin, HIGH);  // Disable stepper motor until needed
  stepper.setMaxSpeed(5000);       // Set maximum speed value for the stepper
  stepper.setAcceleration(5000);   // Set acceleration value for the stepper
  stepper.setCurrentPosition(0);   // Set the current position to 0 steps

  if (VERBOSE) {
    Serial.println("Done.");
  }
}

bool moveStepperMotor(void *) {
  if (lux < luxThreshold) {
    return;
  }
  
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
