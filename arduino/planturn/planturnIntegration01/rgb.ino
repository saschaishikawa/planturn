#include <Fadinglight.h>

#define R_PIN 9
#define G_PIN 11
#define B_PIN 10
#define RGB_INTERVAL 10
#define LOGARITHMIC true

Fadinglight ledR(R_PIN, LOGARITHMIC, RGB_INTERVAL);
Fadinglight ledG(G_PIN, LOGARITHMIC, RGB_INTERVAL);
Fadinglight ledB(B_PIN, LOGARITHMIC, RGB_INTERVAL);

SpeedSetting RGB_SLOW = {
  .on_ms = 800,
  .off_ms = 800,
  .pause_ms = 1600,
  .ending_ms = 3200,
};


SpeedSetting RGB_FAST = {
  .on_ms = 400,
  .off_ms = 400,
  .pause_ms = 800,
  .ending_ms = 1600,
};

void setupRGB() {
  if (VERBOSE) {
    Serial.print("Setting up RGB...");  
  }
  
  ledR.setSpeed(RGB_SLOW);
  ledG.setSpeed(RGB_SLOW);
  ledB.setSpeed(RGB_SLOW);

  if (VERBOSE) {
    Serial.println("Done.");
  }
}

void updateRGB() {

  // NOTE: Moisture takes precedence over light
  if (moisturePercent < moistureThreshold) { 
    Serial.println("NOT ENOUGH WATER");
    blinkBlue();
    return;
  } else if (lux < luxThreshold) { // Not enough light
    blinkRed();
    Serial.println("NOT ENOUGH LIGHT");
    return;
  } else { // All systems go
    blinkGreen();
    return;
  }
  
}

void blinkRed() {
  ledR.blink();
  ledG.off();
  ledB.off();

  ledR.setSpeed(RGB_SLOW);
  ledG.setSpeed(RGB_SLOW);
  ledB.setSpeed(RGB_SLOW);

  ledR.update();
  ledG.update();
  ledB.update();
}

void blinkGreen() {
  ledR.off();
  ledG.blink();
  ledB.off();

  ledR.setSpeed(RGB_SLOW);
  ledG.setSpeed(RGB_SLOW);
  ledB.setSpeed(RGB_SLOW);

  ledR.update();
  ledG.update();
  ledB.update();
}

void blinkBlue() {
  ledR.off();
  ledG.off();
  ledB.blink();

  ledR.setSpeed(RGB_SLOW);
  ledG.setSpeed(RGB_SLOW);
  ledB.setSpeed(RGB_SLOW);

  ledR.update();
  ledG.update();
  ledB.update();
}
