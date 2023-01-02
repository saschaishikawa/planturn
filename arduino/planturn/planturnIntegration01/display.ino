#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED pins
#define OLED_RESET 4

// Instantiate OLED display
Adafruit_SSD1306 display(OLED_RESET);

void setupDisplay() {
  
  if (VERBOSE) {
    Serial.print("Setting up display...");
  }

  // Initialize with the I2C addr 0x3C (128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  if (VERBOSE) {
    Serial.println("Done.");
  }
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
    currentTick = STEPPER_INTERVAL_MS/1000;
  }

  currentTick--;

  // Only display when not moving
  if (abs(stepperMotorDistanceToGo()) > 0) {
    //// DEBUG CODE
    //Serial.print("DISTANCE TO GO: ");
    //Serial.println(stepperMotorDistanceToGo());
  } else {
    // Reset active rotation flag
    isRotationActive = false;

    // Update OLED buffer
    displayActiveMenu();
  } 
  
  return true;
}

void displayActiveMenu() {
    switch (currentMenuIndex) {
      case(0): // Menu 1
        displayMainMenu();
        break;
      case(1): // Menu 2
        displayRotationCountdown();
        break;
      default:
        displayDefaultMenu();
    }
}

void displayMainMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(18,0);
  display.println("PLANTURN v1.0");

  // Countdown
  display.setCursor(0,8);
  display.print("TIM: ");
  if (lux < luxThreshold) {
    display.println("OFF");
  } else {
    display.println(String(currentTick) + "s");  
  }
  display.setCursor(30,8);

  // Current Position (Deg)
  display.setCursor(0,16);
  display.print("DEG: ");
  display.println(String(degPerMove));
  display.setCursor(30,16);

  // Revolutions Per Day
  display.setCursor(64,16);
  display.print("RPD: ");
  display.println(String(revPerDay));

  // Brightness
  display.setCursor(64,8);
  display.print("LUX: ");
  display.println(String(lux));

  // Weight
  display.setCursor(64,24);
  display.println("WGT: N/A");

  // Moisture
  display.setCursor(0,24);
  display.println("H2O:");

  display.setCursor(30,24);
  
  if(moisturePercent > 100) {
    display.println("100 %");
  } else if(moisturePercent <0) {
    display.println("0 %");
  } else if(moisturePercent >= 0 && moisturePercent < 100) {
    display.println(String(moisturePercent) + "%");
  }  

  display.display();
}

void displayRotationCountdown() {
  display.clearDisplay();

  if (lux < luxThreshold) {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(16, 0);
    display.println("ROTATION DISABLED");
    display.setCursor(16, 10);
    display.println("NOT ENOUGH LIGHT");
  } else {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(16, 0);
    display.println("NEXT ROTATION IN");
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(32, 10);
    display.print(currentTick);
    display.println("s");  
  }
  
  display.display(); 
}

void displayDefaultMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 0);
  display.println("???");
  display.display(); 
}
