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
