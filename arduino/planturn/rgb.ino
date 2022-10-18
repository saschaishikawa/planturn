#include <Fadinglight.h>

#define R_PIN 5
#define G_PIN 6
#define B_PIN 9
#define RGB_INTERVAL 10

Fadinglight ledR(R_PIN, true, 3);
Fadinglight ledG(G_PIN, true, 6);
Fadinglight ledB(B_PIN, true, 9);

SpeedSetting rgbSettings = {
    .on_ms = 800,
    .off_ms = 800,
    .pause_ms = 1600,
    .ending_ms = 3200,
};

void setupRGB() {
  ledR.setSpeed(rgbSettings);
  ledG.setSpeed(rgbSettings);
  ledB.setSpeed(rgbSettings);

  ledR.off();
  ledG.blink();
  ledB.off();
}

void updateRGB() {
  ledR.update();
  ledG.update();
  ledB.update();
}
