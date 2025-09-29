#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations



// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_DISPLAY_TEMP = 0,
    FLOW_GLOBAL_VARIABLE_DISPLAY_HUMI = 1,
    FLOW_GLOBAL_VARIABLE_DISPLAY_CO2 = 2
};

// Native global variables

extern const char *get_var_display_temp();
extern void set_var_display_temp(const char *value);
extern const char *get_var_display_humi();
extern void set_var_display_humi(const char *value);
extern const char *get_var_display_co2();
extern void set_var_display_co2(const char *value);


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/