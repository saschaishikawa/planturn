int pinA = 18; // Connected to CLK on KY-040
int pinB = 19; // Connected to DT on KY-040
int pinALast;
int aVal;
bool bCW;
int encoderPosition = 0;
int encoderPosCount = 0;

void setupRotaryEncoder() {
  // Set up rotary encoder
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);
  pinALast = digitalRead(pinA);
}

void handleRotaryEncoderUpdates() {

  // Read Pin A value; if no change, do nothing...
  aVal = digitalRead(pinA);
  if (aVal == pinALast) {
    return;
  }

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
  // stepper.moveTo(encoderPosition);
  moveStepperMotorTo(encoderPosition);
 
  pinALast = aVal;
  return;
}