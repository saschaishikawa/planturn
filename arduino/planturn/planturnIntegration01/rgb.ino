#include <Fadinglight.h>

#define R_PIN 3
#define G_PIN 5
#define B_PIN 6
#define RGB_INTERVAL 10
#define LOGARITHMIC true

Fadinglight ledR(R_PIN, LOGARITHMIC, RGB_INTERVAL);
Fadinglight ledG(G_PIN, LOGARITHMIC, RGB_INTERVAL);
Fadinglight ledB(B_PIN, LOGARITHMIC, RGB_INTERVAL);

SpeedSetting rgbSettings = {
  .on_ms = 800,
  .off_ms = 800,
  .pause_ms = 1600,
  .ending_ms = 3200,
};

void setupRGB() {
  if (VERBOSE) {
    Serial.print("Setting up RGB...");  
  }
  
  ledR.setSpeed(rgbSettings);
  ledG.setSpeed(rgbSettings);
  ledB.setSpeed(rgbSettings);

  ledR.off();
  ledG.blink();
  ledB.off();

  if (VERBOSE) {
    Serial.println("Done.");
  }
}

void updateRGB() {
  ledR.update();
  ledG.update();
  ledB.update();

  if (VERBOSE) {
    Serial.println("Updated RGB.");
  }
}
