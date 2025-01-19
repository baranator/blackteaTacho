#ifndef BTMUI_H 
#define BTMUI_H

#include "lvgl.h"
void showMainScreen(lv_display_t * display);
void setSpeed(uint8_t val);
void setPower(int16_t val);
void setLight(bool on);
#endif