#include "lvgl.h"
#include "btmui.h"
#include "SerialDebug.h"
#include "../.pio/libdeps/esp32-s3/lvgl/examples/lv_examples.h"



LV_FONT_DECLARE(brandon_BI_40);
LV_FONT_DECLARE(brandon_BI_50);
LV_FONT_DECLARE(brandon_BI_150);
LV_FONT_DECLARE(googleMat40);

static lv_style_t t_speed_style;
    static lv_style_t t_unit_style;
static lv_style_t pm_bg_style;
static lv_style_t pm_ind_style;

lv_obj_t *arc, *pm_arc;
lv_obj_t * t_unit ;
lv_obj_t * t_speed;
lv_obj_t * bar_temp_motor;

void setSpeed(uint8_t val){
      lv_arc_set_value(arc, val);
      char str[5];
      sprintf(str, "%d", val);
      lv_label_set_text(t_speed,str);

}

void setPower(int16_t val){
      lv_arc_set_value(pm_arc, val);
          if(val<0){
      lv_style_set_arc_color(&pm_ind_style,lv_color_hex(0x00ff00));
    }else{
      lv_style_set_arc_color(&pm_ind_style,lv_color_hex(0xff0000));
    }
    lv_obj_add_style(pm_arc, &pm_ind_style, LV_PART_INDICATOR);
    //lv_label_set_text(pm_label,"x");
   // lv_arc_align_obj_to_angle(pm_arc, pm_label, 25);
}



void drawPowerMeter(int x=0, int y=0, int rad=360){
     /*Create an Arc*/
    pm_arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(pm_arc, rad-80, rad-80);
    lv_arc_set_rotation(pm_arc, 135);
    lv_arc_set_range(pm_arc, -100, 100);
    lv_arc_set_bg_angles(pm_arc, 0, 90);
    lv_arc_set_value(pm_arc, 0);
    lv_obj_center(pm_arc);
    lv_obj_align(pm_arc, LV_ALIGN_CENTER, 0+x, 0+y);
   // lv_obj_add_event_cb(pm_arc, pm_value_changed_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_arc_set_mode(pm_arc,LV_ARC_MODE_SYMMETRICAL);
    //   lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED,NULL);
    /*Manually update the label for the first time*/
    //lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED, NULL);
    
    //background
    lv_style_set_arc_color(&pm_bg_style,lv_color_hex(0x222222));
    lv_style_set_arc_width(&pm_bg_style,20);
    lv_style_set_arc_rounded(&pm_bg_style,false);
    lv_obj_add_style(pm_arc, &pm_bg_style, LV_PART_MAIN);

    //remove knob
    static lv_style_t my_style;
    //lv_style_set_bg_opa (&my_style,0);
    lv_obj_remove_style(pm_arc, NULL, LV_PART_KNOB);
    lv_obj_add_style(pm_arc, &my_style, LV_PART_KNOB);

    //indicator
    
    lv_style_set_pad_bottom(&pm_ind_style,0);
    lv_style_set_pad_top(&pm_ind_style,0);
    lv_style_set_arc_color(&pm_ind_style,lv_color_hex(0xff0000));
    lv_style_set_arc_width(&pm_ind_style,20);
    lv_style_set_arc_rounded(&pm_ind_style,false);
    
    lv_obj_add_style(pm_arc, &pm_ind_style, LV_PART_INDICATOR);

    //readonly
    lv_obj_remove_flag(pm_arc, LV_OBJ_FLAG_CLICKABLE);

    //text in tacho - speed & unit



}

void drawTacho(int x=0, int y=0, int rad=360){
     /*Create an Arc*/
    arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(arc, rad, rad);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_range(arc, 0, 130);
    lv_arc_set_bg_angles(arc, 0, 250);
    lv_arc_set_value(arc, 10);
    lv_obj_center(arc);
    lv_obj_align(arc, LV_ALIGN_CENTER, 0+x, 0+y);
    //   lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED,NULL);
    /*Manually update the label for the first time*/
    //lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED, NULL);
    
    //background
    static lv_style_t bg_style;
    lv_style_set_arc_color(&bg_style,lv_color_hex(0x555555));
    lv_style_set_arc_width(&bg_style,40);
    lv_style_set_arc_rounded(&bg_style,false);
    lv_obj_add_style(arc, &bg_style, LV_PART_MAIN);

    //remove knob
    static lv_style_t my_style;
    //lv_style_set_bg_opa (&my_style,0);
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);
    lv_obj_add_style(arc, &my_style, LV_PART_KNOB);

    //indicator
    static lv_style_t ind_style;
    lv_style_set_pad_bottom(&ind_style,0);
    lv_style_set_pad_top(&ind_style,0);
    lv_style_set_arc_color(&ind_style,lv_color_hex(0xffffff));
    lv_style_set_arc_width(&ind_style,40);
    lv_style_set_arc_rounded(&ind_style,false);
    lv_obj_add_style(arc, &ind_style, LV_PART_INDICATOR);

    //readonly
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    //text in tacho - speed & unit
    t_speed = lv_label_create(lv_screen_active());
    
    lv_obj_align(t_speed, LV_ALIGN_CENTER, 0+x, 0+y);
    
    lv_style_init(&t_speed_style);
    lv_style_set_text_font(&t_speed_style, &brandon_BI_150);
    lv_style_set_text_color(&t_speed_style,lv_color_hex(0xffffff));
    lv_obj_add_style(t_speed, &t_speed_style, 0);
    lv_label_set_text(t_speed,"187");

    t_unit = lv_label_create(lv_screen_active());
    
    lv_obj_align(t_unit, LV_ALIGN_CENTER, 0+x, 75+y);
    lv_style_init(&t_unit_style);
    lv_style_set_text_font(&t_unit_style, &brandon_BI_40);
    lv_style_set_text_color(&t_unit_style,lv_color_hex(0x999999));
    lv_obj_add_style(t_unit, &t_unit_style, 0);
    lv_label_set_text(t_unit,"km/h");


}


void drawTachoV2(int x=0, int y=0, int rad=360){
  lv_obj_t * scale = lv_scale_create(lv_screen_active());
  lv_obj_set_size(scale, rad-20, rad-20);
  lv_scale_set_label_show(scale, true);
  lv_scale_set_mode(scale, LV_SCALE_MODE_ROUND_OUTER);
  lv_obj_center(scale);
  
    lv_obj_align(scale, LV_ALIGN_CENTER, 0+x, 0+y);

    lv_scale_set_total_tick_count(scale, 15);
    lv_scale_set_major_tick_every(scale, 5);

    lv_obj_set_style_length(scale, 15, LV_PART_ITEMS);
    lv_obj_set_style_length(scale, 13, LV_PART_INDICATOR);
    lv_scale_set_range(scale, 0, 140);

    static const char * custom_labels[] = {"0", "50", "100"};
    lv_scale_set_text_src(scale, custom_labels);

    static lv_style_t indicator_style;
    lv_style_init(&indicator_style);

    /* Label style properties */
    lv_style_set_text_font(&indicator_style, LV_FONT_DEFAULT);
    lv_style_set_text_color(&indicator_style, lv_color_hex(0xffffff));

    /* Major tick properties */
    lv_style_set_line_color(&indicator_style, lv_color_hex(0x000000));
    lv_style_set_width(&indicator_style, 12U);      /*Tick length*/
    lv_style_set_line_width(&indicator_style, 8);  /*Tick width*/
    lv_obj_add_style(scale, &indicator_style, LV_PART_INDICATOR);

    static lv_style_t minor_ticks_style;
    lv_style_init(&minor_ticks_style);
    lv_style_set_line_color(&minor_ticks_style, lv_color_hex(0x000000));
    lv_style_set_width(&minor_ticks_style, 20U);         /*Tick length*/
    lv_style_set_line_width(&minor_ticks_style, 2U);    /*Tick width*/
    lv_obj_add_style(scale, &minor_ticks_style, LV_PART_ITEMS);

    static lv_style_t main_line_style;
    lv_style_init(&main_line_style);
    /* Main line properties */
    lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
    lv_style_set_arc_width(&main_line_style, 0); /*Tick width*/
    lv_obj_add_style(scale, &main_line_style, LV_PART_MAIN);


/*
    // Add a section 
    // static lv_style_t section_minor_tick_style;
    static lv_style_t section_label_style;
    static lv_style_t section_main_line_style;

    lv_style_init(&section_label_style);
    lv_style_init(&section_minor_tick_style);
    lv_style_init(&section_main_line_style);

    //Label style properties 
    lv_style_set_text_font(&section_label_style, LV_FONT_DEFAULT);
    lv_style_set_text_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));

    lv_style_set_line_color(&section_label_style, lv_palette_darken(LV_PALETTE_RED, 3));
    lv_style_set_line_width(&section_label_style, 5U); //Tick width

    lv_style_set_line_color(&section_minor_tick_style, lv_palette_lighten(LV_PALETTE_RED, 2));
    lv_style_set_line_width(&section_minor_tick_style, 4U); //Tick width

    // Main line properties
    lv_style_set_arc_color(&section_main_line_style, lv_palette_darken(LV_PALETTE_RED, 3));
    lv_style_set_arc_width(&section_main_line_style, 4U); //Tick width
    */
    

    /* Configure section styles */
   /* lv_scale_section_t * section = lv_scale_add_section(scale);
    lv_scale_section_set_range(section, 75, 100);
    lv_scale_section_set_style(section, LV_PART_INDICATOR, &section_label_style);
    lv_scale_section_set_style(section, LV_PART_ITEMS, &section_minor_tick_style);
    lv_scale_section_set_style(section, LV_PART_MAIN, &section_main_line_style);*/

}

#define CANVAS_WIDTH  50
#define CANVAS_HEIGHT  50
void lv_example_canvas_5(void){
    /*Create a buffer for the canvas*/
    // LV_DRAW_BUF_DEFINE_STATIC(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
    // LV_DRAW_BUF_INIT_STATIC(draw_buf);

    // /*Create a canvas and initialize its palette*/
    // lv_obj_t * canvas = lv_canvas_create(lv_screen_active());
    // lv_canvas_set_draw_buf(canvas, &draw_buf);
    // lv_canvas_fill_bg(canvas, lv_color_hex3(0xccc), LV_OPA_COVER);
    // lv_obj_center(canvas);

    // lv_layer_t layer;
    // lv_canvas_init_layer(canvas, &layer);

    // lv_vector_dsc_t * dsc = lv_vector_dsc_create(&layer);
    // lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

    // lv_fpoint_t pts[] = {{10, 10}, {130, 130}, {10, 130}};
    // lv_vector_path_move_to(path, &pts[0]);
    // lv_vector_path_line_to(path, &pts[1]);
    // lv_vector_path_line_to(path, &pts[2]);
    // lv_vector_path_close(path);

    // lv_vector_dsc_set_fill_color(dsc, lv_color_make(0x00, 0x80, 0xff));
    // lv_vector_dsc_add_path(dsc, path);

    // lv_draw_vector(dsc);
    // lv_vector_path_delete(path);
    // lv_vector_dsc_delete(dsc);

    //lv_canvas_finish_layer(canvas, &layer);

}

void drawTempBars(int x=0,int y=0){
  static lv_style_t style_indic;
  static lv_style_t style_bg;
    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_color_hex(0xffffff));
    lv_style_set_radius(&style_indic,0);
    
    lv_style_init(&style_bg);
    lv_style_set_border_side(&style_bg, LV_BORDER_SIDE_TOP);
    lv_style_set_border_color(&style_bg, lv_color_hex(0xff0000));
    lv_style_set_border_width(&style_bg, 2);
    lv_style_set_pad_all(&style_bg, 0); /*To make the indicator smaller*/
    lv_style_set_radius(&style_bg, 0);
    
    


    bar_temp_motor = lv_bar_create(lv_screen_active());

    

    lv_obj_remove_style_all(bar_temp_motor);  /*To have a clean start*/
    lv_obj_add_style(bar_temp_motor, &style_bg, 0);
    lv_obj_add_style(bar_temp_motor, &style_indic, LV_PART_INDICATOR);
  
  //lv_obj_center(bar_temp_motor)
    lv_bar_set_orientation(bar_temp_motor, LV_BAR_ORIENTATION_VERTICAL);
    lv_obj_align(bar_temp_motor, LV_ALIGN_BOTTOM_LEFT, 10+x, y-30);
    lv_bar_set_range(bar_temp_motor, 0, 100);
    lv_obj_set_size(bar_temp_motor, 20, 200);
    lv_bar_set_value(bar_temp_motor, 21, LV_ANIM_OFF);

}

void drawTabView(void){
    /*Create a Tab view object*/
    lv_obj_t * tabview;
    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, 70);

    lv_obj_set_style_bg_color(tabview, lv_color_hex(0x000000), 0);

    lv_obj_t * tab_buttons = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_buttons, lv_palette_darken(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_text_color(tab_buttons, lv_palette_lighten(LV_PALETTE_GREY, 5), 0);
    lv_obj_set_style_border_side(tab_buttons, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    lv_obj_t * tab1 = lv_tabview_add_tab(tabview, "Tab 1");
    lv_obj_t * tab2 = lv_tabview_add_tab(tabview, "Tab 2");
    lv_obj_t * tab3 = lv_tabview_add_tab(tabview, "Tab 3");

    lv_obj_set_style_bg_color(tab2, lv_palette_lighten(LV_PALETTE_AMBER, 3), 0);
    lv_obj_set_style_bg_opa(tab2, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    lv_obj_t * label = lv_label_create(tab1);
    lv_label_set_text(label, "First tab");

    label = lv_label_create(tab2);
    lv_label_set_text(label, "Second tab");

    label = lv_label_create(tab3);
    lv_label_set_text(label, "Third tab");

    //style tabbar
    static lv_style_t style_tbar_btn;
    lv_style_init(&style_tbar_btn);
    lv_style_set_text_font(&style_tbar_btn, &googleMat40);
    lv_obj_t * tabbar = lv_tabview_get_tab_bar(tabview);
    const char s[] = "\ue9e4\ue26b\ue8b8";
    for(uint8_t i = 0; i < lv_obj_get_child_count(tabbar); i++) {
      lv_obj_t * child = lv_obj_get_child(tabbar, i);
      //delete old label
      lv_obj_delete(lv_obj_get_child(child, 0));


      lv_obj_t * label = lv_label_create(child);
      lv_obj_add_style(label, &style_tbar_btn, 0);
      char t[6]={0};
      memcpy(t, s+3*i,3);
      lv_label_set_text(label, t);
      lv_obj_center(label);
      /* Do something with child. */
    }


    lv_obj_remove_flag(lv_tabview_get_content(tabview), LV_OBJ_FLAG_SCROLLABLE);
}

void showMainScreen(){
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
    
   drawTabView();
   drawTacho(-100,0,360);
   drawTachoV2(-100,0,360);
   drawPowerMeter(-100,0,360);
   // drawTempBars(0,0);
   // lv_example_canvas_5();

}

