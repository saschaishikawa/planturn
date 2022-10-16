#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Instantiate OLED display
Adafruit_SSD1306 display(OLED_RESET);

void setupDisplay() {
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
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
  if (abs(stepperMotorDistanceToGo()) > 0) {
    // DEBUG CODE
    Serial.print("DISTANCE TO GO: ");
    Serial.println(stepperMotorDistanceToGo());
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