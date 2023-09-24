#include <TFT.h>;
#include <SPI.h>;

#define cs 10
#define dc 9
#define rst 8

TFT TFTscreen = TFT(cs, dc, rst);

void setup() {
  TFTscreen.begin();
}

void loop() {
  TFTscreen.background(0, 0, 0);
  delay(1000);
  TFTscreen.background(255, 255, 255);
  delay(1000);
}
