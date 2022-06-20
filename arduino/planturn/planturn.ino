#include <Adafruit_SSD1306.h>
#include <splash.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Stepper motor pins
#define MOTOR_STEP_PIN 2
#define MOTOR_DIR_PIN 5 

// Rotary encoder pins
#define ENCODER_CLOCK_PIN 30 // Connected to D30
#define ENCODER_DATA_PIN 31  // Connected to D31
#define ENCODER_PUSH_PIN 32  // Connected to D32

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET);

/*
 * MOISTURE SENSOR VARIABLES
 */
const int AirValue = 528;    // value for dry sensor
const int WaterValue = 226;  // value for wet sensor
int soilMoistureValue = 0;
int soilmoisturepercent=0;


/* 
 * ROTARY ENCODER VARIABLES
 */
 int counter = 0;                    // Stores "steps"
 int currentStateClock;              // Stores status ENCODER_CLOCK_PIN (HIGH or LOW)
 int lastStateClock;                 // Stores previous statur of ENCODER_CLOCK_PIN (HIGH or LOW)
 String currentDir ="";              // Print text 
 unsigned long lastButtonPress = 0;  // Stores pushbutton status
  
void setup() {

  /* 
   * INITIALIZE STEPPER MOTOR PINS
   */
  pinMode(MOTOR_STEP_PIN,OUTPUT); 
  pinMode(MOTOR_DIR_PIN,OUTPUT);


  /*
   * INITIALIZE ROTARY ENCODER
   */
  pinMode(ENCODER_CLOCK_PIN, INPUT_PULLUP);
  pinMode(ENCODER_DATA_PIN, INPUT_PULLUP);
  pinMode(ENCODER_PUSH_PIN, INPUT_PULLUP);

  // Open serial port with 9600 bps baud rate
  Serial.begin(9600);

  // Read the initial state of ENCODER_CLOCK_PIN (it could be HIGH or LOW)
  lastStateClock = digitalRead(ENCODER_CLOCK_PIN);

  // Initialize OLED with the I2C addr 0x3C (128x64)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
}

void loop() {

  /*
   * HANDLE ROTARY ENCODER EVENTS
   */

  // Read the current state of CLK
  currentStateClock = digitalRead(ENCODER_CLOCK_PIN);

  // If last and current state of ENCODER_CLOCK_PIN are different, then "pulse occurred"
  // React to only 1 state change to avoid double count
  if (currentStateClock != lastStateClock  && currentStateClock == 1){

    // If the ENCODER_DATA_PIN state is different than the ENCODER_CLOCK_PIN state then
    // the encoder is rotating "CCW" so we decrement
    if (digitalRead(ENCODER_DATA_PIN) != currentStateClock) {
      counter --;
      currentDir ="Counterclockwise";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="Clockwise";
    }

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  // We save last ENCODER_CLOCK_PIN state for next loop
  lastStateClock = currentStateClock;

  // Read the button state
  int btnState = digitalRead(ENCODER_PUSH_PIN);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  /*
   * MOVE STEPPER MOTOR
   */
  digitalWrite(MOTOR_STEP_PIN, HIGH);
  delayMicroseconds(1);
  digitalWrite(MOTOR_STEP_PIN, LOW);

  /*
   * READ MOISTURE SENSOR DATA
   */
  soilMoistureValue = analogRead(A0);
  Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);

  /*
   * UPDATE OLED SCREEN
   */
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
