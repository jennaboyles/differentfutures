#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_DotStarMatrix.h>
#include <Adafruit_DotStar.h>


#define DATAPIN  11
#define CLOCKPIN 13

Adafruit_DotStarMatrix matrix = Adafruit_DotStarMatrix(
                                  (uint16_t)16, (uint16_t)16, 3, 1, DATAPIN, CLOCKPIN,
                                  DS_TILE_TOP   + DS_TILE_RIGHT   + DS_TILE_ROWS   + DS_TILE_PROGRESSIVE +
                                  DS_MATRIX_TOP + DS_MATRIX_RIGHT + DS_MATRIX_ROWS + DS_MATRIX_ZIGZAG,
                                  DOTSTAR_BRG);

//ADD YOUR MESSAGES HERE:
String text[] = {"Catalog for the Post Human", "Tim Parsons", "Jessica Charlesworth"};

//CHANGE MESSAGE SIZE TO THE AMOUNT OF MESSAGES
int messages_size = 2; //size of messages. start count at 0.

//ADD SOME COLORS HERE IN BRG SEQUENCE
const uint16_t colors[] = {
  matrix.Color(255, 0, 0), matrix.Color(0, 255, 0), matrix.Color(0, 0, 255)
};
//CHANGE YOUR "SPEED" HERE (~10-100):
int scroll_speed = 50;

int x  = matrix.width();
int message_length, i, j = 0;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  matrix.setTextWrap(false);
  matrix.setBrightness(10);
  matrix.setTextSize(2);
  matrix.setTextColor(matrix.Color(0, 0, 255));
}


void loop() {
  message_length = -text[i].length() * 12;
  matrix.fillScreen(0);
  matrix.setCursor(x, 0);
  matrix.print(text[i]);

  if (--x < message_length) {
    x = matrix.width();
    if (++j >= 3)
      j = 0;
    matrix.setTextColor(colors[j]);
    if (++i > messages_size)
      i = 0;
  }
  matrix.show();
  delay(scroll_speed);
}
