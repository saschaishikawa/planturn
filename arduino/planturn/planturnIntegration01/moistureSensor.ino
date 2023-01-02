/*
 * Sensor must be calibrated before use by setting AirValue and WaterValue.
 * 
 * MOISTURE SENSOR CALIBRATION INSTRUCTIONS
 * 
 * Step 1: Print moistureReading (uncomment line 26), re-compile and upload code to Arduino
 * Step 2: Plug in Moisture Sensor (yellow signal pin routes to Arduino Analog Pin 1, or A1)
 * Step 3: Hold Moisture Sensor by the plastic housing (do not touch the capacitive strip)
 * Step 4: Record moisture reading value as AirValue
 * Step 5: Fill a cup of water until it reaches the white line on the Moisture Sensor
 * Step 6: Record moisture reading value as WaterValue
 * Step 7: Re-comment line 26, re-compile and upload code to Arduino
 * 
 * Moisture Sensor should now be calibrated.
 */
 
const int AirValue = 460;    // value for dry sensor
const int WaterValue = 196;  // value for wet sensor
int moistureReading = 0;

bool pollMoistureSensor(void *) { 
  moistureReading = analogRead(A1);
  moisturePercent = map(moistureReading, AirValue, WaterValue, 0, 100);

  //// Use for calibration
  //Serial.println(moistureReading);

  if (VERBOSE) {
    if(moisturePercent > 100) {
      Serial.println("H2O: 100%");
    } else if(moisturePercent <0) {
      Serial.println("H2O: 0%");
    } else if(moisturePercent >= 0 && moisturePercent < 100) {
      Serial.println("H2O: " + String(moisturePercent) + "%");
    }  
  }
  return true;
}
