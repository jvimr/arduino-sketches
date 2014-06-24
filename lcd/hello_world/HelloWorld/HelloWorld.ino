/*

 board: http://www.hobbytronics.co.uk/arduino-lcd-keypad-shield
 
 
 
 Arduino LCD Keypad Shield

This is a 16x2 LCD and Keypad shield for Arduino Uno, Diecimila, Duemilanove and Freeduino boards.

Blue Backlight with white words
uses 4 Bit Arduino LCD Library
Left, Right, Up, Down and Select buttons
Screen contrast adjustment
Arduino Reset button
The board uses different pins to the Arduino example sketches, so to make the display work, use the following sequence of pins when starting the library:

LiquidCrystal lcd(8,9,4,5,6,7); 

The buttons are connected to only one analog input pin through resistors to give a different voltage for each button, thus saving on input/output pins. Reading the buttons is easy and example code is shown below

Pin Connections

Pin	Function
Analog 0	 Buttons (select, up, right, down and left)
Digital 4	 DB4
Digital 5	 DB5
Digital 6	 DB6
Digital 7	 DB7
Digital 8	 RS (Data or Signal Display Selection)
Digital 9	 Enable
 	  
Note: Do not use pin Digital 10 when this board is plugged in
 

  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch prints "Hello World!" to the LCD
 and shows the time.
 
  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 */

// include the library code:
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
//LiquidCrystal lcd(8,9,4,5,6,7); 
LiquidCrystal lcd(8,9,2,3,4,5); 


void setup() {
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
}

