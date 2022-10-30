// Rotary Encoder Inputs
#define inputCLK 2
#define inputDT 3
#define inputBTN 4

// Position
int rotaryEncoderCounter = 0;  // Counts number of rotary encoder "clicks"
String encdir ="";             // Possible values: CW, CCW
int currentStateCLK;
int previousStateCLK; 
int currentStateBTN;
int encoderPosition;

// Button
const int DEBOUNCE_DELAY = 50;
int lastFlickerableStateBTN; // Reliable state
int lastSteadyStateBTN;
int lastDebounceTimeBTN;

int menuCount = 2;

void setupRotaryEncoder() {
  if (VERBOSE) {
    Serial.print("Setting up rotary encoder...");
  }

  pinMode(inputCLK, INPUT);
  pinMode(inputDT, INPUT);
  pinMode(inputBTN, INPUT_PULLUP);
  
  previousStateCLK = digitalRead(inputCLK);
  currentStateBTN = HIGH;
  encoderPosition = 0;
  
  if (VERBOSE) {
    Serial.println("Done.");
  }
}

void checkButtonPress() {
  // Read current button state
  currentStateBTN = digitalRead(inputBTN);
  
  // Potential button press (could be noise)
  if (currentStateBTN != lastFlickerableStateBTN) {
    lastDebounceTimeBTN = millis();
    lastFlickerableStateBTN = currentStateBTN;
  }

  // Debounce button presses
  if ((millis() - lastDebounceTimeBTN) > DEBOUNCE_DELAY) {
    if (lastSteadyStateBTN == HIGH && currentStateBTN == LOW) {
      if (VERBOSE) {
        Serial.println("The button is pressed");
      }
    } else if (lastSteadyStateBTN == LOW && currentStateBTN == HIGH) {
      cycleMenus();
      if (VERBOSE) {
        Serial.println("The button is released");
      }
      
    }
    lastSteadyStateBTN = currentStateBTN;
  }
}

void cycleMenus() {
  currentMenuIndex++;
  if (currentMenuIndex > menuCount-1) {
    currentMenuIndex = 0; // Reset to beginning
  }

  if (VERBOSE) {
    Serial.print("Current Menu: ");
    Serial.println(currentMenuIndex + 1);
  }
}


void handleRotaryEncoderUpdates() {
  
  // Read the current state of inputCLK
  currentStateCLK = digitalRead(inputCLK);
  
  // If the previous and the current state of the inputCLK are different then a pulse has occured
  if (currentStateCLK == previousStateCLK) {
   return;
  }

  if (!isRotationActive) {
    displayRotationScreen();
    isRotationActive = true; // Block re-renders until rotation concluded
  }
     
  // Detect change in direction
  if (digitalRead(inputDT) != currentStateCLK) {
    rotaryEncoderCounter--;
    encdir ="CCW";
  } else {
    rotaryEncoderCounter++;
    encdir =" CW";
  }
  
  // Update previousStateCLK with the current state
  previousStateCLK = currentStateCLK; 
  encoderPosition = map(rotaryEncoderCounter, 1, 30, 1, stepsPerRevolution);
  moveStepperMotorTo(encoderPosition);

  if (VERBOSE) {
    Serial.print(encdir);
    Serial.print(" POSITION: ");
    Serial.println(rotaryEncoderCounter);
  }
}
