#include <PololuLedStrip.h>

// Initialize the LED strip
#define LED_COUNT 100
#define LED_PORT 6
rgb_color colors[LED_COUNT];
PololuLedStrip<LED_PORT> led_strip;

// Constants for the servo
#define SERVO_PORT 3

// Constants for the pots
#define POT1 A0

enum class Mode { constant, gradient, rainbow, pot, servo };
Mode mode = Mode::constant;

// Things for the constant mode.
rgb_color color(0, 0, 0);

// Things for the gradient mode.
// Things for the rainbow mode.
// Things for the pot mode.
// Things for the servo mode.
bool servo_sweep = false;

void setup() {
  Serial.begin(9600);

  // Write the (empty) colors to the LED strip.
  // This clears anything we had done previously.
  led_strip.write(colors, LED_COUNT);
}

void handle_command(char command) {
  switch(command) { // Only do things if these things are sent
    case 'A':
      mode = Mode::constant;
      color = rgb_color(255, 0, 0);
      break;
    case 'X':  
      mode = Mode::constant;
      color = rgb_color(0, 255, 0);
      break;
    case 'Y':
      mode = Mode::constant;
      color = rgb_color(0, 0, 255);
      break;
    case 'B':
      mode = Mode::constant;
      color = rgb_color(0, 0, 0);
      break;
    case 'U': // Up
      mode = Mode::gradient;
      break;
    case 'D': // Down
      mode = Mode::rainbow;
      break;
    case 'L': // Left
      mode = Mode::servo;
      servo_sweep = !servo_sweep;
      break;
    case 'R': // Right
      mode = Mode::pot;
      break;
  }
}


// Converts a color from HSV to RGB.
// h is hue, as a number between 0 and 360.
// s is the saturation, as a number between 0 and 255.
// v is the value, as a number between 0 and 255.
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v)
{
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return rgb_color(r, g, b);
}


void apply_constant() {
  for (uint16_t i = 0; i < LED_COUNT; ++i) {
    colors[i] = color;
  }

  led_strip.write(colors, LED_COUNT);
}


// This is also taken from pololu
void apply_gradient() {
  // Update the colors.
  byte time = millis() >> 2;
  for (uint16_t i = 0; i < LED_COUNT; i++)
  {
    byte x = time - 8*i;
    colors[i] = rgb_color(x, 255 - x, x);
  }

  // Write the colors to the LED strip.
  led_strip.write(colors, LED_COUNT);
}


// This is taken directly from Pololu
void apply_rainbow() {
  // Update the colors.
  uint16_t time = millis() >> 2;
  for(uint16_t i = 0; i < LED_COUNT; i++)
  {
    byte x = (time >> 2) - (i << 3);
    colors[i] = hsvToRgb((uint32_t)x * 359 / 256, 255, 255);
  }

  // Write the colors to the LED strip.
  led_strip.write(colors, LED_COUNT);
}


void apply_pot() {
  int val1 = analogRead(POT1) / 4; // Convert 0-1024 -> 0-255
  color = hsvToRgb(val1 * 360 / 255, 255, 255);
  color = rgb_color(color.red, color.blue, color.green);
  apply_constant();
}

void apply_servo() {
  
}

void loop() {

  // First, try to read from serial to see if anything new is available.
  if (Serial.available()) {
    char command = Serial.read(); // Read a single byte
    handle_command(command);  
  
  }

  switch(mode) {
    case Mode::constant:
      apply_constant();
      break;
     case Mode::gradient:
      apply_gradient();
      break;
     case Mode::rainbow:
      apply_rainbow();
      break;
     case Mode::pot:
      apply_pot();
      break;
     case Mode::servo:
      apply_servo();
      break;
  }
  
  delay(10);
}
