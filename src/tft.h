#ifndef TFT_H
#define TFT_H

#include <Arduino_GFX_Library.h>
#include <SPI.h>
#define TFT_CS 40 // GFX_NOT_DEFINED for display without CS pin
#define TFT_DC 41
#define TFT_RST 42
#define GFX_BL 2

#define DTFT_MOSI 35
#define TFT_SCLK 36
 

#define RGB_PANEL

//Arduino_DataBus *bus = new Arduino_ESP32SPI(TFT_DC, TFT_CS, 36 /* SCK */, 35 /* MOSI */, GFX_NOT_DEFINED /* MISO */, HSPI /* spi_num */);

Arduino_ESP32RGBPanel *rgbpanel = new Arduino_ESP32RGBPanel(
    40 /* DE */, 41 /* VSYNC */, 39 /* HSYNC */, 42 /* PCLK */,
    45 /* R0 */, 48 /* R1 */, 47 /* R2 */, 21 /* R3 */, 14 /* R4 */,
    5 /* G0 */, 6 /* G1 */, 7 /* G2 */, 15 /* G3 */, 16 /* G4 */, 4 /* G5 */,
    8 /* B0 */, 3 /* B1 */, 46 /* B2 */, 9 /* B3 */, 1 /* B4 */,
    0 /* hsync_polarity */, 8 /* hsync_front_porch */, 4 /* hsync_pulse_width */, 8 /* hsync_back_porch */,
    0 /* vsync_polarity */, 8 /* vsync_front_porch */, 4 /* vsync_pulse_width */, 8 /* vsync_back_porch */,
    1 /* pclk_active_neg */, 16000000 /* prefer_speed */);
Arduino_RGB_Display *gfx = new Arduino_RGB_Display(
    800 /* width */, 480 /* height */, rgbpanel, 0 /* rotation */, true /* auto_flush */);

void tftBacklight(boolean on){
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, on?HIGH:LOW);  
}

#endif