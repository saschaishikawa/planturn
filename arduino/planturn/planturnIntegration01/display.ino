#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED pins
#define OLED_RESET 4

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

//  Serial.print("CURRENT TICK: ");
//  Serial.println(currentTick);

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
  display.println("PLANTURN v1.0a");
  
  display.setCursor(0,16);
  display.println("DEG: N/A");
  display.setCursor(30,16);
  
  display.setCursor(64,16);
  display.println("RPD: 1.0");

  display.setCursor(64,8);
  display.println("LUX: N/A");

  display.setCursor(64,24);
  display.println("WGT: N/A");

  display.setCursor(0,24);
  display.println("H2O:");

  display.setCursor(30,24);
  
  if(moisturePercent > 100) {
    display.println("100 %");
  } else if(moisturePercent <0) {
    display.println("0 %");
  } else if(moisturePercent >= 0 && moisturePercent < 100) {
    display.println(String(moisturePercent) + " %");
  }  

  display.display();
}


void displayRotationCountdown() {
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

void displayDefaultMenu() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(16, 0);
  display.println("???");
  display.display(); 
}
