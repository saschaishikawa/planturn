#include <arduino-timer.h>

const bool VERBOSE = true;
const int VIN = 5;
const int R_LUX = 220; // 220 Ohm pulldown resistor

/* H2O Settings */
long h2oPollInterval = 250;   // Sensor reading interval (in seconds)
int h2oThreshold = 25;         // Rotor disabled, red light if h2oReading < h2oThreshold
int h2oReading;                // Raw measurement from capacitive soil moisture sensor
int h2oWetReading = 207;       // Raw measurement for fully submerged sensor
int h2oDryReading = 456;       // Raw measurement for dry sensor
int h2oPercent = 0;            // H2O percentage (after calibration)

/* LUX Settings */
long luxPollInterval = 250;  // Sensor reading interval (in seconds)
int luxThreshold = 150;       // Rotor disabled, blue LED if lux < luxThreshold
int luxReading;               // Raw measurement from photoresistor
int lux;                      // Light measurement in flux units

// Instantiate timer for handling timed intervals
auto timer = timer_create_default();
long pingInterval = 1000;

/*
 * Periodically write messages to Serial Port 
 */
bool ping(void *) { 
  // Serial.println("H2O READING: " + String(h2oReading));
  Serial.print("LUX: " + String(lux) + "  //  ");

  if(h2oPercent > 100) {
    Serial.println("H2O: 100%");
  } else if(h2oPercent < 0) {
    Serial.println("H2O: 0%");
  } else if(h2oPercent >= 0 && h2oPercent < 100) {
    Serial.println("H2O: " + String(h2oPercent) + "%");
  }    
}

/*
 * Light Sensor Polling 
 */ 
bool pollLUXSensor(void *) { 

  // Get sensor reading
  luxReading = analogRead(A0);

  // Calculate LUX from circuit parameters
  float vout = float(luxReading) * VIN / float(1023);
  float rldr = (R_LUX * (VIN - vout)) / vout;
  lux = 500 / (rldr/1000);

  return true;
}

/*
 * Moisture Sensor Polling
 */
bool pollH2OSensor(void *) { 
  // Get sensor reading
  h2oReading = analogRead(A1);

  // Calculate H2O percentage based on (calibrated) raw readings
  h2oPercent = map(h2oReading, h2oDryReading, h2oWetReading, 0, 100);

  return true;
}

void setup() {
  // Open serial port, set the baud rate to 9600 bps
  Serial.begin(9600);

  // Set up polling
  timer.every(luxPollInterval, pollLUXSensor);
  timer.every(h2oPollInterval, pollH2OSensor);
  timer.every(min(luxPollInterval, h2oPollInterval), ping);
}

void loop() {
  // Advance timer
  timer.tick();
}