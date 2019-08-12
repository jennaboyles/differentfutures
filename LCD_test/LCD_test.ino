//Command Reference for adusting the display
//https://learn.adafruit.com/usb-plus-serial-backpack/command-reference

#include "Arduino.h"
 
#if defined(ARDUINO_ARCH_SAMD) || defined(__SAM3X8E__)
  // use pin 18 with Due, pin 1 with Zero or M0 Pro 
  #define lcd Serial1 
#else
  #include <SoftwareSerial.h>
  // Create a software serial port!
  SoftwareSerial lcd = SoftwareSerial(0,2);
#endif
 
// for ESP8266 use this SoftwareSerial library: https://github.com/plerup/espsoftwareserial
 
void setup() {
  lcd.begin(9600);  
  
  // set the size of the display if it isn't 16x2 (you only have to do this once)
  lcd.write(0xFE); //Turn LCD display on
  lcd.write(0xD1); //Set LCD size
  lcd.write(16);  // 16 columns
  lcd.write(2);   // 2 rows
  delay(10);       
  // we suggest putting delays after each command to make sure the data 
  // is sent and the LCD is updated.
 
  // set the contrast, 200 is a good place to start, adjust as desired
  
  lcd.write(0xFE);
  lcd.write(0x50); //set display contrast 
  lcd.write(200); //display contrast value. 180-220 recommended
  delay(10);       
  
  // set the brightness - we'll max it (255 is max brightness)
  lcd.write(0xFE);
  lcd.write(0x99); //set overall brightness
  lcd.write(255);  //brightness value 0-255
  delay(10);       
  
  // turn off cursors
  lcd.write(0xFE);
  lcd.write(0x4B); //0x4A turns on underline cursor
  lcd.write(0xFE);
  lcd.write(0x54); //0x53 turns on blinking block cursor
  
  // create a custom character
  lcd.write(0xFE);
  lcd.write(0x4E); //create a custom character in spot (can be between 0-7 which is 8 spots)
  lcd.write((uint8_t)0);     // location #0. uint8_t is a byte. a type of unsigned integer of length 8 bits
  lcd.write((uint8_t)0x00);  // 8 bytes of character data
  lcd.write(0x0A);
  lcd.write(0x15);
  lcd.write(0x11);
  lcd.write(0x11);
  lcd.write(0x0A);
  lcd.write(0x04);
  lcd.write((uint8_t)0x00);
  delay(10);   // we suggest putting delays after each command 
  
  // clear screen
  lcd.write(0xFE);
  lcd.write(0x58); //clears the screen of text
  delay(10);   // we suggest putting delays after each command 
  
  // go 'home'
  lcd.write(0xFE);
  lcd.write(0x48); //place cursor at (1,1)
  delay(10);   // we suggest putting delays after each command 
 
  lcd.print("We ");
  lcd.write((uint8_t)0);            // to print the custom character, 'write' the location
  lcd.println(" Arduino!");
  lcd.print("     - Adafruit");
  
  delay(1000);
}
 
uint8_t red, green, blue;
void loop() {
// adjust colors 
// to set backlight to red use 0xD0 0xFF 0x0 0x0
// to set backlight to blue use 0xD0 0x0 0x0 0xFF
// to set backlight to white use 0xD0 0xFF 0xFF 0xFF
 
 for (red = 0; red < 255; red++) { //increment brightness
  lcd.write(0xFE);
  lcd.write(0xD0); //sets backlight to red, green, and blue
  lcd.write(red); 
  lcd.write((uint8_t)0);
  lcd.write(255 - red);
  delay(10);  // give it some time to adjust the backlight!
 }   
 
 for (green = 0; green < 255; green++) {
  lcd.write(0xFE);
  lcd.write(0xD0);
  lcd.write(255-green); 
  lcd.write(green);
  lcd.write((uint8_t)0);
  delay(10);  // give it some time to adjust the backlight!
 }   
 
 for (blue = 0; blue < 255; blue++) {
  lcd.write(0xFE);
  lcd.write(0xD0);
  lcd.write((uint8_t)0); 
  lcd.write(255-blue);
  lcd.write(blue);
  delay(10);  // give it some time to adjust the backlight!
}   
}
