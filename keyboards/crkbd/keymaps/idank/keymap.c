/*
Copyright 2019 @foostan
Copyright 2020 Drashna Jaelre <@drashna>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdbool.h>
#include <stdint.h>
#include QMK_KEYBOARD_H
#include "keymap_us_international.h"
#include "keyboards/crkbd/keymaps/idank/secret.h"
#include "users/holykeebs/holykeebs.h"

#define QK_C_EEPROM QK_CLEAR_EEPROM

enum custom_keycodes {
    BSP_DEL = QK_USER_0,
    SWITCH,
    KC_PSTRING,
    KC_TABWIN
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
  KC_TAB,    KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN,  BSP_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      SC_LSPO,    KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,  KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      SC_RSPC,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H, KC_COMM,  KC_DOT, KC_SLSH,   RSFT_T(KC_ESC),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RALT,  TL_LOWR,  KC_SPC,     KC_ENT,   TL_UPPR, RCTL_T(KC_TABWIN)
                                      //`--------------------------'  `--------------------------'

  ),

  [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, BSP_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  SC_LSPO, KC_PSTRING, KC_BTN4, KC_BTN5, KC_BTN1, HK_DRAGSCROLL_MODE,            KC_LEFT, KC_DOWN,   KC_UP,KC_RIGHT, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
  SC_RSPC, XXXXXXX, XXXXXXX, KC_BTN3, KC_BTN2, HK_SNIPING_MODE,               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RALT, _______,  KC_SPC,     KC_ENT, _______, KC_LGUI
                                      //`--------------------------'  `--------------------------'
  ),

  [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                      KC_CIRC, KC_AMPR, KC_ASTR, KC_LPRN, KC_RPRN, KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_BACKSLASH, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_RALT, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

  [3] = LAYOUT_split_3x6_3(
  //,------------------------------------------------------------------------------.                   ,-----------------------------------------------------------------------------.
          QK_BOOT,     HK_DUMP,     HK_SAVE,     HK_RESET,     XXXXXXX, HK_C_SCROLL,                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     QK_BOOT,
  //|------------+------------+------------+-------------+------------+------------|                   |------------+------------+------------+------------+------------+------------|
      QK_C_EEPROM,  HK_P_SET_D,  HK_P_SET_S, HK_P_SET_BUF,     XXXXXXX, HK_S_MODE_T,                           KC_UP,     KC_DOWN,     XXXXXXX,     XXXXXXX,     XXXXXXX, QK_C_EEPROM,
  //|------------+------------+------------+-------------+------------+------------|                   |------------+------------+------------+------------+------------+------------|
          KC_LSFT,     XXXXXXX,     XXXXXXX,      XXXXXXX,     XXXXXXX, HK_D_MODE_T,                         XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     XXXXXXX,     KC_RSFT,
  //|------------+------------+------------+-------------+------------+------------+--------| |--------+------------+------------+------------+------------+------------+------------|
                                                               KC_LGUI,     _______,  KC_SPC,    KC_ENT,     _______,     KC_RALT
                                                       //`----------------------------------' `----------------------------------'
  )
};

uint16_t key_timer;

bool process_record_keymap(uint16_t keycode, keyrecord_t *record){
  static uint8_t saved_mods   = 0;

  switch (keycode){
    case BSP_DEL:
        if (record->event.pressed) {
            saved_mods = get_mods() & MOD_MASK_SHIFT;

            if (saved_mods == MOD_MASK_SHIFT) {  // Both shifts pressed
                register_code(KC_DEL);
            } else if (saved_mods) {   // One shift pressed
                del_mods(saved_mods);  // Remove any Shifts present
                register_code(KC_DEL);
                add_mods(saved_mods);  // Add shifts again
            } else {
                register_code(KC_BSPC);
            }
        } else {
            unregister_code(KC_DEL);
            unregister_code(KC_BSPC);
        }
        return false;
    case KC_PSTRING:
        if (record->event.pressed) {
            SEND_STRING(PASS);
        }
        break;
    case RCTL_T(KC_TABWIN):
        if (record->tap.count && record->event.pressed) {
            tap_code16(G(KC_GRV));
            return false;        // Return false to ignore further processing of key
        }
        break;
    case SWITCH:
    //     if (record->event.pressed) {
    //         key_timer = timer_read();
    //         register_code(KC_LALT);
    //     } else {
    //         unregister_code(KC_LALT);
    //         if (timer_elapsed(key_timer) < TAPPING_TERM) {
    //             set_single_persistent_default_layer(get_highest_layer(default_layer_state) == _QWERTY ? _BASE : _QWERTY);
    //             tap_code16(C(LOPT(KC_SPACE)));
    //         }
    //     }
    //     break;
    default:
        break;
  }

  return true;
}

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case RCTL_T(KC_TABWIN):
            return 150;
        default:
            return TAPPING_TERM;
    }
}
