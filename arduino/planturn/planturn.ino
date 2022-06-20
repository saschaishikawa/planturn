#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Stepper motor pins
#define stepPin 2
#define dirPin 5 

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

const int AirValue = 528;    // value for dry sensor
const int WaterValue = 226;  // value for wet sensor
int soilMoistureValue = 0;
int soilmoisturepercent=0;

void setup() {

  // Initialize stepper motor pins
  pinMode(stepPin,OUTPUT); 
  pinMode(dirPin,OUTPUT);

  Serial.begin(9600); // open serial port, set the baud rate to 9600 bps
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
}

void loop() {

  // Move stepper motor
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(1);
  digitalWrite(stepPin, LOW);

  // Read moisture sensor data
  soilMoistureValue = analogRead(A0);
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

 // Display OLED
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
  
  if(soilmoisturepercent > 100) {
    Serial.println("100 %");
    display.println("100 %");
  } else if(soilmoisturepercent <0) {
    Serial.println("0 %");
    display.println("0 %");
  } else if(soilmoisturepercent >= 0 && soilmoisturepercent < 100) {
    Serial.print(soilmoisturepercent);
    display.println(String(soilmoisturepercent) + " %");
  }  

  display.display();
  delay(100);
  display.clearDisplay();
}
