// Copyright 2022 beekeeb
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

#include "quantum.h"
#include "qp.h"
#include "qp_comms.h"
#include "qp_st7735.h"
#include "qp_st77xx_opcodes.h"
#include "color.h"
#include "qp_lvgl.h"

enum keymap_layers {
    LAYER_BASE = 0,
    LAYER_LOWER,
    LAYER_RAISE,
    LAYER_POINTER,
};

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
  KC_Q,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         QK_BOOT,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  KC_Q,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         BL_TOGG,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    PT_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, PT_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, KC_Q,  KC_SPC,     KC_ENT, KC_Q, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),
      /*
    * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
    * │Tab│ Q │ W │ E │ R │ T │       │ Y │ U │ I │ O │ P │Bsp│
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Ctl│ A │ S │ D │ F │ G │       │ H │ J │ K │ L │ ; │ ' │
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Sft│ Z │ X │ C │ V │ B │       │ N │ M │ , │ . │ / │Esc│
    * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
    *               ┌───┐                   ┌───┐
    *               │GUI├───┐           ┌───┤Alt│
    *               └───┤LWR├───┐   ┌───┤UPR├───┘
    *                   └───┤Spc│   │Ent├───┘
    *                       └───┘   └───┘
    */
  [LAYER_LOWER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, XXXXXXX,                      KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),
    /*
    * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
    * │Tab│ 1 │ 2 │ 3 │ 4 │ 5 │       │ 6 │ 7 │ 8 │ 9 │ 0 │Bsp│
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Ctl│TOG│H+ │S+ │V+ │   │       │ ← │ ↓ │ ↑ │ → │   │   │
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Sft│MOD│H- │S- │V- │   │       │   │   │   │   │   │   │
    * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
    *               ┌───┐                   ┌───┐
    *               │GUI├───┐           ┌───┤Alt│
    *               └───┤   ├───┐   ┌───┤   ├───┘
    *                   └───┤Spc│   │Ent├───┘
    *                       └───┘   └───┘
    */
  [LAYER_RAISE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),
    /*
    * ┌───┬───┬───┬───┬───┬───┐       ┌───┬───┬───┬───┬───┬───┐
    * │Tab│ ! │ @ │ # │ $ │ % │       │ ^ │ & │ * │ ( │ ) │Bsp│
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Ctl│   │   │   │   │   │       │ - │ = │ [ │ ] │ \ │ ` │
    * ├───┼───┼───┼───┼───┼───┤       ├───┼───┼───┼───┼───┼───┤
    * │Sft│   │   │   │   │   │       │ _ │ + │ { │ } │ | │ ~ │
    * └───┴───┴───┴───┴───┴───┘       └───┴───┴───┴───┴───┴───┘
    *               ┌───┐                   ┌───┐
    *               │GUI├───┐           ┌───┤Alt│
    *               └───┤   ├───┐   ┌───┤   ├───┘
    *                   └───┤Spc│   │Ent├───┘
    *                       └───┘   └───┘
    */
  [LAYER_POINTER] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT,  EE_CLR, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      XXXXXXX, XXXXXXX,SBUF_MOD, XXXXXXX, DPI_MOD, S_D_MOD,                     S_D_RMOD,DPI_RMOD, XXXXXXX, XXXXXXX,SBUF_RMOD,XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, DRGSCRL, SNIPING, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, SNIPING, DRGSCRL, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),
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

// void housekeeping_task_user(void) {
//     static uint32_t timer = 0;
//     static HSV color = { .h = 0, .s = 255, .v = 255 };

//     if (timer_elapsed32(timer) < 40)
//         return;

//     timer = timer_read32();

//     // increase hue -> change color
//     color.h++;
//     qp_circle(display, LCD_WIDTH/2, LCD_HEIGHT/2, 10, color.h, color.s, color.v, true);
//     qp_flush(display);
// }
