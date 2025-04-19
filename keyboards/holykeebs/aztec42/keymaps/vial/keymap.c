// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H
#include "qp.h"
#include "qp_comms.h"
#include "qp_st7735.h"
#include "qp_st77xx_opcodes.h"
#include "color.h"
#include "qp_lvgl.h"

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
         KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P, KC_BSPC,
         KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
         KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
                                             KC_LGUI,    MO(1), KC_SPC,     KC_ENT,   MO(2),  KC_RALT
    ),
    [1] = LAYOUT_split_3x6_3(
         KC_TAB,    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, KC_BSPC,
         KC_LCTL,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_QUOT,
         KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX,  KC_F11, XXXXXXX,                      XXXXXXX,  KC_F12, XXXXXXX, XXXXXXX, XXXXXXX, KC_RSFT,
                                             KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
    ),

    [2] = LAYOUT_split_3x6_3(
          KC_TAB,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS, KC_BSPC,
          KC_LCTL, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN,  KC_GRV,
          KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
                                             KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
    ),

    [3] = LAYOUT_split_3x6_3(
          QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
          XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
                                             KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
    )
};

painter_device_t display;

void keyboard_post_init_kb(void) {
    wait_ms(LCD_WAIT_TIME);

    // Initialise the LCD
    display = qp_st7735_make_spi_device(LCD_HEIGHT, LCD_WIDTH, LCD_CS_PIN, LCD_DC_PIN, LCD_RST_PIN, LCD_SPI_DIVISOR, 0);
    if (!qp_init(display, QP_ROTATION_270) || !qp_power(display, true) || !qp_lvgl_attach(display)) return;

    #ifdef LCD_INVERT_COLOUR
    qp_comms_start(display);
    qp_comms_command(display, ST77XX_CMD_INVERT_ON);
    qp_comms_stop(display);
    #endif

    // qp_rect(display, 0, 0, LCD_WIDTH, LCD_HEIGHT, 0, 0, 0, true);
    // qp_flush(display);

    // qp_circle(display, LCD_WIDTH/2, LCD_HEIGHT/2, 10, 0, 0, 0, true);
    // qp_flush(display);
    lv_obj_t * slider = lv_slider_create(lv_scr_act());
    lv_obj_set_width(slider, 120);                          /*Set the width*/
    lv_obj_center(slider);                                  /*Align to the center of the parent (screen)*/

    /*Create a label below the slider*/
    lv_obj_t * label = lv_label_create(lv_scr_act());
    lv_label_set_text(label, "0");
    lv_obj_align_to(label, slider, LV_ALIGN_OUT_TOP_MID, 0, -15);

    keyboard_post_init_user();
}
