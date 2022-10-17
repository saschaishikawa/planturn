int buttonPin = 17; // Rotary encoder pushbutton
int pinA = 18;      // Connected to CLK on KY-040
int pinB = 19;      // Connected to DT on KY-040

int buttonState = LOW;
int lastState;

const int DEBOUNCE_DELAY = 50; // 50ms debounce time between button presses
int lastDebounceTime;
int lastFlickerableState = LOW;
int lastSteadyState = LOW;

int menuCount = 2;

int pinALast;
int aVal;
bool bCW;
int encoderPosition = 0;
int encoderPosCount = 0;

void setupRotaryEncoder() {
  // Set up rotary encoder
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(pinA,INPUT);
  pinMode(pinB,INPUT);

  pinALast = digitalRead(pinA);
}

void checkButtonPress() {

  // Read current button state
  buttonState = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch/button changed, due to noise or pressing:
  if (buttonState != lastFlickerableState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
    // save the the last flickerable state
    lastFlickerableState = buttonState;
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (lastSteadyState == HIGH && buttonState == LOW) {
      Serial.println("The button is pressed");
    } else if (lastSteadyState == LOW && buttonState == HIGH) {
      Serial.println("The button is released");
      cycleMenus();
    }

    // save the the last steady state
    lastSteadyState = buttonState;
  }
  
//  // Read the button state
//  buttonState = digitalRead(buttonPin);
//
//  if(lastState == HIGH && buttonState == LOW) {
//    Serial.println("The button is pressed");
//  } else if(lastState == LOW && buttonState == HIGH) {
//    Serial.println("The button is released");
//    cycleMenus();
//  }
//    lastState = buttonState;
}

void cycleMenus() {
  currentMenuIndex++;
  if (currentMenuIndex > menuCount-1) {
    currentMenuIndex = 0; // Reset to beginning
  }
  Serial.print("Current Menu: ");
  Serial.println(currentMenuIndex + 1);
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
  moveStepperMotorTo(encoderPosition);
 
  pinALast = aVal;
  return;
}
