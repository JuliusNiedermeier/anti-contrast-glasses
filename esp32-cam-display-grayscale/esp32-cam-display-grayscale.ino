#include "esp_camera.h"
#include "camera_index.h"

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

camera_fb_t *frame_buffer = NULL;
static esp_err_t cam_err;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// Display pins
#define TFT_CS 15    // Chip Select (HSPI)
#define TFT_DC 12    // Data Command / RS / A0
#define TFT_RST 2    // Reset
#define TFT_MOSI 13  // Master Out Slave In / SDA (HSPI)
#define TFT_SCLK 14  // Clock (HSPI)

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(115200);

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;

  config.frame_size = FRAMESIZE_QQVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;

  // camera init
  cam_err = esp_camera_init(&config);
  if (cam_err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", cam_err);
    return;
  }

  sensor_t *cam_sensor = esp_camera_sensor_get();
  cam_sensor->set_framesize(cam_sensor, FRAMESIZE_QQVGA);
  cam_sensor->set_vflip(cam_sensor, 0);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
}

void loop() {
  int start_millis = millis();

  frame_buffer = esp_camera_fb_get();

  if (!frame_buffer) {
    Serial.println("Camera capture failed");
  }

  int width = frame_buffer->width;
  int height = frame_buffer->height;
  uint8_t *image_pointer = frame_buffer->buf;
  uint16_t color;

  for (int y = 0; y < height; y++) {
    for(int x = 0; x < width; x++) {
      uint8_t pixel = *image_pointer;
      color = tft.color565(pixel, pixel, pixel);
      tft.drawPixel(x, y, color);
      image_pointer += 1;
    }
  }

  esp_camera_fb_return(frame_buffer);
}

word ConvertRGB(byte R, byte G, byte B) {
  return (((R & 0xF8) << 8) | ((G & 0xFC) << 3) | (B >> 3));
}
