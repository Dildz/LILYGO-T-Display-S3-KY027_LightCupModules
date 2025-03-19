/*********************************************************************************************************
* LILYGO T-Display-S3 KY-027 Magic Light Cup Module Project
*
* This sketch reads the digital signals from the KY-027 modules and controls the brightness of LEDs
* based on the tilt state of the modules. The status and brightness levels are displayed on the
* built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.
*
* Description:
*  The code uses two intervals: one for reading the tilt switches and updating LED brightness (5ms),
*   and another for updating the display (100ms).
*  The brightness levels for each module are displayed and updated separately.
*  The LEDs are initialized based on the tilt switch positions at startup.
*  The LEDs fade on and off smoothly based on the tilt state.
*
* Pin Connections:
*  - Module A&B VCC -> 3.3V
*  - Module A&B GND -> GND
*  - Module A D0    -> GPIO03 (input)
*  - Module A LED   -> GPIO02 (output)
*  - Module B D0    -> GPIO18 (input)
*  - Module B LED   -> GPIO44 (output)
*
* KY-027 Specifications:
*  - Protocol: Digital
*  - Operating Voltage: 3.3V to 5V
**********************************************************************************************************/

/*************************************************************
******************* INCLUDES & DEFINITIONS *******************
**************************************************************/

#include <Arduino.h>
#include <TFT_eSPI.h>

// TFT_eSPI
TFT_eSPI tft = TFT_eSPI();

// Pin definitions
const int switchPinA = 3;  // tilt switch pin for Module A
const int ledPinA = 2;     // LED pin for Module A

const int switchPinB = 18; // tilt switch pin for Module B
const int ledPinB = 44;    // LED pin for Module B

// Variables for timing
unsigned long previousReadMillis = 0;   // last time the switches were read
unsigned long previousUpdateMillis = 0; // last time the display was updated
const long readInterval = 5;            // interval for reading switches and updating brightness (ms)
const long updateInterval = 100;        // interval for updating the display (ms)

// Global variables
int switchStateA = HIGH;       // state from Module A (active low)
int switchStateB = HIGH;       // state from Module B (active low)
int brightnessA = 0;           // brightness level for LED A
int brightnessB = 0;           // brightness level for LED B
const int brightnessSteps = 2; // step size for brightness changes
bool redrawRequired = true;    // flag to redraw the screen


/*************************************************************
********************** HELPER FUNCTIONS **********************
**************************************************************/

// Function to draw static elements on the TFT screen
void drawStaticElements() {
  // Clear the screen and set text properties
  tft.fillScreen(TFT_BLACK);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Reset cursor to the top-left corner
  tft.setCursor(0, 0);

  // Display header
  tft.println("---------------------------");
  tft.println(" KY027 Magic Light Cups");
  tft.println("---------------------------");

  // Display static labels
  tft.setCursor(0, 70); // position for LED A brightness
  tft.print("LED A Brightness:");

  tft.setCursor(0, 120); // position for LED B brightness
  tft.print("LED B Brightness:");
}

// Function to update dynamic elements on the TFT screen
void updateDynamicElements() {
  // Update LED A brightness
  tft.setCursor(0, 90); // position for LED A brightness value
  tft.print("      ");  // clear previous value (6 spaces)
  tft.setCursor(0, 90); // reset cursor
  tft.print(brightnessA);

  // Update LED B brightness
  tft.setCursor(0, 140); // position for LED B brightness value
  tft.print("      ");   // clear previous value (6 spaces)
  tft.setCursor(0, 140); // reset cursor
  tft.print(brightnessB);
}


/*************************************************************
*********************** MAIN FUNCTIONS ***********************
**************************************************************/

// SETUP
void setup() {
  // Set pin modes
  pinMode(switchPinA, INPUT);
  pinMode(ledPinA, OUTPUT);

  pinMode(switchPinB, INPUT);
  pinMode(ledPinB, OUTPUT);

  // Initialize the TFT display
  tft.init();
  tft.setRotation(0);                     // adjust rotation (0 & 2 portrait | 1 & 3 landscape)
  tft.fillScreen(TFT_BLACK);              // black background
  tft.setTextFont(2);                     // set the font size
  tft.setTextColor(TFT_WHITE, TFT_BLACK); // set text colour (white) and background colour (black)

  // Draw static elements once
  drawStaticElements();

  // Set initial LED states based on tilt switch states
  switchStateA = digitalRead(switchPinA); // read Module A tilt switch
  switchStateB = digitalRead(switchPinB); // read Module B tilt switch

  // Set LEDs based on startup tilt switch states
  if (switchStateA == LOW) { // module A is tilted
    brightnessA = 255;       // LED A to full brightness
  }
  if (switchStateB == LOW) { // module B is tilted
    brightnessB = 255;       // LED B to full brightness
  }

  // Control the LEDs based on initial brightness values
  analogWrite(ledPinA, brightnessA); // set LED A brightness
  analogWrite(ledPinB, brightnessB); // set LED B brightness

  // Initial display update
  redrawRequired = true; // update display
}


// MAIN LOOP
void loop() {
  unsigned long currentMillis = millis(); // get the current millis time

  // Read tilt switches and update brightness at the set readInterval
  if (currentMillis - previousReadMillis >= readInterval) {
    previousReadMillis = currentMillis; // save the last read time

    // Read the digital value from Module A
    switchStateA = digitalRead(switchPinA);

    // Read the digital value from Module B
    switchStateB = digitalRead(switchPinB);

    // Fade LED A ON
    if (switchStateA == LOW) {
      if (brightnessA < 255) {
        brightnessA += brightnessSteps; // fade LED
        if (brightnessA > 255) brightnessA = 255; // clamp brightness to maximum value
      }
    }
    // Fade LED A OFF
    else {
      if (brightnessA > 0) {
        brightnessA -= brightnessSteps; // fade LED
        if (brightnessA < 0) brightnessA = 0; // clamp brightness to minimum value
      }
    }

    // Fade LED B ON
    if (switchStateB == LOW) {
      if (brightnessB < 255) {
        brightnessB += brightnessSteps; // fade LED
        if (brightnessB > 255) brightnessB = 255; // clamp brightness to maximum value
      }
    }
    // Fade LED B OFF
    else {
      if (brightnessB > 0) {
        brightnessB -= brightnessSteps; // fade LED
        if (brightnessB < 0) brightnessB = 0; // clamp brightness to minimum value
      }
    }

    // Control the LEDs
    analogWrite(ledPinA, brightnessA); // LED A brightness
    analogWrite(ledPinB, brightnessB); // LED B brightness
  }

  // Update the display every updateInterval (100ms)
  if (currentMillis - previousUpdateMillis >= updateInterval) {
    previousUpdateMillis = currentMillis; // save the last display update time

    // Update the display
    updateDynamicElements();
  }
}
