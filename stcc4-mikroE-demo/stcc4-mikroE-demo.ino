#define LV_CONF_INCLUDE_SIMPLE
#include "lv_conf.h"
#include <string>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <ui.h>
#include "vars.h"
#include <Arduino.h>
#include <SensirionI2cStcc4.h>
#include <Wire.h>

#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

#define SDA_PIN 8
#define SCL_PIN 7


TFT_eSPI tft = TFT_eSPI();
SensirionI2cStcc4 sensor;

static char errorMessage[16];
static int16_t error;

float last_temp = 20.0;
float last_humi = 50.0;
int16_t last_co2 = 500;
uint16_t sensorStatus = 0;

lv_chart_series_t * co2_serie;

char display_co2[5];
char display_humi[5];
char display_temp[5];

extern "C" const char *get_var_display_humi() {
    snprintf ( display_humi, 5, "%.1f", last_humi );
    return display_humi;
}


extern "C" const char *get_var_display_temp() {
    snprintf ( display_temp, 5, "%.1f", last_temp );
    return display_temp;
}

extern "C" const char *get_var_display_co2() {
    snprintf ( display_co2, 5, "%i", last_co2 );
    return display_co2;
}


/* Display rendering callback */
static void lv_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h);
    lv_disp_flush_ready(disp);
}

void read_measured_data(lv_timer_t * timer)
{
    error = sensor.readMeasurement(last_co2, last_temp, last_humi, sensorStatus);
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute readMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    lv_chart_set_next_value(objects.co2_chart,co2_serie,last_co2);
}


void setup() {
    Serial.begin(115200);
    Wire.begin();
    sensor.begin(Wire, 0x65);
    sensor.stopContinuousMeasurement();

    tft.begin();
    tft.setRotation(1);

    lv_init();

    /* Initialize the display buffer */
    static lv_disp_draw_buf_t draw_buf;
    static lv_color_t *lv_disp_buf;
    lv_disp_buf = (lv_color_t *)heap_caps_malloc(TFT_WIDTH*TFT_HEIGHT * sizeof(lv_color_t), MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
    lv_disp_draw_buf_init(&draw_buf, lv_disp_buf, NULL, TFT_HEIGHT * TFT_WIDTH / 10);

    /*Initialize the display*/
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = TFT_HEIGHT;
    disp_drv.ver_res = TFT_WIDTH;
    disp_drv.flush_cb = lv_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    ui_init();  // Init EEZ-Studio UI

    lv_chart_set_point_count(objects.co2_chart, 30);
    lv_chart_set_range(objects.co2_chart,LV_CHART_AXIS_PRIMARY_Y, 0,3000);
    co2_serie = lv_chart_add_series(objects.co2_chart, lv_color_hex(0x5c5c5c), LV_CHART_AXIS_PRIMARY_Y);

    error = sensor.startContinuousMeasurement();
    if (error != NO_ERROR) {
        Serial.print("Error trying to execute startContinuousMeasurement(): ");
        errorToString(error, errorMessage, sizeof errorMessage);
        Serial.println(errorMessage);
        return;
    }
    delay(1000);
    lv_timer_t * timer = lv_timer_create(read_measured_data, 1000, nullptr);
}

void loop() {
  sleep(0.2);
  lv_timer_handler();
  ui_tick();
}
