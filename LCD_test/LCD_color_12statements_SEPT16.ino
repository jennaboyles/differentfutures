#include "Arduino.h"
#include <SoftwareSerial.h>
SoftwareSerial lcd = SoftwareSerial(0, 2);

uint16_t *settings[] = {0x58, 0x48, 0x51, 0x54, 0x54, 0x4B};
String message[] = {"Your stablecoin options are     marked down -5  points. Your    life adviser    recommends 6    hours overtime",
                    "Todays goals:   11/37 complete",
                    "Last flow state:18 mins 10 secs", 
                    "Family cohesion is up 3%",
                    "Your social     media influence is up 0.07% ",
                    "Three close     friends profile contain themes  identified as   \"dangerous\". ",
                    "MumsTheWord     logged 8        microaggressionsin your         professional    communications  last month. ",
                    "Your resting    heart rate is   high. Take      ashwagandha rootfrom your       Nootdial. ",
                    "Your resume was read by 3752    bots today.     Interviews      pending:     0 ",
                    "Your clickstreamanalysis shows  signs of apathy.Facetime with   Marei. ",
                    "Cortisol level  is higher than  average. Programforest therapy  during down time ",
                    "Your WorkDate   drink this week is with Dave C  and Erykah S.   Your            conversation    topics are      \"Learning       Spanish\" and    \"books you own  but             haven't read\". "
                   };
int delay_between_messages = 4000; //delay between messages in milliseconds.
int delay_between_characters = 50; //delay between characters in milliseconds.

uint8_t red, green, blue;

void setup() {
  lcd.begin(9600);
  for (int i = 0; i < 5; i++) {
    lcd.write(0xFE);
    lcd.write(settings[i]);
    delay(10);
  }
  delay(1000);
}

void loop() {
  //go through all the messages
  for (int i = 0; i < 9; i++) {
    //disable cursor
    lcd.write(0xFE);
    lcd.write(0x54);
    delay(10);
    //display each message
    for (int j = 0; j < message[i].length(); j++) {
      lcd.print(message[i].charAt(j));
      delay(delay_between_characters);
    }
    //wait one second
    delay(1000);
    //cycle through the colors
    colorBackground();
    //clear the screen
    lcd.write(0xFE);
    lcd.write(0x58);
    delay(50);
    //enable cursor
    lcd.write(0xFE);
    lcd.write(0x53);
    delay(10);
    //wait on a blank screen for a few seconds.
    delay(delay_between_messages);
  }
}

//background color cycle
void colorBackground(){
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
