#include "lvgl.h"
#include "btmui.h"
#include "SerialDebug.h"
#include "../.pio/libdeps/esp32-s3/lvgl/examples/lv_examples.h"



LV_FONT_DECLARE(brandon_BI_40);
LV_FONT_DECLARE(brandon_BI_50);
LV_FONT_DECLARE(brandon_BI_150);

static lv_style_t t_speed_style;
    static lv_style_t t_unit_style;
    lv_obj_t * t_unit ;
lv_obj_t * t_speed;

void drawTacho(){
     /*Create an Arc*/
    lv_obj_t * arc = lv_arc_create(lv_screen_active());
    lv_obj_set_size(arc, 360, 360);
    lv_arc_set_rotation(arc, 135);
    lv_arc_set_range(arc, 0, 130);
    lv_arc_set_bg_angles(arc, 0, 270);
    lv_arc_set_value(arc, 10);
    lv_obj_center(arc);
    //lv_obj_add_event_cb(arc, value_changed_event_cb, LV_EVENT_VALUE_CHANGED, label);

    /*Manually update the label for the first time*/
    //lv_obj_send_event(arc, LV_EVENT_VALUE_CHANGED, NULL);
    
    //background
    static lv_style_t bg_style;
    lv_style_set_arc_color(&bg_style,lv_color_hex(0x999999));
    lv_style_set_arc_width(&bg_style,40);
    lv_style_set_arc_rounded(&bg_style,false);
    lv_obj_add_style(arc, &bg_style, LV_PART_MAIN);

    //remove knob
    static lv_style_t my_style;
    lv_style_set_bg_opa (&my_style,0);
    lv_obj_add_style(arc, &my_style, LV_PART_KNOB);

    //indicator
    static lv_style_t ind_style;
    lv_style_set_pad_bottom(&ind_style,0);
    lv_style_set_pad_top(&ind_style,0);
    lv_style_set_arc_color(&ind_style,lv_color_hex(0xff0000));
    lv_style_set_arc_width(&ind_style,40);
    lv_style_set_arc_rounded(&ind_style,false);
    lv_obj_add_style(arc, &ind_style, LV_PART_INDICATOR);

    //readonly
    lv_obj_remove_flag(arc, LV_OBJ_FLAG_CLICKABLE);

    //text in tacho - speed & unit
    t_speed = lv_label_create(lv_screen_active());
    
    lv_obj_align(t_speed, LV_ALIGN_CENTER, 0, 0);
    
    lv_style_init(&t_speed_style);
    lv_style_set_text_font(&t_speed_style, &brandon_BI_150);
    lv_style_set_text_color(&t_speed_style,lv_color_hex(0xffffff));
    lv_obj_add_style(t_speed, &t_speed_style, 0);
lv_label_set_text(t_speed,"187");

    t_unit = lv_label_create(lv_screen_active());
    
    lv_obj_align(t_unit, LV_ALIGN_CENTER, 0, 75);
    lv_style_init(&t_unit_style);
    lv_style_set_text_font(&t_unit_style, &brandon_BI_40);
    lv_style_set_text_color(&t_unit_style,lv_color_hex(0x999999));
    lv_obj_add_style(t_unit, &t_unit_style, 0);
lv_label_set_text(t_unit,"km/h");


}

#define CANVAS_WIDTH  50
#define CANVAS_HEIGHT  50
void lv_example_canvas_5(void)
{
    /*Create a buffer for the canvas*/
    LV_DRAW_BUF_DEFINE_STATIC(draw_buf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_COLOR_FORMAT_ARGB8888);
    LV_DRAW_BUF_INIT_STATIC(draw_buf);

    /*Create a canvas and initialize its palette*/
    lv_obj_t * canvas = lv_canvas_create(lv_screen_active());
    lv_canvas_set_draw_buf(canvas, &draw_buf);
    lv_canvas_fill_bg(canvas, lv_color_hex3(0xccc), LV_OPA_COVER);
    lv_obj_center(canvas);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    lv_vector_dsc_t * dsc = lv_vector_dsc_create(&layer);
    lv_vector_path_t * path = lv_vector_path_create(LV_VECTOR_PATH_QUALITY_MEDIUM);

    lv_fpoint_t pts[] = {{10, 10}, {130, 130}, {10, 130}};
    lv_vector_path_move_to(path, &pts[0]);
    lv_vector_path_line_to(path, &pts[1]);
    lv_vector_path_line_to(path, &pts[2]);
    lv_vector_path_close(path);

    lv_vector_dsc_set_fill_color(dsc, lv_color_make(0x00, 0x80, 0xff));
    lv_vector_dsc_add_path(dsc, path);

    lv_draw_vector(dsc);
    lv_vector_path_delete(path);
    lv_vector_dsc_delete(dsc);

    lv_canvas_finish_layer(canvas, &layer);

}


void showMainScreen(){
    lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(0x000000), LV_PART_MAIN);
    
    drawTacho();
lv_example_canvas_5();

}

