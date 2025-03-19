LILYGO T-Display-S3 KY-027 Magic Light Cup Module Project

This sketch reads the digital signals from the KY-027 modules and controls the brightness of LEDs
based on the tilt state of the modules. The status and brightness levels are displayed on the
built-in screen of the LilyGO T-Display-S3 using the TFT_eSPI library.

Description:

 The code uses two intervals: one for reading the tilt switches and updating LED brightness (5ms), and another for updating the display (100ms).
 The brightness levels for each module are displayed and updated separately.
 The LEDs are initialized based on the tilt switch positions at startup.
 The LEDs fade on and off smoothly based on the tilt state.

Pin Connections:
 - Module A&B VCC -> 3.3V
 - Module A&B GND -> GND
 - Module A D0    -> GPIO03 (input)
 - Module A LED   -> GPIO02 (output)
 - Module B D0    -> GPIO18 (input)
 - Module B LED   -> GPIO44 (output)

KY-028 Specifications:
 - Operating Voltage: 3.3V to 5.5V
