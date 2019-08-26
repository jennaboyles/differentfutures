/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/

#include <string.h>                  //from neopixel control
#include <Arduino.h>                 //from neopixel control
#include <SPI.h>                     //both
#include <Adafruit_GFX.h>            //from DotstarMatrixTest
#include <Adafruit_DotStarMatrix.h>  //from DotstarMatrixTest
#include <Adafruit_DotStar.h>        //from DotstarMatrixTest
#include <Fonts/TomThumb.h>          //from DotstarMatrixTest

#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)  //from neopixel control
  #include <SoftwareSerial.h>         //from neopixel control
#endif

//WHEN ANY OF THESE ARE UNCOMMENTED THERE IS AN ERROR COMPILING TO BOARD
//#include "Adafruit_BLE.h"             //from neopixel control // must comment out when using bluefruit.h?
//#include "Adafruit_BluefruitLE_SPI.h" //from neopixel control // must comment out when using bluefruit.h?
//#include "Adafruit_BluefruitLE_UART.h"//from neopixel control // must comment out when using bluefruit.h?

//#include "BluefruitConfig.h"         //from neopixel control ... had to delete the .h file for the code to upload

#include <bluefruit.h>                //from test_control not sure if needed... YES I think it is?
// OTA DFU service 
BLEDfu bledfu;    //from controller test
// Uart over BLE service
BLEUart bleuart;  //from controller test 

// Function prototypes for packetparser.cpp // from controller test
uint8_t readPacket (BLEUart *ble_uart, uint16_t timeout);
float   parsefloat (uint8_t *buffer);
void    printHex   (const uint8_t * data, const uint32_t numBytes);

// Packet buffer
extern uint8_t packetbuffer[]; // from controller test

#define DATAPIN    7
#define CLOCKPIN   16

#define SHIFTDELAY 100                   //from DotstarMatrixTest
#define BRIGHTNESS 20                    //from DotstarMatrixTest

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
                                 16, 32, DATAPIN, CLOCKPIN,
                                 DS_MATRIX_TOP     + DS_MATRIX_RIGHT +
                                 DS_MATRIX_ROWS + DS_MATRIX_ZIGZAG,
                                 DOTSTAR_BGR);

/*begin init code for color cycle & Adafruit text test from DotstarMatrixTest===*/                           

const uint16_t primaryColors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};

//const uint16_t adaColors[] = {
//  matrix.Color(255, 0, 0),   //A red
//  matrix.Color(255, 125, 0), //D orange
//  matrix.Color(200, 255, 0), //A yellowish
//  matrix.Color(0, 255, 0),   //F green
//  matrix.Color(0, 255, 225), //R blue
//  matrix.Color(150, 0, 255), //U purple
//  matrix.Color(255, 0, 220), //I pink
//  matrix.Color(255, 65, 0),  //T reddish
//  matrix.Color(255, 220, 0)  //! orange/yellow
//};

char adafruit[] = "ADAFRUIT!";
/*end init code for color cycle & Adafruit text test from DotstarMatrixTest=== */


/*=========================================================================
    APPLICATION SETTINGS // from neopixel animation control

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    -----------------------------------------------------------------------*/
  #define FACTORYRESET_ENABLE     1  
///*=========================================================================*/
//
//// Create the bluefruit object, either software serial...uncomment these lines
///*
//SoftwareSerial bluefruitSS = SoftwareSerial(BLUEFRUIT_SWUART_TXD_PIN, BLUEFRUIT_SWUART_RXD_PIN);
//
//Adafruit_BluefruitLE_UART ble(bluefruitSS, BLUEFRUIT_UART_MODE_PIN,
//                      BLUEFRUIT_UART_CTS_PIN, BLUEFRUIT_UART_RTS_PIN);
//*/
//
///* ...or hardware serial, which does not need the RTS/CTS pins. Uncomment this line */
//// Adafruit_BluefruitLE_UART ble(BLUEFRUIT_HWSERIAL_NAME, BLUEFRUIT_UART_MODE_PIN);
//
///* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST - THIS LINE UNCOMMENTED IN NEOPIXEL EXAMPLE*/ 
//Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
//
///* ...software SPI, using SCK/MOSI/MISO user-defined SPI pins and then user selected CS/IRQ/RST */
////Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_SCK, BLUEFRUIT_SPI_MISO,
////                             BLUEFRUIT_SPI_MOSI, BLUEFRUIT_SPI_CS,
////                             BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
//
//// A small helper // from neopixel control 
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}
//
// the packet buffer // from neopixel control
extern uint8_t packetbuffer[];
//
///* begin copied code from neopixel control */
///**************************************************************************/
///*! 
//    @brief  Sets up the HW an the BLE module (this function is called
//            automatically on startup)
//*/
///**************************************************************************/
//additional variables

//Color 
    uint8_t red = 100;      //from neopixel control 
    uint8_t green = 100;    //from neopixel control 
    uint8_t blue = 100;     //from neopixel control 
uint8_t animationState = 1; //from neopixel control 

    int pos = 0, dir = 1;   //Position, direction of "eye" for larson scanner animation 
/* end copied code from neopixel control ******************************/

void setup() //(void)
{
//  delay(500);                //from neopixel control
  Serial.begin(115200);      
//
//// uncomment to have wait
////while (!Serial) delay(500);
//
//
///* begin from DotstarMatrixTest*/
// Serial.println("\nDotstar Matrix Wing & Bluefruit Controller App Test");   
// 
  matrix.begin();            
  matrix.setFont(&TomThumb);
  matrix.setTextWrap(false);
  matrix.setBrightness(BRIGHTNESS);

  for (byte i = 0; i < 3; i++) {
    matrix.fillScreen(primaryColors[i]);
    matrix.show();          
    delay(500);
//
//////test A : begin code I added to try to turn off the LEDS.
for (byte i = 0; i < 3; i++) {
  matrix.fillScreen(0);
  matrix.show();          //turn all LEDS off ... 
  delay(500);
 }

colorWipe(matrix.Color(100, 100, 100), 20); // do a quick colorWipe to show that the pixels are all working, even before Bluefruit connection established
  colorWipe(matrix.Color(0, 0, 0), 20); 
  matrix.show();
  
//...end code I added

}
///* end from DotstarMatrixTest*/
//
///* begin copied code from controller test... probably don't need*/
////  Bluefruit.begin();
////  Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
////  Bluefruit.setName("Bluefruit52");
////
////  // To be consistent OTA DFU should be added first if it exists
////  bledfu.begin();
////
////  // Configure and start the BLE Uart service
////  bleuart.begin();
////
////  // Set up and start advertising
////  startAdv();
////
////  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
////  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
////  Serial.println();  
///* end copied code from controller test... probably don't need*/
//
///* begin copied code from neopixel control */
///**************************************************************************/
///* Initialise the module */
//  Serial.print(F("Initialising the Bluefruit LE module: "));
//
//  if ( !ble.begin(VERBOSE_MODE) )
//  {
//    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
//  }
//  Serial.println( F("OK!") );
//
//  if ( FACTORYRESET_ENABLE )
//  {
//    /* Perform a factory reset to make sure everything is in a known state */
//    Serial.println(F("Performing a factory reset: "));
//    if ( ! ble.factoryReset() ){
//      error(F("Couldn't factory reset"));
//    }
//  }
//
//  /* Disable command echo from Bluefruit */
//  ble.echo(false);
//
//  Serial.println("Requesting Bluefruit info:");
//  /* Print Bluefruit information */
//  ble.info();
//
//  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in Controller mode"));
//  Serial.println(F("Then activate/use the sensors, color picker, game controller, etc!"));
//  Serial.println();
//
//  ble.verbose(false);  // debug info is a little annoying after this point!
//
//  /* Wait for connection */
//  while (! ble.isConnected()) {
//      delay(500);
//  }
//
//  Serial.println(F("***********************"));
//
//  // Set Bluefruit to DATA mode
//  Serial.println( F("Switching to DATA mode!") );
//  ble.setMode(BLUEFRUIT_MODE_DATA);
//
//  Serial.println(F("***********************"));  
///**************************************************************************/
///* end copied code from neopixel control */
//
} //end bracket for setup function
// 
int x = matrix.width();  //from DotstarMatrixTest
int pass = 0;            //from DotstarMatrixTest
//
////// Runs 10 LEDs at a time along strip, cycling through red, green and blue.   //from Dotstar Strip Test
////// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel. //from Dotstar Strip Test
////int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels       //from Dotstar Strip Test
////uint32_t color = 0xFF0000;      // 'On' color (starts red)                    //from Dotstar Strip Test
//
void loop() //(void)
{
//
//// Wait for new data to arrive (comment out one)
//// uint8_t len = readPacket(&bleuart, 500);               //controller_test 
// uint8_t len = readPacket(&ble, BLE_READPACKET_TIMEOUT);  //neopixel_test
// if (len == 0) return;                            //commented out in controller_test but not neopixel control
//
// /* Got a packet! */
//  // printHex(packetbuffer, len);                 //commented out in both controller_test & neopixel control
//
//  // Color
//  if (packetbuffer[1] == 'C') {
//    red = packetbuffer[2];
//    green = packetbuffer[3];
//    blue = packetbuffer[4];
//    Serial.print ("RGB #");
//    if (red < 0x10) Serial.print("0");
//    Serial.print(red, HEX);
//    if (green < 0x10) Serial.print("0");
//    Serial.print(green, HEX);
//    if (blue < 0x10) Serial.print("0");
//    Serial.println(blue, HEX);
//
///*added from neopixel_anim_control but changed pixel to matrix... and NUMPIXELS NEEDS TO BE CHANGED*/
////// this part colors ALL the pixels according to the app's color picker:   
////      for(uint8_t i=0; i<NUMPIXELS; i++) {                               //this is formatted for Neopixels
////      pixel.setPixelColor(i, pixel.Color(red,green,blue));
////   }
////    matrix.show(); // This sends the updated color to the hardware.
//  }
//
///*begin code for Bluetooth control used for neopixel anim and Dotstar Strip Test */
  // Buttons
  if (packetbuffer[1] == 'B') {
 
    uint8_t buttnum = packetbuffer[2] - '0';
    boolean pressed = packetbuffer[3] - '0';
    Serial.print ("Button "); Serial.print(buttnum);
//   animationState = buttnum;                      //animationState from neopixel control
    if (pressed) {
      Serial.println(" pressed");
    } else {
      Serial.println(" released");
    }
  }
///*end code for Bluetooth control used for neopixel control and Dotstar Strip Test */

/*begin a bunch of if statements from neopixel control... need to be reformatte for DotStars */
if (animationState == 1){ // button labeled "1" in control pad
    for(uint16_t i=0; i<matrix.numPixels(); i++) { //clear all pixels before displaying new animation
          matrix.setPixelColor(i, matrix.Color(0,0,0));
        }
     flashRandom(5,random(10,30));
     matrix.show(); // This sends the updated pixel color to the hardware.
   }
  
  if (animationState == 2){ // button labeled "2" in control pad
    for(uint16_t i=0; i<matrix.numPixels(); i++) { //clear all pixels before displaying new animation
          matrix.setPixelColor(i, matrix.Color(0,0,0));
        }
    colorWipe(matrix.Color(red, green, blue), 20);
    matrix.show(); // This sends the updated pixel color to the hardware.
    colorWipe(matrix.Color(0, 0, 0), 20);
    matrix.show();
  }

  if (animationState == 3){ // button labeled "3" in control pad
    for(uint16_t i=0; i<matrix.numPixels(); i++) { //clear all pixels before displaying new animation
          matrix.setPixelColor(i, matrix.Color(0,0,0));
        }
    larsonScanner(30); // larsonScanner is set to red and does not take color input.
    matrix.show(); // This sends the updated pixel color to the hardware.
  }
  
  if (animationState == 4){ // button labeled "4" in control pad
    for(uint16_t i=0; i<matrix.numPixels(); i++) { //clear all pixels before displaying new animation
          matrix.setPixelColor(i, matrix.Color(0,0,0));
        }
    rainbowCycle(20);
    matrix.show(); // This sends the updated pixel color to the hardware.
  }
///*end a bunch of if statements from neopixel control... need to be reformatte for DotStars */
//
  matrix.fillScreen(0);                           //from DotstarMatrixTest
  matrix.setCursor(x, 5);                         //from DotstarMatrixTest

//  // for loop from DotstarMatrixTest
//  for (byte i = 0; i < strlen(adafruit); i++) {    
//    // set the color
//    matrix.setTextColor(adaColors[i]);
//    // write the letter
//    matrix.print(adafruit[i]);
//  }

  //from DotstarMatrixTest
  if (--x < -50) {
    x = matrix.width();
  }

  matrix.show();                                //from DotstarMatrixTest
  delay(SHIFTDELAY);                            //from DotstarMatrixTest
//
//
///*begin added code from controller_test...probably don't need*/
/////**************************************************************************/
////  // GPS Location
////  if (packetbuffer[1] == 'L') {
////    float lat, lon, alt;
////    lat = parsefloat(packetbuffer+2);
////    lon = parsefloat(packetbuffer+6);
////    alt = parsefloat(packetbuffer+10);
////    Serial.print("GPS Location\t");
////    Serial.print("Lat: "); Serial.print(lat, 4); // 4 digits of precision!
////    Serial.print('\t');
////    Serial.print("Lon: "); Serial.print(lon, 4); // 4 digits of precision!
////    Serial.print('\t');
////    Serial.print(alt, 4); Serial.println(" meters");
////  }
////
////  // Accelerometer
////  if (packetbuffer[1] == 'A') {
////    float x, y, z;
////    x = parsefloat(packetbuffer+2);
////    y = parsefloat(packetbuffer+6);
////    z = parsefloat(packetbuffer+10);
////    Serial.print("Accel\t");
////    Serial.print(x); Serial.print('\t');
////    Serial.print(y); Serial.print('\t');
////    Serial.print(z); Serial.println();
////  }
////
////  // Magnetometer
////  if (packetbuffer[1] == 'M') {
////    float x, y, z;
////    x = parsefloat(packetbuffer+2);
////    y = parsefloat(packetbuffer+6);
////    z = parsefloat(packetbuffer+10);
////    Serial.print("Mag\t");
////    Serial.print(x); Serial.print('\t');
////    Serial.print(y); Serial.print('\t');
////    Serial.print(z); Serial.println();
////  }
////
////  // Gyroscope
////  if (packetbuffer[1] == 'G') {
////    float x, y, z;
////    x = parsefloat(packetbuffer+2);
////    y = parsefloat(packetbuffer+6);
////    z = parsefloat(packetbuffer+10);
////    Serial.print("Gyro\t");
////    Serial.print(x); Serial.print('\t');
////    Serial.print(y); Serial.print('\t');
////    Serial.print(z); Serial.println();
////  }
////
////  // Quaternions
////  if (packetbuffer[1] == 'Q') {
////    float x, y, z, w;
////    x = parsefloat(packetbuffer+2);
////    y = parsefloat(packetbuffer+6);
////    z = parsefloat(packetbuffer+10);
////    w = parsefloat(packetbuffer+14);
////    Serial.print("Quat\t");
////    Serial.print(x); Serial.print('\t');
////    Serial.print(y); Serial.print('\t');
////    Serial.print(z); Serial.print('\t');
////    Serial.print(w); Serial.println();
////  }
/////*********************************************************************/
///*end added code from controller_test...probably don't need*/
//
//
} // end bracket for loop function
//
//
///* begin more copied code from controller test... probably don't need*/
////void startAdv(void)
////{
////  // Advertising packet
////  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
////  Bluefruit.Advertising.addTxPower();
////  
////  // Include the BLE UART (AKA 'NUS') 128-bit UUID
////  Bluefruit.Advertising.addService(bleuart);
////
////  // Secondary Scan Response packet (optional)
////  // Since there is no room for 'Name' in Advertising packet
////  Bluefruit.ScanResponse.addName();
////
////  /* Start Advertising
////   * - Enable auto advertising if disconnected
////   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
////   * - Timeout for fast mode is 30 seconds
////   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
////   * 
////   * For recommended advertising interval
////   * https://developer.apple.com/library/content/qa/qa1931/_index.html   
////   */
////  Bluefruit.Advertising.restartOnDisconnect(true);
////  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
////  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
////  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds  
////}
///* end copied code from controller test... probably don't need*/
//


//*begin a bunch of functions from neopixel control... need to be reformatted for DotStars */
// Fill the dots one after the other with a color
// THIS WORKED 
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<matrix.numPixels(); i++) {
  matrix.setPixelColor(i, c);
  matrix.show();
  delay(wait);
 }
}

void larsonScanner(uint8_t wait){
  int j;
  for(uint16_t i=0; i<matrix.numPixels()+5; i++) {
  // Draw 5 pixels centered on pos.  setPixelColor() will clip any
  // pixels off the ends of the strip, we don't need to watch for that.
  matrix.setPixelColor(pos - 2, 0x100000); // Dark red
  matrix.setPixelColor(pos - 1, 0x800000); // Medium red
  matrix.setPixelColor(pos    , 0xFF3000); // Center pixel is brightest
  matrix.setPixelColor(pos + 1, 0x800000); // Medium red
  matrix.setPixelColor(pos + 2, 0x100000); // Dark red

  matrix.show();
  delay(wait);

// Rather than being sneaky and erasing just the tail pixel,
// it's easier to erase it all and draw a new one next time.
for(j=-2; j<= 2; j++) matrix.setPixelColor(pos+j, 0);

// Bounce off ends of strip
pos += dir;
if(pos < 0) {
pos = 1;
dir = -dir;
} else if(pos >= matrix.numPixels()) {
pos = matrix.numPixels() - 2;
 dir = -dir;
 } 
 } 
}
//
void flashRandom(int wait, uint8_t howmany) {
 randomSeed(analogRead(0));
  for(uint16_t i=0; i<howmany; i++) {
    // get a random pixel from the list
    int j = random(matrix.numPixels());
    
    // now we will 'fade' it in 5 steps
    for (int x=0; x < 5; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      
      matrix.setPixelColor(j, matrix.Color(r, g, b));
      matrix.show();
      delay(wait);
    }
    // & fade out in 5 steps
    for (int x=5; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      
      matrix.setPixelColor(j, matrix.Color(r, g, b));
      matrix.show();
      delay(wait);
    }
  }
  // LEDs will be off when done (they are faded to 0)
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<matrix.numPixels(); i++) {
      matrix.setPixelColor(i, Wheel((i+j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

//Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< matrix.numPixels(); i++) {
      matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}

//////Theatre-style crawling lights.
////void theaterChase(uint32_t c, uint8_t wait) {
////  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
////    for (int q=0; q < 3; q++) {
////      for (int i=0; i < pixel.numPixels(); i=i+3) {
////        pixel.setPixelColor(i+q, c);    //turn every third pixel on
////      }
////      pixel.show();
////
////      delay(wait);
////
////      for (int i=0; i < pixel.numPixels(); i=i+3) {
////        pixel.setPixelColor(i+q, 0);        //turn every third pixel off
////      }
////    }
////  }
////}
//
//////Theatre-style crawling lights with rainbow effect
////void theaterChaseRainbow(uint8_t wait) {
////  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
////    for (int q=0; q < 3; q++) {
////      for (int i=0; i < pixel.numPixels(); i=i+3) {
////        pixel.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
////      }
////      pixel.show();
////
////      delay(wait);
////
////      for (int i=0; i < pixel.numPixels(); i=i+3) {
////        pixel.setPixelColor(i+q, 0);        //turn every third pixel off
////      }
////    }
////  }
////}
///*end a bunch of functions from neopixel control... need to be reformatted for DotStars */
//
//
////code below from neopixel control ... not sure if will work with pixel replaced with matrix
//// Input a value 0 to 255 to get a color value.
//// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
