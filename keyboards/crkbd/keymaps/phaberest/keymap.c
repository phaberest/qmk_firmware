#include QMK_KEYBOARD_H
#include "holykeebs.h"

// Tap dance state for ESC/TAB/FN layer

// Layer definitions
enum layers {
    _QWERTY = 0,
    _COLEMAK,
    _NAV,
    _NUM,
    _FN,
    _GAMING,
    _MOUSE
};

#define XXXXXXX KC_NO

// Manual auto mouse layer implementation
static uint32_t mouse_timer = 0;
static bool mouse_layer_active = false;
#define MOUSE_LAYER_TIMEOUT 1000  // 1 second timeout

#define KC_SLCK KC_SCROLL_LOCK

#define RALT_X RALT_T(KC_X)
#define RALT_DOT RALT_T(KC_DOT)

enum {
    TD_ESC_TAB,
};

enum custom_keycodes {
    RESET_KBD = SAFE_RANGE,
    TOGGLE_LAYOUT,
    BIT_0,
    BIT_1,
    BIT_2,
    BIT_3,
    BIT_4,
};

// Tap dance state for ESC/TAB/FN layer
typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD = 2,
    DOUBLE_TAP = 3,
};

// Forward declarations
void esc_tab_finished(tap_dance_state_t *state, void *user_data);
void esc_tab_reset(tap_dance_state_t *state, void *user_data);

// Determine the current tap dance state
uint8_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (!state->pressed) return SINGLE_TAP;
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        return DOUBLE_TAP;
    } else return 8; // Magic number. At some point this method will expand to work for more presses
}

// Initialize tap structure associated with example tap dance key
static tap esc_tab_tap_state = {
    .is_press_action = true,
    .state = 0
};

// Functions that control what our tap dance key does
void esc_tab_finished(tap_dance_state_t *state, void *user_data) {
    esc_tab_tap_state.state = cur_dance(state);
    switch (esc_tab_tap_state.state) {
        case SINGLE_TAP:
            tap_code(KC_ESC);
            break;
        case SINGLE_HOLD:
            layer_on(_MOUSE);
            break;
        case DOUBLE_TAP:
            tap_code(KC_TAB);
            break;
    }
}

void esc_tab_reset(tap_dance_state_t *state, void *user_data) {
    // If the key was held down and now is released then switch off the layer
    if (esc_tab_tap_state.state == SINGLE_HOLD) {
        layer_off(_MOUSE);
    }
    esc_tab_tap_state.state = 0;
}

tap_dance_action_t tap_dance_actions[] = {
    // Tap once for ESC, twice for TAB, hold for FN layer
    [TD_ESC_TAB] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, esc_tab_finished, esc_tab_reset),
};

#define TD_ESC_TAB TD(TD_ESC_TAB)

// Two-key combo state tracking variables
// Key position mapping for 3x6 split layout (0-35):
// Left:  0  1  2  3  4  5
//        12 13 14 15 16 17
//        24 25 26 27 28 29
// Right: 6  7  8  9  10 11
//        18 19 20 21 22 23
//        30 31 32 33 34 35

// Combo state variables for each key that participates in combos
static bool combo_keys[38] = {false};

// Timer-based combo detection
#define COMBO_DELAY_MS 35
typedef struct {
    uint16_t keycode;
    uint16_t timer;
    bool pending;
    int8_t key_pos;
} pending_key_t;

static pending_key_t pending_key = {0, 0, false, -1};

// ═══════════════════════════════════════════════════════════════════════════
// BITWISE FUNCTION KEY INPUT IMPLEMENTATION
// ═══════════════════════════════════════════════════════════════════════════

// Bitwise F-key state tracking
static int8_t bitwise_pressed_keys = 0;
static uint8_t bitwise_accumulator = 0;

// Define which keys act as bit inputs on the function layer (left home row)
// We'll use the BIT_0 through BIT_4 keycodes from the custom enum above
const uint16_t bitwise_f_keys[] = { BIT_0, BIT_1, BIT_2, BIT_3, BIT_4 };
uint8_t NUM_BITWISE_F_KEYS = sizeof(bitwise_f_keys) / sizeof(uint16_t);

// Function key mapping array (F1-F24)
uint16_t F_KEYS[] = {
    KC_NO,   // 0 - no function key
    KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,
    KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_F13,  KC_F14,  KC_F15,  KC_F16,
    KC_F17,  KC_F18,  KC_F19,  KC_F20,  KC_F21,  KC_F22,  KC_F23,  KC_F24,
    KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO    // 25-31 unused
};

// Process bitwise function key input
bool process_bitwise_f(uint16_t keycode, keyrecord_t *record) {
    // Check if we're on the function layer
    if (get_highest_layer(layer_state) != _FN) {
        return true; // Not on function layer, let normal processing continue
    }

    // Find if this keycode is one of our bitwise keys
    int index = -1;
    for (int i = 0; i < NUM_BITWISE_F_KEYS; ++i) {
        if (keycode == bitwise_f_keys[i]) {
            index = i;
            break;
        }
    }

    // If not a bitwise key, let normal processing continue
    if (index == -1) {
        return true;
    }

    // Handle bitwise key press/release
    if (record->event.pressed) {
        bitwise_pressed_keys++;
        bitwise_accumulator |= (1 << index);
    } else {
        bitwise_pressed_keys--;
        if (bitwise_pressed_keys <= 0) {
            bitwise_pressed_keys = 0;
            // Send the function key if accumulator is in valid range
            if (bitwise_accumulator > 0 && bitwise_accumulator < sizeof(F_KEYS)/sizeof(uint16_t)) {
                if (F_KEYS[bitwise_accumulator] != KC_NO) {
                    tap_code(F_KEYS[bitwise_accumulator]);
                }
            }
            bitwise_accumulator = 0;
        }
    }

    return false; // Don't process this key normally
}

// ═══════════════════════════════════════════════════════════════════════════

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 0: QWERTY BASE LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [0] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_GRV,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                       KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    TD_ESC_TAB,     KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                       KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN,  KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    MT(MOD_LSFT, KC_TAB), KC_Z, KC_X, KC_C, KC_V, KC_B,                             KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, MT(MOD_LSFT, KC_ENT),
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                KC_LALT, LT(2, KC_TAB), LT(3, KC_SPC),   MT(MOD_LSFT, KC_ENT), KC_LGUI, KC_RCTL
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 1: COLEMAK-DH BASE LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [1] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_GRV,     KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y, KC_SCLN, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    TD_ESC_TAB,     KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,  KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
    MT(MOD_LSFT, KC_TAB), KC_Z, KC_X, KC_C, KC_D, KC_V,                             KC_K, KC_H, KC_COMM, KC_DOT, KC_SLSH, MT(MOD_LSFT, KC_ENT),
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                KC_LALT, LT(2, KC_TAB), LT(3, KC_SPC),   MT(MOD_LSFT, KC_ENT), KC_LGUI, KC_RCTL
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 2: NAVIGATION / MEDIA LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [2] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, KC_MINS,  KC_EQL, KC_COPY,KC_PASTE, KC_VOLU,                      KC_PSCR, KC_SLCK, KC_PAUS, XXXXXXX, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, KC_VOLD,                      LALT(KC_LEFT), KC_LEFT, KC_DOWN,  KC_UP,  KC_RGHT, LALT(KC_RGHT),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, KC_MUTE,                      KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,   _______, _______, _______
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 3: NUMBERS / SYMBOLS LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [3] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, KC_BRIU,                      KC_MINS,   KC_7,    KC_8,    KC_9,  KC_EQL, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID,                      KC_PLUS,   KC_4,    KC_5,    KC_6, KC_ASTR, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, KC_COPY, KC_PASTE, XXXXXXX,                     KC_EQL,    KC_1,    KC_2,    KC_3, KC_SLSH, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,       LT(4, KC_ENT), KC_0, _______
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 4: FUNCTION KEYS LAYER (BITWISE INPUT)
    // ═══════════════════════════════════════════════════════════════════════════
    // Left home row uses bitwise input: A(1) + S(2) + D(4) + F(8) + G(16) = F1-F24
    // Example: A+D = 1+4 = 5 = F5, S+F = 2+8 = 10 = F10, etc.
    [4] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, TOGGLE_LAYOUT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, BIT_0,   BIT_1,   BIT_2,   BIT_3,   BIT_4,                        XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,    _______, _______, _______
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 5: GAMING LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [5] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_TAB,     KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LSFT,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            KC_LALT,  KC_ESC,  KC_SPC,     KC_ENT, KC_BSPC, KC_CAPS
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 6: MOUSE / UTILITIES LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [6] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      RESET_KBD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_WH_R, KC_WH_D, KC_WH_U, KC_WH_L, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,    KC_BTN1, KC_BTN3, KC_BTN2
                                        //`--------------------------'  `--------------------------'
    ),
};

bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
    // Handle bitwise function key input first
    if (!process_bitwise_f(keycode, record)) {
        return false;
    }

    switch (keycode) {
        case KC_ESC:
            // Detect the activation of only Left Alt
            if ((get_mods() & MOD_BIT(KC_LALT)) == MOD_BIT(KC_LALT)) {
                if (record->event.pressed) {
                    // No need to register KC_LALT because it's already active.
                    // The Alt modifier will apply on this KC_TAB.
                    register_code(KC_TAB);
                } else {
                    unregister_code(KC_TAB);
                }
                // Do not let QMK process the keycode further
                return false;
            }
            // Else, let QMK process the KC_ESC keycode as usual
            return true;
        case RESET_KBD:
            if (record->event.pressed) {
                soft_reset_keyboard();
            }
            return false;
        case TOGGLE_LAYOUT:
            if (record->event.pressed) {
                // Check current default layer and toggle between QWERTY and COLEMAK
                if (get_highest_layer(default_layer_state) == _QWERTY) {
                    // Currently QWERTY, switch to COLEMAK
                    default_layer_set(1UL << _COLEMAK);
                } else {
                    // Currently COLEMAK (or anything else), switch to QWERTY
                    default_layer_set(1UL << _QWERTY);
                }
            }
            return false;
    }

    // Handle two-key combos for symbols (works on both QWERTY and COLEMAK)
    // First, map QMK keycode to position index for combo tracking
    int8_t key_pos = -1;

    // Map QMK keycodes to our logical positions
    switch (keycode) {
        // Top row (positions 0-11)
        case KC_Q:    key_pos = 1; break;
        case KC_W:    key_pos = 2; break;
        case KC_E:    key_pos = 3; break;
        case KC_R:    key_pos = 4; break;
        case KC_T:    key_pos = 5; break;
        case KC_Y:    key_pos = 6; break;
        case KC_U:    key_pos = 7; break;
        case KC_I:    key_pos = 8; break;
        case KC_O:    key_pos = 9; break;
        case KC_P:    key_pos = 10; break;

        // Home row (positions 12-23)
        case KC_A:    key_pos = 13; break;
        case KC_S:    key_pos = 14; break;
        case KC_D:    key_pos = 15; break;
        case KC_F:    key_pos = 16; break;
        case KC_G:    key_pos = 17; break;
        case KC_H:    key_pos = 18; break;
        case KC_J:    key_pos = 19; break;
        case KC_K:    key_pos = 20; break;
        case KC_L:    key_pos = 21; break;
        case KC_SCLN: key_pos = 22; break;

        // Bottom row (positions 24-35)
        case KC_Z:    key_pos = 25; break;
        case KC_X:    key_pos = 26; break;
        case KC_C:    key_pos = 27; break;
        case KC_V:    key_pos = 28; break;
        case KC_B:    key_pos = 29; break;
        case KC_N:    key_pos = 30; break;
        case KC_M:    key_pos = 31; break;
        case KC_COMM: key_pos = 32; break;
        case KC_DOT:  key_pos = 33; break;
        case KC_SLSH: key_pos = 34; break;

        // Number keys for combos
        case KC_4:    key_pos = 36; break;
        case KC_5:    key_pos = 37; break;
    }

    // Update combo key state if this is a combo-participating key
    if (key_pos >= 0) {
        combo_keys[key_pos] = record->event.pressed;

        // Check if this is a combo key that needs delay processing
        bool is_combo_key = (key_pos == 15 || key_pos == 16 ||  // D,F
                             key_pos == 19 || key_pos == 20 ||  // J,K
                             key_pos == 3 || key_pos == 4 ||    // E,R
                             key_pos == 7 || key_pos == 8 ||    // U,I
                             key_pos == 27 || key_pos == 28 ||  // C,V
                             key_pos == 31 || key_pos == 32 ||  // M,COMM
                             key_pos == 2 || key_pos == 9 ||    // W,E and I,O
                             key_pos == 13 || key_pos == 14 ||  // A,S and S,D
                             key_pos == 29 || key_pos == 30 ||  // B,N
                             key_pos == 36 || key_pos == 37);   // 4,5 (number layer)

        if (record->event.pressed && is_combo_key) {
            // Check if combo is triggered immediately
            bool combo_triggered = false;

            // Check all two-key combos to see if this key completes one
            // Left parenthesis: positions 15,16 (D,F)
            if ((key_pos == 15 || key_pos == 16) && combo_keys[15] && combo_keys[16]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_9);
                unregister_code(KC_9);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[15] = combo_keys[16] = false;
                combo_triggered = true;
            }
            // Right parenthesis: positions 19,20 (J,K)
            else if ((key_pos == 19 || key_pos == 20) && combo_keys[19] && combo_keys[20]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_0);
                unregister_code(KC_0);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[19] = combo_keys[20] = false;
                combo_triggered = true;
            }
            // Left bracket: positions 3,4 (E,R)
            else if ((key_pos == 3 || key_pos == 4) && combo_keys[3] && combo_keys[4]) {
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                combo_keys[3] = combo_keys[4] = false;
                combo_triggered = true;
            }
            // Right bracket: positions 7,8 (U,I)
            else if ((key_pos == 7 || key_pos == 8) && combo_keys[7] && combo_keys[8]) {
                register_code(KC_RBRC);
                unregister_code(KC_RBRC);
                combo_keys[7] = combo_keys[8] = false;
                combo_triggered = true;
            }
            // Left brace: positions 27,28 (C,V)
            else if ((key_pos == 27 || key_pos == 28) && combo_keys[27] && combo_keys[28]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_LBRC);
                unregister_code(KC_LBRC);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[27] = combo_keys[28] = false;
                combo_triggered = true;
            }
            // Right brace: positions 31,32 (M,COMM)
            else if ((key_pos == 31 || key_pos == 32) && combo_keys[31] && combo_keys[32]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_RBRC);
                unregister_code(KC_RBRC);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[31] = combo_keys[32] = false;
                combo_triggered = true;
            }
            // Less than: positions 2,3 (W,E)
            else if ((key_pos == 2 || key_pos == 3) && combo_keys[2] && combo_keys[3]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_COMM);
                unregister_code(KC_COMM);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[2] = combo_keys[3] = false;
                combo_triggered = true;
            }
            // Greater than: positions 8,9 (I,O)
            else if ((key_pos == 8 || key_pos == 9) && combo_keys[8] && combo_keys[9]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_DOT);
                unregister_code(KC_DOT);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[8] = combo_keys[9] = false;
                combo_triggered = true;
            }
            // Tilde: positions 13,14 (A,S)
            else if ((key_pos == 13 || key_pos == 14) && combo_keys[13] && combo_keys[14]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[13] = combo_keys[14] = false;
                combo_triggered = true;
            }
            // Grave: positions 14,15 (S,D)
            else if ((key_pos == 14 || key_pos == 15) && combo_keys[14] && combo_keys[15]) {
                register_code(KC_GRV);
                unregister_code(KC_GRV);
                combo_keys[14] = combo_keys[15] = false;
                combo_triggered = true;
            }
            // Plus: positions 4,7 (R,U)
            else if ((key_pos == 4 || key_pos == 7) && combo_keys[4] && combo_keys[7]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_EQL);
                unregister_code(KC_EQL);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[4] = combo_keys[7] = false;
                combo_triggered = true;
            }
            // Minus: positions 28,31 (V,M)
            else if ((key_pos == 28 || key_pos == 31) && combo_keys[28] && combo_keys[31]) {
                register_code(KC_MINS);
                unregister_code(KC_MINS);
                combo_keys[28] = combo_keys[31] = false;
                combo_triggered = true;
            }
            // Equal: positions 16,19 (F,J)
            else if ((key_pos == 16 || key_pos == 19) && combo_keys[16] && combo_keys[19]) {
                register_code(KC_EQL);
                unregister_code(KC_EQL);
                combo_keys[16] = combo_keys[19] = false;
                combo_triggered = true;
            }
            // Asterisk: positions 3,8 (E,I)
            else if ((key_pos == 3 || key_pos == 8) && combo_keys[3] && combo_keys[8]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_8);
                unregister_code(KC_8);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[3] = combo_keys[8] = false;
                combo_triggered = true;
            }
            // Underscore: positions 29,30 (B,N)
            else if ((key_pos == 29 || key_pos == 30) && combo_keys[29] && combo_keys[30]) {
                register_mods(MOD_BIT(KC_LSFT));
                register_code(KC_MINS);
                unregister_code(KC_MINS);
                unregister_mods(MOD_BIT(KC_LSFT));
                combo_keys[29] = combo_keys[30] = false;
                combo_triggered = true;
            }
            // Option+Backspace (word delete): positions 36,37 (4,5 on number layer)
            else if ((key_pos == 36 || key_pos == 37) && combo_keys[36] && combo_keys[37]) {
                register_mods(MOD_BIT(KC_LALT));
                register_code(KC_BSPC);
                unregister_code(KC_BSPC);
                unregister_mods(MOD_BIT(KC_LALT));
                combo_keys[36] = combo_keys[37] = false;
                combo_triggered = true;
            }

            // If combo was triggered, don't process individually
            if (combo_triggered) {
                pending_key.pending = false; // Cancel any pending key
                return false;
            }

            // If no combo triggered and no pending key, start timer for this key
            if (!pending_key.pending) {
                pending_key.keycode = keycode;
                pending_key.timer = timer_read();
                pending_key.pending = true;
                pending_key.key_pos = key_pos;
                return false; // Don't process immediately
            }

            // If there's already a pending key, send both without combo
            if (pending_key.pending) {
                // Send the pending key first
                tap_code(pending_key.keycode);
                pending_key.pending = false;
                // Then send current key normally
                return true;
            }
        }

        // Handle key release for combo keys
        if (!record->event.pressed && is_combo_key) {
            // If releasing a pending key without combo, send it
            if (pending_key.pending && pending_key.keycode == keycode) {
                tap_code(keycode);
                pending_key.pending = false;
                return false;
            }
        }
    }

    return true;
}

bool wpm_keycode_user(uint16_t keycode) {
    if ((keycode >= QK_MOD_TAP && keycode <= QK_MOD_TAP_MAX) || (keycode >= QK_LAYER_TAP && keycode <= QK_LAYER_TAP_MAX) || (keycode >= QK_MODS && keycode <= QK_MODS_MAX)) {
        keycode = keycode & 0xFF;
    } else if (keycode > 0xFF) {
        keycode = 0;
    }
    if (keycode >= KC_A && keycode <= KC_0) {
        return true;
    }

    return false;
}

// Check mouse layer timeout regularly
void matrix_scan_user(void) {
    // Handle mouse layer timeout (handled in matrix_scan_keymap and pointing_device_task_keymap)

    // Handle pending combo key timeout
    if (pending_key.pending && timer_elapsed(pending_key.timer) > COMBO_DELAY_MS) {
        // Send the pending key as individual keypress
        tap_code(pending_key.keycode);
        pending_key.pending = false;
    }
}

void matrix_scan_keymap(void) {
    if (mouse_layer_active && timer_elapsed32(mouse_timer) > MOUSE_LAYER_TIMEOUT) {
        layer_off(_MOUSE);
        mouse_layer_active = false;
    }
}

// This function is called by the holykeebs user code
report_mouse_t pointing_device_task_keymap(report_mouse_t mouse_report) {
    // Check if mouse movement is detected
    if (mouse_report.x != 0 || mouse_report.y != 0) {
        // Activate mouse layer and reset timer
        if (!mouse_layer_active) {
            layer_on(_MOUSE);
            mouse_layer_active = true;
        }
        mouse_timer = timer_read32();
    }

    // Check if we should deactivate the mouse layer due to inactivity
    if (mouse_layer_active && timer_elapsed32(mouse_timer) > MOUSE_LAYER_TIMEOUT) {
        layer_off(_MOUSE);
        mouse_layer_active = false;
    }

    return mouse_report;
}
