#include <stdio.h>
#include <string.h>

#include "sound.h"
#include "lvgl.h"
#include "btmui.h"
#include "prefs.h"
#include "../.pio/libdeps/esp32-s3/lvgl/examples/lv_examples.h"



LV_FONT_DECLARE(brandon_BI_40);
LV_FONT_DECLARE(brandon_BI_50);
LV_FONT_DECLARE(brandon_BI_55);
LV_FONT_DECLARE(brandon_BI_150);
LV_FONT_DECLARE(googleMat40);
//LV_FONT_DECLARE(lv_font_montserrat_10);


LV_IMAGE_DECLARE(warn_charge);
LV_IMAGE_DECLARE(warn_engine);
LV_IMAGE_DECLARE(warn_light);
LV_IMAGE_DECLARE(warn_temp);

lv_theme_t * th;

static lv_style_t t_speed_style;
static lv_style_t t_unit_style;
static lv_style_t pm_bg_style;
static lv_style_t tileGridStyle;
//static lv_style_t pm_ind_style;

lv_obj_t *arc, *pm_arc;
lv_obj_t * t_unit ;
lv_obj_t * t_speed;
lv_obj_t * bar_temp_motor;
lv_obj_t * tachoTab;
lv_obj_t * tachoTileView;
lv_obj_t * statsTab;
lv_obj_t * settingsTab;
lv_obj_t * tabview;

//warning lights
lv_obj_t* warnEngine;
lv_obj_t* warnLight;
lv_obj_t* warnBattery;
lv_obj_t* warnTemp;

void setSpeedKmh(uint8_t val){

  if(getPrefs()->getBool("retardUnits")){
    //do conversion km/h->mph
    val=val
  }

      lv_arc_set_value(arc, val);
      char str[5];
      sprintf(str, "%d", val);
      lv_label_set_text(t_speed,str);
}

void setLight(bool on){
  lv_obj_set_style_img_recolor(warnLight, lv_color_hex(on?0x0000ff:0x222222),LV_PART_MAIN);
}


void setPower(int16_t val){
  
  if(val<0){
      lv_obj_set_style_arc_color(pm_arc,lv_color_hex(0x00ff00),LV_PART_INDICATOR);
  }else{
      lv_obj_set_style_arc_color(pm_arc,lv_color_hex(0xff0000),LV_PART_INDICATOR);
  }
  lv_arc_set_value(pm_arc, val);
  //lv_obj_add_style(pm_arc, &pm_ind_style, LV_PART_INDICATOR);
    //lv_label_set_text(pm_label,"x");
   // lv_arc_align_obj_to_angle(pm_arc, pm_label, 25);
}

void  drawGearDisp(int x=0, int y=0, int rad=360){

    //gear display
    lv_obj_t * gearDisp = lv_label_create(tachoTab);
    lv_obj_set_style_text_font(gearDisp, &brandon_BI_55,0);
    lv_label_set_text(gearDisp, "2");
    lv_obj_set_style_text_align(gearDisp,LV_TEXT_ALIGN_RIGHT,LV_PART_MAIN);
    lv_obj_align(gearDisp, LV_ALIGN_BOTTOM_LEFT, x+209, y-78);
    lv_obj_set_style_bg_opa(gearDisp, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(gearDisp,lv_color_hex(0xffffff),LV_PART_MAIN);//lv_theme_get_color_primary(gearDisp),LV_PART_MAIN);
    lv_obj_set_style_text_color(gearDisp,lv_color_hex(0x000000),LV_PART_MAIN);
    lv_obj_set_style_size(gearDisp,69, 55, LV_PART_MAIN);
    lv_obj_set_style_pad_all(gearDisp, 4,LV_PART_MAIN); 
    lv_obj_set_style_pad_right(gearDisp, 7,LV_PART_MAIN); 
}

void drawPowerMeter(int x=0, int y=0, int rad=360){
     /*Create an Arc*/
    pm_arc = lv_arc_create(tachoTab);
    lv_obj_set_size(pm_arc, rad-78, rad-78);
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
    
    lv_obj_set_style_pad_bottom(pm_arc, 0, LV_PART_INDICATOR);
    lv_obj_set_style_pad_top(pm_arc, 0, LV_PART_INDICATOR);
    lv_obj_set_style_arc_color(pm_arc, lv_color_hex(0xff0000), LV_PART_INDICATOR);
    lv_obj_set_style_arc_width(pm_arc, 20, LV_PART_INDICATOR);
    lv_obj_set_style_arc_rounded(pm_arc, false, LV_PART_INDICATOR);
    
    //lv_obj_add_style(pm_arc, &pm_ind_style, LV_PART_INDICATOR);

    //readonly
    lv_obj_remove_flag(pm_arc, LV_OBJ_FLAG_CLICKABLE);

    //text in tacho - speed & unit



}

void drawTacho(int x=0, int y=0, int rad=360){
     /*Create an Arc*/
    arc = lv_arc_create(tachoTab);
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
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);

    //indicator
    static lv_style_t ind_style;
    lv_style_set_pad_bottom(&ind_style,0);
    lv_style_set_pad_top(&ind_style,0);
    lv_style_set_arc_color(&ind_style,lv_theme_get_color_primary(arc));
    lv_style_set_arc_width(&ind_style,40);
    lv_style_set_arc_rounded(&ind_style,false);
    lv_obj_add_style(arc, &ind_style, LV_PART_INDICATOR);

    //readonly
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    //text in tacho - speed & unit
    t_speed = lv_label_create(tachoTab);
    
    lv_obj_align(t_speed, LV_ALIGN_CENTER, 0+x, y-10);
    
    lv_style_init(&t_speed_style);
    lv_style_set_text_font(&t_speed_style, &brandon_BI_150);
    lv_style_set_text_color(&t_speed_style,lv_color_hex(0xffffff));
    lv_obj_add_style(t_speed, &t_speed_style, 0);
    lv_label_set_text(t_speed,"187");

    t_unit = lv_label_create(tachoTab);
    
    lv_obj_align(t_unit, LV_ALIGN_CENTER, 0+x, y+62);
    lv_style_init(&t_unit_style);
    lv_style_set_text_font(&t_unit_style, &brandon_BI_40);
    lv_style_set_text_color(&t_unit_style,lv_color_hex(0x999999));
    lv_obj_add_style(t_unit, &t_unit_style, 0);
    lv_label_set_text(t_unit,"km/h");


}


void drawTachoV2(int x=0, int y=0, int rad=360){
  lv_obj_t * scale = lv_scale_create(tachoTab);
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

    /* Main line properties */
   // lv_style_set_arc_color(&main_line_style, lv_palette_darken(LV_PALETTE_BLUE, 3));
    lv_obj_set_style_arc_width(scale, 0, LV_PART_MAIN); /*Tick width*/
    //lv_obj_add_style(scale, &main_line_style, );



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

// void drawTempBars(int x=0,int y=0){
//   static lv_style_t style_indic;
//   static lv_style_t style_bg;
//     lv_style_init(&style_indic);
//     lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
//     lv_style_set_bg_color(&style_indic, lv_color_hex(0xffffff));
//     lv_style_set_radius(&style_indic,0);
    
//     lv_style_init(&style_bg);
//     lv_style_set_border_side(&style_bg, LV_BORDER_SIDE_TOP);
//     lv_style_set_border_color(&style_bg, lv_color_hex(0xff0000));
//     lv_style_set_border_width(&style_bg, 2);
//     lv_style_set_pad_all(&style_bg, 0); /*To make the indicator smaller*/
//     lv_style_set_radius(&style_bg, 0);
    
    


//     bar_temp_motor = lv_bar_create(lv_screen_active());

    

//     lv_obj_remove_style_all(bar_temp_motor);  /*To have a clean start*/
//     lv_obj_add_style(bar_temp_motor, &style_bg, 0);
//     lv_obj_add_style(bar_temp_motor, &style_indic, LV_PART_INDICATOR);
  
//   //lv_obj_center(bar_temp_motor)
//     lv_bar_set_orientation(bar_temp_motor, LV_BAR_ORIENTATION_VERTICAL);
//     lv_obj_align(bar_temp_motor, LV_ALIGN_BOTTOM_LEFT, 10+x, y-30);
//     lv_bar_set_range(bar_temp_motor, 0, 100);
//     lv_obj_set_size(bar_temp_motor, 20, 200);
//     lv_bar_set_value(bar_temp_motor, 21, LV_ANIM_OFF);

// }

void drawTabView(void){
    /*Create a Tab view object*/
    
    tabview = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_position(tabview, LV_DIR_LEFT);
    lv_tabview_set_tab_bar_size(tabview, 70);

    lv_obj_set_style_bg_color(tabview, lv_color_hex(0x000000), 0);

    lv_obj_t * tab_buttons = lv_tabview_get_tab_bar(tabview);
    lv_obj_set_style_bg_color(tab_buttons, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_color(tab_buttons, lv_color_hex(0x222222), 0);
    lv_obj_set_style_text_color(tab_buttons, lv_color_hex(0xffffff), LV_STATE_CHECKED);
    lv_obj_set_style_border_side(tab_buttons, LV_BORDER_SIDE_RIGHT, LV_PART_ITEMS | LV_STATE_CHECKED);

    /*Add 3 tabs (the tabs are page (lv_page) and can be scrolled*/
    tachoTab = lv_tabview_add_tab(tabview, "TachoTab");
    statsTab = lv_tabview_add_tab(tabview, "StatsTab");
    settingsTab = lv_tabview_add_tab(tabview, "SettingsTab");
   // lv_obj_set_style_bg_color(settingsTab, lv_palette_lighten(LV_PALETTE_AMBER, 3), 0);
    //v_obj_set_style_bg_opa(settingsTab, LV_OPA_COVER, 0);

    /*Add content to the tabs*/
    lv_obj_t * label;// = lv_label_create(tachoTab);
   // lv_label_set_text(label, "First tab");

    //label = lv_label_create(settingsTab);
   //lv_label_set_text(label, "Second tab");

    label = lv_label_create(statsTab);
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

// lv_obj_t * drawInfoTile(lv_obj_t * parent, const char* name, uint8_t row, uint8_t col, uint16_t xsize=190,uint16_t ysize=100){
//     static int32_t col_dsc[] = {xsize, LV_GRID_TEMPLATE_LAST};
//     static int32_t row_dsc[] = {16, ysize-16, LV_GRID_TEMPLATE_LAST};
//     //static lv_style_t tileStyle;




//     lv_obj_t * cont = lv_obj_create(parent);
//     lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), 0);
//     lv_obj_t * label = lv_label_create(cont);
//     lv_obj_set_style_pad_all(label,5,0);
    
//     lv_label_set_text(label, name);

//     lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
//     lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
//     lv_obj_set_style_pad_all(cont,0,0);
//     lv_obj_set_style_border_width(cont,2,0);
//     lv_obj_set_style_border_color(cont,lv_color_hex(0xffffff),0);
//     //lv_obj_set_style_pad_hor(cont,0,0);

//     lv_obj_set_style_radius(cont,0,0);
//     //lv_obj_add_style(cont, &tileStyle, 0);

//     //lv_obj_set_size(cont, 200, 120);
//     //lv_obj_align(cont, LV_ALIGN_CENTER, 0, 0);
//     //lv_obj_center(cont);
//     lv_obj_set_layout(cont, LV_LAYOUT_GRID);
//     // lv_obj_set_grid_align(cont, LV_GRID_ALIGN_SPACE_EVENLY, LV_GRID_ALIGN_SPACE_EVENLY);
//     lv_obj_set_grid_cell(label, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, 0, 1);
    

//     lv_obj_set_grid_cell(cont, LV_GRID_ALIGN_STRETCH, col, 1, LV_GRID_ALIGN_STRETCH, row, 1);

//     return cont;
// }

struct infotile{
  lv_obj_t * cont;
  lv_obj_t * tile;
  lv_obj_t * value;
};

void drawInfoTile2(lv_obj_t * parent,infotile* it, const char* name, void (*valfunc)(infotile*,char*),char* val, uint16_t xsize=lv_pct(100),uint16_t ysize=100){
  lv_obj_t * cont = lv_obj_create(parent);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), 0);
  lv_obj_t * label = lv_label_create(cont);
  lv_obj_set_size(label,lv_pct(100), 15);
  lv_obj_set_style_pad_all(label,0,0);
  
  lv_label_set_text(label, name);

  lv_obj_set_style_pad_all(cont,5,0);
  lv_obj_set_style_border_width(cont,2,0);
  lv_obj_set_style_border_color(cont,lv_color_hex(0xffffff),0);

  lv_obj_set_style_radius(cont,0,0);
  lv_obj_set_size(cont, xsize, ysize);

  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

  it->cont=cont; 
  valfunc(it,val);  
}

void drawSettingsTile2(lv_obj_t * parent,infotile* it, const char* name, void (*valfunc)(infotile*,char*),char* val, uint16_t xsize=lv_pct(100),uint16_t ysize=100){
  lv_obj_t * cont = lv_obj_create(parent);
  lv_obj_set_style_bg_color(cont, lv_color_hex(0x000000), 0);
  lv_obj_t * label = lv_label_create(cont);
  //lv_obj_set_size(label,lv_pct(100), 15);
  lv_obj_set_style_pad_all(label,0,0);
  
  lv_label_set_text(label, name);

  lv_obj_set_style_pad_all(cont,5,0);
  lv_obj_set_style_border_width(cont,2,0);
  lv_obj_set_style_border_color(cont,lv_color_hex(0xffffff),0);

  lv_obj_set_style_radius(cont,0,0);
  lv_obj_set_size(cont, xsize, ysize);

  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN,LV_FLEX_ALIGN_CENTER,LV_FLEX_ALIGN_CENTER);
  it->cont=cont; 
  valfunc(it,val);  
}

void simpleTileValueSwitch(infotile* it, char* v){
  it->value = lv_switch_create(it->cont);
}

void simpleTileValueLabel(infotile* it, char* v){
  it->value = lv_label_create(it->cont);
  lv_label_set_text(it->value , v); 
    lv_obj_set_style_text_font(it->value , &brandon_BI_50,0);
  //  lv_obj_set_grid_cell(it->value , LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);

}


void drawInfoGrid(int width=210){
    lv_obj_t * cont = lv_obj_create(tachoTab);

    lv_obj_add_style(cont, &tileGridStyle, 0);

    lv_obj_set_scroll_snap_y(cont, LV_SCROLL_SNAP_START);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);

    lv_obj_set_size(cont, width, 400);
    lv_obj_align(cont , LV_ALIGN_TOP_RIGHT, 0, 40);

    infotile akku,odo,odo2,odo3,range;

    drawInfoTile2(cont,&range, "Restreichweite", simpleTileValueLabel, "42km", width-0);
    drawInfoTile2(cont,&akku, "Akkustand", simpleTileValueLabel, "68p",width-0);
    drawInfoTile2(cont,&odo, "Gesamtkilometer", simpleTileValueLabel, "137km", width-0);
    drawInfoTile2(cont,&odo2, "Gesamtkilometer", simpleTileValueLabel, "138km", width-0);
    drawInfoTile2(cont,&odo3, "Gesamtkilometer", simpleTileValueLabel, "139km", width-0);


    //prevent overscrolling at the bottom
    lv_obj_remove_flag(odo2.cont, LV_OBJ_FLAG_SNAPPABLE);
    lv_obj_remove_flag(odo3.cont, LV_OBJ_FLAG_SNAPPABLE);
  



}

// lv_obj_t * initWarnLight(  lv_obj_t * parent, const char* symbol,int32_t posx, int32_t posy){
//   lv_obj_t * lbl= lv_label_create(parent);
//   lv_label_set_text(lbl, symbol);

//   static lv_style_t warnStyle;
//   lv_style_init(&warnStyle);
//   lv_style_set_bg_opa(&warnStyle, LV_OPA_0);
//   lv_style_set_pad_all(&warnStyle, 2);
//   lv_style_set_text_font(&warnStyle, &lv_font_montserrat_40);
//   lv_style_set_text_color(&warnStyle, lv_color_hex(0x222222));
//   lv_obj_add_style(lbl, &warnStyle, LV_PART_MAIN);
  

//   lv_obj_set_grid_cell(lbl, LV_GRID_ALIGN_STRETCH, posx, 1, LV_GRID_ALIGN_STRETCH, posy, 1);
//   return lbl;
// }




lv_obj_t * initWarnLight2(  lv_obj_t * parent, const char symbol){
  //lv_obj_t * lbl= lv_label_create(parent);
  //lv_label_set_text(lbl, symbol);

  //static lv_style_t warnStyle;
  //lv_style_init(&warnStyle);
  //lv_style_set_bg_opa(&warnStyle, LV_OPA_0);
  //lv_style_set_pad_all(&warnStyle, 2);
  //lv_style_set_text_font(&warnStyle, &lv_font_montserrat_40);
  //lv_style_set_text_color(&warnStyle, lv_color_hex(0x222222));
  //lv_obj_add_style(lbl, &warnStyle, LV_PART_MAIN);
  
  lv_obj_t * img1 = lv_image_create(parent);
  lv_obj_set_style_img_recolor(img1, lv_color_hex(0x222222),0);
  lv_obj_set_style_img_recolor_opa(img1,LV_OPA_COVER ,0);
  switch(symbol){
    case 'b': lv_image_set_src(img1, &warn_charge);break;
    case 'e': lv_image_set_src(img1, &warn_engine);break;
    case 'l': lv_image_set_src(img1, &warn_light);break;
    case 't': lv_image_set_src(img1, &warn_temp);break;

  }
  
  return img1;
}

void drawWarningLights2(){

  /*Create a container with grid*/
  lv_obj_t * cont = lv_obj_create(tachoTab);

  lv_obj_set_style_bg_opa(cont, LV_OPA_0, LV_PART_MAIN);
  lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
  lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);

  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(cont, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_size(cont, 220, 60);
  lv_obj_align(cont , LV_ALIGN_BOTTOM_LEFT, 195, -75);

  warnBattery = initWarnLight2(cont,'b');
  warnLight= initWarnLight2(cont,'l');
  warnTemp = initWarnLight2(cont,'t');
  warnEngine = initWarnLight2(cont,'e');

}






// void drawWarningLights(){
//   static int32_t col_dsc[] = {50,50,50,50, LV_GRID_TEMPLATE_LAST};
//   static int32_t row_dsc[] = {50, LV_GRID_TEMPLATE_LAST};



//      /*Create a container with grid*/
//     lv_obj_t * cont = lv_obj_create(tachoTab);

//     lv_obj_set_style_bg_opa(cont, LV_OPA_0, LV_PART_MAIN);
//     lv_obj_set_style_border_width(cont, 0, LV_PART_MAIN);
//     lv_obj_set_style_pad_all(cont, 0, LV_PART_MAIN);
//     lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
//     lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
//     lv_obj_set_size(cont, 240, 70);
//     //lv_obj_center(cont);
//     lv_obj_set_layout(cont, LV_LAYOUT_GRID);
//     lv_obj_align(cont , LV_ALIGN_BOTTOM_LEFT, 185, -60);



//     warnEngine = initWarnLight(cont,LV_SYMBOL_WARNING,3,0);
//     warnLight= initWarnLight2(cont,'l');
//     warnBattery = initWarnLight2(cont,'b');
//     warnTemp = initWarnLight(cont,LV_SYMBOL_UP,2,0);

// }

void drawBTMLogo(){
  LV_IMAGE_DECLARE(Black_50);
  lv_obj_t * img1 = lv_image_create(tachoTab);
  lv_obj_set_style_img_recolor(img1, lv_color_hex(0xffffff),0);
  lv_obj_set_style_img_recolor_opa(img1,LV_OPA_COVER ,0);
  lv_image_set_src(img1, &Black_50);
  lv_obj_align(img1 , LV_ALIGN_BOTTOM_LEFT,200,-10);
}

void createTachoTab(){
   //fahrstufe???, blinker
   drawPowerMeter(-110,0,400);
   drawGearDisp(-110,0,400);
   drawTacho(-110,0,400);
   drawTachoV2(-110,0,400);
   //kontrolleuchten: FernLicht, Temperaturwarnung heiß/kalt, Akku/Reserve/wird geladen, Motor/Technikproblem
   //drawTachoTiles();
   
   
   drawInfoGrid();
   drawWarningLights2();
   drawBTMLogo();
}

static void sw_avas_evh(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_current_target_obj(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)){
          getPrefs()->putBool("avasEnabled", true);
          soundEnable();
        }else{
          getPrefs()->putBool("avasEnabled", false);
          soundDisable();
        }
    }
}


void createSettingsTab(){
  lv_obj_t *  heading = lv_label_create(settingsTab);
  lv_obj_align(heading , LV_ALIGN_TOP_LEFT, 10, 10);
  lv_label_set_text(heading, "Einstellungen");
  lv_obj_set_style_text_font(heading, &lv_font_montserrat_18,0);

   /*Create a container with grid*/
  lv_obj_t * cont = lv_obj_create(settingsTab);

  lv_obj_set_size(cont, lv_pct(100), 360);
  lv_obj_set_flex_flow(cont, LV_FLEX_FLOW_COLUMN);
  lv_obj_align(cont , LV_ALIGN_TOP_LEFT, 0, 50);

  lv_obj_add_style(cont, &tileGridStyle, 0);

  infotile avason,themesel,retardunits;

  //AVAS ON/OFF
  drawSettingsTile2(cont,&avason, "AVAS an", simpleTileValueSwitch, "", 600);
  lv_obj_add_event_cb(avason.value, sw_avas_evh, LV_EVENT_ALL, NULL);

  if(getPrefs()->getBool("avasEnabled")){
    lv_obj_add_state(avason.value, LV_STATE_CHECKED);
    soundEnable();
  }else{
    lv_obj_remove_state(avason.value, LV_STATE_CHECKED);
    soundDisable();
  }
  
  
  drawSettingsTile2(cont,&retardunits, "Imperiale Einheiten", simpleTileValueSwitch, "", 600);



}

void drawClock(){
  lv_obj_t *  clk = lv_label_create(lv_screen_active());
  lv_obj_align(clk , LV_ALIGN_TOP_RIGHT, -20, 10);
  lv_obj_set_style_text_font(clk, &brandon_BI_40,0);
  lv_label_set_text(clk,"13.37");

}

void initTileGridStyle(){
  lv_style_init(&tileGridStyle);
 // lv_style_set_pad_column(&tileGridStyle,1);
  lv_style_set_border_width(&tileGridStyle,0);
//  lv_style_set_pad_row(&tileGridStyle,1);
  lv_style_set_pad_all(&tileGridStyle,0);
  lv_style_set_radius(&tileGridStyle,0);
  lv_style_set_bg_color(&tileGridStyle, lv_color_hex(0x000000));

}

void showMainScreen(lv_display_t * display){
 th = lv_theme_default_init(display,                 /* Use DPI, size, etc. from this display */
                                        lv_color_hex(0xffffff),   /* Primary and secondary palette */
                                        lv_color_hex(0xdddddd),
                                        true,                   /* Dark theme?  False = light theme. */
                                        &lv_font_montserrat_14);
  lv_display_set_theme(display, th); /* Assign theme to display */


  lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);


  initTileGridStyle();



  drawTabView();
  createTachoTab();
  createSettingsTab();
  drawClock();

   // drawTempBars(0,0);
   // lv_example_canvas_5();

}

