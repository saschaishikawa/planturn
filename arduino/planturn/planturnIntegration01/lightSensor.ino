#define VIN 5
#define R 220

int lightReading;

bool pollLightSensor(void *) { 
  lightReading = analogRead(A0);

  float vout = float(lightReading) * VIN / float(1023);
  float rldr = (R * (VIN - vout)) / vout;
  lux = 500 / (rldr/1000);

//  if (VERBOSE) {
//    Serial.print("LUX: ");
//    Serial.println(String(lux));
//  }
  
  return true;
}
