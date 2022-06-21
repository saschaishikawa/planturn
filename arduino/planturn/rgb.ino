#define R_PIN 10
#define G_PIN 11
#define B_PIN 12
#define RGB_SPEED 1
 
void setup() {
  pinMode(R_PIN, OUTPUT);
  pinMode(G_PIN, OUTPUT);
  pinMode(B_PIN, OUTPUT);
}
 
 
void loop() {
  int r, g, b;
 
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(R_PIN, r);
    delay(RGB_SPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(B_PIN, b);
    delay(RGB_SPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(G_PIN, g);
    delay(RGB_SPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(R_PIN, r);
    delay(RGB_SPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(B_PIN, b);
    delay(RGB_SPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(G_PIN, g);
    delay(RGB_SPEED);
  } 
}
