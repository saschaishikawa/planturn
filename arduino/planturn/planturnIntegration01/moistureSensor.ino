const int AirValue = 528;    // value for dry sensor
const int WaterValue = 226;  // value for wet sensor
int moistureReading = 0;
int moisturePercent = 0;

bool pollMoistureSensor(void *) { 
  moistureReading = analogRead(A0);
  moisturePercent = map(moistureReading, AirValue, WaterValue, 0, 100);

  if(moisturePercent > 100) {
    Serial.println("100 %");
  } else if(moisturePercent <0) {
    Serial.println("0 %");
  } else if(moisturePercent >= 0 && moisturePercent < 100) {
    Serial.println(moisturePercent);
  }  
  
  return true;
}