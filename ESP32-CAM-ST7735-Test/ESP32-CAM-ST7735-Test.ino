#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <SPI.h>             // Native SPI communications library

// TFT_CS, TFT_MOSI, TFT_SCLK are set to the default HSPI pins for the ESP32 Cam board.
// The preconfigured SCK, MISO, MOSI, SS macros cannot be used, because they point to the VSPI pins,
// and VSPI pins are not accessible on the ESP32 Cam board.
// https://randomnerdtutorials.com/esp32-spi-communication-arduino/

// TFT_DC and TFT_RST can be set to any available pins.
// Note, however, that pin 4 is used for the flashlight internally and should not be used for TFT_DC or TFT_RST.

#define TFT_CS   15 // Chip Select (HSPI)
#define TFT_DC   12 // Data Command / RS / A0
#define TFT_RST  2  // Reset
#define TFT_MOSI 13 // Master Out Slave In / SDA (HSPI)
#define TFT_SCLK 14 // Clock (HSPI)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup(void) {

  // Use this initializer if using a 1.8" TFT screen.
  tft.initR(INITR_BLACKTAB);

  tft.setTextWrap(true);

  // SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, but can be overridden.
  // Note that speed allowable depends on chip and quality of wiring. if you go too fast, you
  // Setting the frequency too high can result in black frames or a permanently black screen.
  // tft.setSPISpeed(40000000); // defaults to 40.000.000
}

int x = 0;
int y = 0;

bool x_inc = true;
bool y_inc = true;

int color_bg = ST77XX_BLACK;
int color_fg = ST77XX_WHITE;

void loop() {
  if((x == 0 && y == 0) || (x == tft.width() && y == 0) || (x == 0 && y == tft.height()) || (x == tft.width() && y == tft.height())) {
    if(color_bg == ST77XX_BLACK) { color_bg = ST77XX_WHITE; }
    else { color_bg = ST77XX_BLACK; }

    if(color_fg == ST77XX_BLACK) { color_fg = ST77XX_WHITE; }
    else { color_fg = ST77XX_BLACK; }

    tft.fillScreen(color_bg);
    tft.setTextColor(color_fg);
  }

  tft.drawPixel(x, y, color_bg);

  if(x == tft.width()) { x_inc = false; }
  if(y == tft.height()) { y_inc = false; }
  if(x == 0) { x_inc = true; }
  if(y == 0) { y_inc = true; }

  if(x_inc == true) { x++; }
  else { x--; }

  if(y_inc == true) { y++; }
  else { y--; }

  tft.drawPixel(x, y, color_fg);

  tft.setCursor(10, tft.height() / 2);
  tft.print("Wilde Brille");
}
