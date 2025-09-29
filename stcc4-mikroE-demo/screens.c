#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 170);
    {
        lv_obj_t *parent_obj = obj;
        {
            // temp_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.temp_value = obj;
            lv_obj_set_pos(obj, 180, 63);
            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            // humi_value__1
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.humi_value__1 = obj;
            lv_obj_set_pos(obj, 180, 107);
            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 279, 85);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "°C");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 279, 129);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "%");
        }
        {
            // co2_chart
            lv_obj_t *obj = lv_chart_create(parent_obj);
            objects.co2_chart = obj;
            lv_obj_set_pos(obj, 13, 16);
            lv_obj_set_size(obj, 167, 137);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_line_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_line_width(obj, 2, LV_PART_ITEMS | LV_STATE_DEFAULT);
        }
        {
            // co2_value
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.co2_value = obj;
            lv_obj_set_pos(obj, 180, 19);
            lv_obj_set_size(obj, 98, LV_SIZE_CONTENT);
            lv_label_set_long_mode(obj, LV_LABEL_LONG_CLIP);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_40, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafafa), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 279, 41);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "ppm");
        }
        {
            // co2_icon
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.co2_icon = obj;
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_co2);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
    {
        const char *new_val = get_var_display_temp();
        const char *cur_val = lv_label_get_text(objects.temp_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.temp_value;
            lv_label_set_text(objects.temp_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_display_humi();
        const char *cur_val = lv_label_get_text(objects.humi_value__1);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.humi_value__1;
            lv_label_set_text(objects.humi_value__1, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_display_co2();
        const char *cur_val = lv_label_get_text(objects.co2_value);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.co2_value;
            lv_label_set_text(objects.co2_value, new_val);
            tick_value_change_obj = NULL;
        }
    }
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
