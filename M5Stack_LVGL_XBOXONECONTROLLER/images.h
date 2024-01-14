#ifndef EEZ_LVGL_UI_IMAGES_H
#define EEZ_LVGL_UI_IMAGES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <lvgl.h>

extern const lv_img_dsc_t img_button_x;
extern const lv_img_dsc_t img_button_y;
extern const lv_img_dsc_t img_button_a;
extern const lv_img_dsc_t img_button_b;
extern const lv_img_dsc_t img_analog_l;
extern const lv_img_dsc_t img_analog_r;
extern const lv_img_dsc_t img_analog_trigger_l;
extern const lv_img_dsc_t img_analog_trigger_r;
extern const lv_img_dsc_t img_bumper_l;
extern const lv_img_dsc_t img_bumper_r;
extern const lv_img_dsc_t img_dpad_up;
extern const lv_img_dsc_t img_dpad_r;
extern const lv_img_dsc_t img_dpad_down;
extern const lv_img_dsc_t img_dpad_l;
extern const lv_img_dsc_t img_connected;
extern const lv_img_dsc_t img_disconnected;
extern const lv_img_dsc_t img_button_view;
extern const lv_img_dsc_t img_button_menu;
extern const lv_img_dsc_t img_button_start;

#ifndef EXT_IMG_DESC_T
#define EXT_IMG_DESC_T
typedef struct _ext_img_desc_t {
    const char *name;
    const lv_img_dsc_t *img_dsc;
} ext_img_desc_t;
#endif

extern const ext_img_desc_t images[19];


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_IMAGES_H*/