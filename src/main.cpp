#include "tft.h"
#include "btmlogo.h"
#include "btmui.h"
#include "sound.h"

#include "lvgl.h"
    
#include "touch.h"


 



uint32_t screenW;
uint32_t screenH;
unsigned long tl;

uint32_t bufSize;



lv_display_t *disp;
lv_color_t *disp_draw_buf;
lv_color_t *disp_draw_buf2;




#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif


uint32_t millis_cb(void){
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map){
#ifndef DIRECT_MODE
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);
#endif // #ifndef DIRECT_MODE

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data){
  if (touch_has_signal()){
    if (touch_touched()){
      data->state = LV_INDEV_STATE_PRESSED;

      /*Set the coordinates*/
      data->point.x = touch_last_x;
      data->point.y = touch_last_y;
    }else if (touch_released()){
      data->state = LV_INDEV_STATE_RELEASED;
    }
  }else{
    data->state = LV_INDEV_STATE_RELEASED;
  }
}

void setup(){
  tl=millis();
  tftBacklight(true);
  Serial.begin(115200);
  //Serial.setDebugOutput(true);
  //while(!Serial);
  //Serial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  //Serial.println(LVGL_Arduino);

  // Init Display
  if (!gfx->begin())
  {
    //Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);


  // Init touch device
  touch_init();

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenW = gfx->width();
  screenH = gfx->height();

  bufSize = screenW * screenH / 4;


  disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (!disp_draw_buf){
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
  if (!disp_draw_buf2){
    // remove MALLOC_CAP_INTERNAL flag try again
    disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_8BIT);
  }
  

  if (!disp_draw_buf || !disp_draw_buf2){
    //Serial.println("LVGL disp_draw_buf allocate failed!");
  }else{
    disp = lv_display_create(screenW, screenH);
     
   


    lv_display_set_flush_cb(disp, my_disp_flush);

    lv_display_set_buffers(disp, disp_draw_buf, NULL, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);

    /*Initialize the (dummy) input device driver*/
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);


    showMainScreen(disp);
  }
  soundSetup();
  //Serial.println("Setup done");
}


void loop(){
  lv_task_handler(); /* let the GUI do its work */
  //soundLoop();
#ifdef CANVAS
  gfx->flush();
#endif

  if(true && millis()-tl>500){
    setSpeed(random(30,110));
    //soundLoop();
    setPower(random(-90,90));
    //soundLoop();
    setLight(random(0,100)>70);
    //soundLoop();
    tl=millis();
  }
  //soundLoop();
  delay(5);


}