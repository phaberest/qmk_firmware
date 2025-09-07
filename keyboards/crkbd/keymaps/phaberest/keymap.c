#include QMK_KEYBOARD_H
#include "enums.h"

// Tap dance state for ESC/TAB/FN layer

#define XXXXXXX KC_NO

// Manual auto mouse layer implementation
static uint32_t mouse_timer = 0;
static bool mouse_layer_active = false;
#define MOUSE_LAYER_TIMEOUT 1000  // 1 second timeout

// Caps lock toggle state tracking
static bool left_shift_pressed = false;
static bool right_shift_pressed = false;
static bool caps_toggle_triggered = false;

// Chat mode state tracking
static bool chat_mode_active = false;

#define KC_SLCK KC_SCROLL_LOCK

#define RALT_X RALT_T(KC_X)
#define RALT_DOT RALT_T(KC_DOT)

enum custom_keycodes {
    RESET_KBD = SAFE_RANGE,
    TOGGLE_LAYOUT,
    BIT_0,
    BIT_1,
    BIT_2,
    BIT_3,
    BIT_4,
    // Emoji keycodes
    EMOJI_HANDS,     // 🙌🏼
    EMOJI_LAUGH,     // 😂
    EMOJI_HEART,     // ❤️
    SYMBOL_EURO,     // €
    CAPS_TOGGLE,     // Custom caps lock toggle
    // Additional symbols for numbers layer
    SYMBOL_ARROW,    // =>
    SYMBOL_PERCENT,  // %
    SYMBOL_CARET,    // ^
    SYMBOL_DOLLAR,   // $
    TOGGLE_GAMING,   // Toggle gaming layer on
    EXIT_GAMING,     // Exit gaming layer
    CHAT_MODE,       // Temporary chat mode in gaming
};

// QMK Native Combo Definitions
// Define combo key sequences for QWERTY layout

// Parentheses combos
const uint16_t PROGMEM combo_lparen[] = {KC_D, KC_F, COMBO_END}; // Left parenthesis (
const uint16_t PROGMEM combo_rparen[] = {KC_J, KC_K, COMBO_END}; // Right parenthesis )

// Brackets combos
const uint16_t PROGMEM combo_lbracket[] = {KC_E, KC_R, COMBO_END}; // Left bracket [
const uint16_t PROGMEM combo_rbracket[] = {KC_U, KC_I, COMBO_END}; // Right bracket ]

// Braces combos
const uint16_t PROGMEM combo_lbrace[] = {KC_C, KC_V, COMBO_END}; // Left brace {
const uint16_t PROGMEM combo_rbrace[] = {KC_M, KC_COMM, COMBO_END}; // Right brace }

// Comparison operators
const uint16_t PROGMEM combo_less[] = {KC_W, KC_E, COMBO_END}; // Less than <
const uint16_t PROGMEM combo_greater[] = {KC_I, KC_O, COMBO_END}; // Greater than >

// Special characters
const uint16_t PROGMEM combo_tilde[] = {KC_A, KC_S, COMBO_END}; // Tilde ~
const uint16_t PROGMEM combo_grave[] = {KC_S, KC_D, COMBO_END}; // Grave `

// Math operators
const uint16_t PROGMEM combo_plus[] = {KC_R, KC_U, COMBO_END}; // Plus +
const uint16_t PROGMEM combo_minus[] = {KC_V, KC_M, COMBO_END}; // Minus -
const uint16_t PROGMEM combo_equal[] = {KC_F, KC_J, COMBO_END}; // Equal =
const uint16_t PROGMEM combo_asterisk[] = {KC_E, KC_I, COMBO_END}; // Asterisk *
const uint16_t PROGMEM combo_underscore[] = {KC_R, KC_T, COMBO_END}; // Underscore _

// Special functions
const uint16_t PROGMEM combo_word_del[] = {KC_4, KC_5, COMBO_END}; // Word delete (Alt+Backspace)

// Emoji combos
const uint16_t PROGMEM combo_emoji_hands[] = {KC_T, KC_Y, COMBO_END}; // Raising hands emoji 🙌🏼
const uint16_t PROGMEM combo_emoji_laugh[] = {KC_G, KC_H, COMBO_END}; // Laughing emoji 😂
const uint16_t PROGMEM combo_emoji_heart[] = {KC_B, KC_N, COMBO_END}; // Heart emoji ❤️

// Additional symbol combos
const uint16_t PROGMEM combo_exclamation[] = {KC_Y, KC_U, COMBO_END}; // Exclamation mark !
const uint16_t PROGMEM combo_ampersand[] = {KC_D, KC_K, COMBO_END}; // Ampersand &
const uint16_t PROGMEM combo_dollar[] = {KC_S, KC_L, COMBO_END}; // Dollar sign $
const uint16_t PROGMEM combo_hash[] = {KC_F, KC_G, COMBO_END}; // Hash symbol #
const uint16_t PROGMEM combo_at[] = {KC_H, KC_J, COMBO_END}; // At symbol @
const uint16_t PROGMEM combo_percent[] = {KC_V, KC_B, COMBO_END}; // Percent symbol %
const uint16_t PROGMEM combo_caret[] = {KC_N, KC_M, COMBO_END}; // Caret symbol ^

// Colemak-DH layout combos (same positions, different keys)
const uint16_t PROGMEM combo_lparen_colemak[] = {KC_S, KC_T, COMBO_END}; // Left parenthesis ( (Colemak)
const uint16_t PROGMEM combo_rparen_colemak[] = {KC_N, KC_E, COMBO_END}; // Right parenthesis ) (Colemak)
const uint16_t PROGMEM combo_lbracket_colemak[] = {KC_F, KC_P, COMBO_END}; // Left bracket [ (Colemak)
const uint16_t PROGMEM combo_rbracket_colemak[] = {KC_L, KC_U, COMBO_END}; // Right bracket ] (Colemak)
const uint16_t PROGMEM combo_lbrace_colemak[] = {KC_C, KC_D, COMBO_END}; // Left brace { (Colemak)
const uint16_t PROGMEM combo_rbrace_colemak[] = {KC_H, KC_COMM, COMBO_END}; // Right brace } (Colemak)
const uint16_t PROGMEM combo_less_colemak[] = {KC_W, KC_F, COMBO_END}; // Less than < (Colemak)
const uint16_t PROGMEM combo_greater_colemak[] = {KC_U, KC_Y, COMBO_END}; // Greater than > (Colemak)
const uint16_t PROGMEM combo_tilde_colemak[] = {KC_A, KC_R, COMBO_END}; // Tilde ~ (Colemak)
const uint16_t PROGMEM combo_grave_colemak[] = {KC_R, KC_S, COMBO_END}; // Grave ` (Colemak)
const uint16_t PROGMEM combo_plus_colemak[] = {KC_P, KC_L, COMBO_END}; // Plus + (Colemak)
const uint16_t PROGMEM combo_minus_colemak[] = {KC_D, KC_H, COMBO_END}; // Minus - (Colemak)
const uint16_t PROGMEM combo_equal_colemak[] = {KC_T, KC_N, COMBO_END}; // Equal = (Colemak)
const uint16_t PROGMEM combo_asterisk_colemak[] = {KC_F, KC_U, COMBO_END}; // Asterisk * (Colemak)
const uint16_t PROGMEM combo_underscore_colemak[] = {KC_P, KC_B, COMBO_END}; // Underscore _ (Colemak) - R+T equivalent
const uint16_t PROGMEM combo_emoji_hands_colemak[] = {KC_B, KC_J, COMBO_END}; // Raising hands emoji 🙌🏼 (Colemak)
const uint16_t PROGMEM combo_emoji_laugh_colemak[] = {KC_G, KC_M, COMBO_END}; // Laughing emoji 😂 (Colemak)
const uint16_t PROGMEM combo_emoji_heart_colemak[] = {KC_V, KC_K, COMBO_END}; // Heart emoji ❤️ (Colemak) - B+N equivalent
const uint16_t PROGMEM combo_exclamation_colemak[] = {KC_J, KC_L, COMBO_END}; // Exclamation mark ! (Colemak)
const uint16_t PROGMEM combo_ampersand_colemak[] = {KC_S, KC_E, COMBO_END}; // Ampersand & (Colemak)
const uint16_t PROGMEM combo_dollar_colemak[] = {KC_R, KC_I, COMBO_END}; // Dollar sign $ (Colemak)
const uint16_t PROGMEM combo_hash_colemak[] = {KC_T, KC_G, COMBO_END}; // Hash symbol # (Colemak)
const uint16_t PROGMEM combo_at_colemak[] = {KC_M, KC_N, COMBO_END}; // At symbol @ (Colemak)
const uint16_t PROGMEM combo_percent_colemak[] = {KC_D, KC_V, COMBO_END}; // Percent symbol % (Colemak)
const uint16_t PROGMEM combo_caret_colemak[] = {KC_K, KC_H, COMBO_END}; // Caret symbol ^ (Colemak)

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
    if (get_highest_layer(layer_state) != _F) {
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

// Define the combo arrays for QMK native combo system
combo_t key_combos[] = {
    // QWERTY layout combos
    COMBO(combo_lparen, LSFT(KC_9)),           // Left parenthesis (
    COMBO(combo_rparen, LSFT(KC_0)),           // Right parenthesis )
    COMBO(combo_lbracket, KC_LBRC),             // Left bracket [
    COMBO(combo_rbracket, KC_RBRC),             // Right bracket ]
    COMBO(combo_lbrace, LSFT(KC_LBRC)),         // Left brace {
    COMBO(combo_rbrace, LSFT(KC_RBRC)),         // Right brace }
    COMBO(combo_less, LSFT(KC_COMM)),           // Less than <
    COMBO(combo_greater, LSFT(KC_DOT)),         // Greater than >
    COMBO(combo_tilde, LSFT(KC_GRV)),           // Tilde ~
    COMBO(combo_grave, KC_GRV),                 // Grave `
    COMBO(combo_plus, LSFT(KC_EQL)),            // Plus +
    COMBO(combo_minus, KC_MINS),                // Minus -
    COMBO(combo_equal, KC_EQL),                 // Equal =
    COMBO(combo_asterisk, LSFT(KC_8)),          // Asterisk *
    COMBO(combo_underscore, LSFT(KC_MINS)),     // Underscore _
    COMBO(combo_word_del, LALT(KC_BSPC)),       // Word delete (Alt+Backspace)
    COMBO(combo_emoji_hands, EMOJI_HANDS),      // Raising hands emoji 🙌🏼
    COMBO(combo_emoji_laugh, EMOJI_LAUGH),      // Laughing emoji 😂
    COMBO(combo_emoji_heart, EMOJI_HEART),      // Heart emoji ❤️
    COMBO(combo_exclamation, LSFT(KC_1)),       // Exclamation mark !
    COMBO(combo_ampersand, LSFT(KC_7)),         // Ampersand &
    COMBO(combo_dollar, LSFT(KC_4)),            // Dollar sign $
    COMBO(combo_hash, LSFT(KC_3)),              // Hash symbol #
    COMBO(combo_at, LSFT(KC_2)),                // At symbol @
    COMBO(combo_percent, LSFT(KC_5)),           // Percent symbol %
    COMBO(combo_caret, LSFT(KC_6)),             // Caret symbol ^
    
    // Colemak-DH layout combos (same outputs, different key combinations)
    COMBO(combo_lparen_colemak, LSFT(KC_9)),           // Left parenthesis ( (Colemak)
    COMBO(combo_rparen_colemak, LSFT(KC_0)),           // Right parenthesis ) (Colemak)
    COMBO(combo_lbracket_colemak, KC_LBRC),             // Left bracket [ (Colemak)
    COMBO(combo_rbracket_colemak, KC_RBRC),             // Right bracket ] (Colemak)
    COMBO(combo_lbrace_colemak, LSFT(KC_LBRC)),         // Left brace { (Colemak)
    COMBO(combo_rbrace_colemak, LSFT(KC_RBRC)),         // Right brace } (Colemak)
    COMBO(combo_less_colemak, LSFT(KC_COMM)),           // Less than < (Colemak)
    COMBO(combo_greater_colemak, LSFT(KC_DOT)),         // Greater than > (Colemak)
    COMBO(combo_tilde_colemak, LSFT(KC_GRV)),           // Tilde ~ (Colemak)
    COMBO(combo_grave_colemak, KC_GRV),                 // Grave ` (Colemak)
    COMBO(combo_plus_colemak, LSFT(KC_EQL)),            // Plus + (Colemak)
    COMBO(combo_minus_colemak, KC_MINS),                // Minus - (Colemak)
    COMBO(combo_equal_colemak, KC_EQL),                 // Equal = (Colemak)
    COMBO(combo_asterisk_colemak, LSFT(KC_8)),          // Asterisk * (Colemak)
    COMBO(combo_underscore_colemak, LSFT(KC_MINS)),     // Underscore _ (Colemak)
    COMBO(combo_emoji_hands_colemak, EMOJI_HANDS),      // Raising hands emoji 🙌🏼 (Colemak)
    COMBO(combo_emoji_laugh_colemak, EMOJI_LAUGH),      // Laughing emoji 😂 (Colemak)
    COMBO(combo_emoji_heart_colemak, EMOJI_HEART),      // Heart emoji ❤️ (Colemak)
    COMBO(combo_exclamation_colemak, LSFT(KC_1)),       // Exclamation mark ! (Colemak)
    COMBO(combo_ampersand_colemak, LSFT(KC_7)),         // Ampersand & (Colemak)
    COMBO(combo_dollar_colemak, LSFT(KC_4)),            // Dollar sign $ (Colemak)
    COMBO(combo_hash_colemak, LSFT(KC_3)),              // Hash symbol # (Colemak)
    COMBO(combo_at_colemak, LSFT(KC_2)),                // At symbol @ (Colemak)
    COMBO(combo_percent_colemak, LSFT(KC_5)),           // Percent symbol % (Colemak)
    COMBO(combo_caret_colemak, LSFT(KC_6)),             // Caret symbol ^ (Colemak)
};

uint16_t COMBO_LEN = ARRAY_SIZE(key_combos);

// Combo indices for layout switching
enum combo_indices {
    // QWERTY layout combo indices (0-25)
    COMBO_LPAREN = 0,
    COMBO_RPAREN,
    COMBO_LBRACKET,
    COMBO_RBRACKET,
    COMBO_LBRACE,
    COMBO_RBRACE,
    COMBO_LESS,
    COMBO_GREATER,
    COMBO_TILDE,
    COMBO_GRAVE,
    COMBO_PLUS,
    COMBO_MINUS,
    COMBO_EQUAL,
    COMBO_ASTERISK,
    COMBO_UNDERSCORE,
    COMBO_WORD_DEL,
    COMBO_EMOJI_HANDS,
    COMBO_EMOJI_LAUGH,
    COMBO_EMOJI_HEART,
    COMBO_EXCLAMATION,
    COMBO_AMPERSAND,
    COMBO_DOLLAR,
    COMBO_HASH,
    COMBO_AT,
    COMBO_PERCENT,
    COMBO_CARET,
    
    // Colemak layout combo indices (26-51)
    COMBO_LPAREN_COLEMAK,
    COMBO_RPAREN_COLEMAK,
    COMBO_LBRACKET_COLEMAK,
    COMBO_RBRACKET_COLEMAK,
    COMBO_LBRACE_COLEMAK,
    COMBO_RBRACE_COLEMAK,
    COMBO_LESS_COLEMAK,
    COMBO_GREATER_COLEMAK,
    COMBO_TILDE_COLEMAK,
    COMBO_GRAVE_COLEMAK,
    COMBO_PLUS_COLEMAK,
    COMBO_MINUS_COLEMAK,
    COMBO_EQUAL_COLEMAK,
    COMBO_ASTERISK_COLEMAK,
    COMBO_UNDERSCORE_COLEMAK,
    COMBO_EMOJI_HANDS_COLEMAK,
    COMBO_EMOJI_LAUGH_COLEMAK,
    COMBO_EMOJI_HEART_COLEMAK,
    COMBO_EXCLAMATION_COLEMAK,
    COMBO_AMPERSAND_COLEMAK,
    COMBO_DOLLAR_COLEMAK,
    COMBO_HASH_COLEMAK,
    COMBO_AT_COLEMAK,
    COMBO_PERCENT_COLEMAK,
    COMBO_CARET_COLEMAK,
    
    COMBO_COUNT
};

#define QWERTY_COMBO_START 0
#define QWERTY_COMBO_END 25
#define COLEMAK_COMBO_START 26
#define COLEMAK_COMBO_END 51

// Conditional combo triggering based on current layout
bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    bool is_colemak = (get_highest_layer(default_layer_state) == _COLEMAK);
    
    if (is_colemak) {
        // In Colemak mode, only allow Colemak combos
        return (combo_index >= COLEMAK_COMBO_START && combo_index <= COLEMAK_COMBO_END);
    } else {
        // In QWERTY mode, only allow QWERTY combos
        return (combo_index >= QWERTY_COMBO_START && combo_index <= QWERTY_COMBO_END);
    }
}

// ═══════════════════════════════════════════════════════════════════════════

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 0: QWERTY BASE LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [0] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_GRV,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,   KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        LT(6, KC_ESC),  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,   KC_J,    KC_K,    KC_L,   KC_SCLN,  KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,   KC_M,   KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                KC_LALT, LT(2, KC_TAB), LT(3, KC_SPC),   MT(MOD_LSFT, KC_ENT), KC_LGUI, KC_RCTL
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 1: COLEMAK-DH BASE LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [1] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        KC_GRV,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,                         KC_J,    KC_L,    KC_U,    KC_Y,  KC_SCLN, KC_BSPC,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        LT(6, KC_ESC),  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,                         KC_M,    KC_N,    KC_E,    KC_I,   KC_O,   KC_QUOT,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
       KC_LSFT,   KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,                         KC_K,    KC_H,  KC_COMM,  KC_DOT, KC_SLSH, KC_RSFT,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                KC_LALT, LT(2, KC_TAB), LT(3, KC_SPC),   MT(MOD_LSFT, KC_ENT), KC_LGUI, KC_RCTL
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 2: NAVIGATION / MEDIA LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [2] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, XXXXXXX, KC_VOLD, KC_MUTE, KC_VOLU, KC_BRIU,                      KC_PSCR, KC_SLCK, KC_PAUS, XXXXXXX, XXXXXXX, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT, KC_BRID,                      LALT(KC_LEFT), KC_LEFT, KC_DOWN,  KC_UP,  KC_RGHT, LALT(KC_RGHT),
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, KC_COPY, KC_PASTE, XXXXXXX,                      KC_INS, KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,   _______, _______, _______
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 3: NUMBERS / SYMBOLS LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [3] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        _______, SYMBOL_PERCENT, SYMBOL_CARET, SYMBOL_EURO, SYMBOL_DOLLAR, SYMBOL_ARROW,                      KC_MINS,   KC_7,    KC_8,    KC_9,  KC_EQL, _______,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_PLUS,   KC_4,    KC_5,    KC_6, KC_ASTR, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                     KC_EQL,    KC_1,    KC_2,    KC_3, KC_SLSH, _______,
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
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX,   XXXXXXX,   XXXXXXX,   XXXXXXX, XXXXXXX, TOGGLE_GAMING,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
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
        KC_LCTL,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, EXIT_GAMING,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            KC_LALT,  KC_ESC,  KC_SPC,     KC_ENT, KC_BSPC, CHAT_MODE
                                        //`--------------------------'  `--------------------------'
    ),

    // ═══════════════════════════════════════════════════════════════════════════
    // LAYER 6: MOUSE / UTILITIES LAYER
    // ═══════════════════════════════════════════════════════════════════════════
    [6] = LAYOUT_split_3x6_3(
    //,-----------------------------------------------------.                    ,-----------------------------------------------------.
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                 XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
        RESET_KBD, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                    XXXXXXX, KC_WH_R, KC_WH_D, KC_WH_U, KC_WH_L, XXXXXXX,
    //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                            _______, _______, _______,    KC_BTN1, KC_BTN3, KC_BTN2
                                        //`--------------------------'  `--------------------------'
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle bitwise function key input first
    if (!process_bitwise_f(keycode, record)) {
        return false;
    }

    switch (keycode) {
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
                // Note: combo_should_trigger will automatically handle the layout switch
            }
            return false;
        case SYMBOL_EURO:
            if (record->event.pressed) {
                // Use Option+Shift+2 for Euro symbol on macOS
                register_mods(MOD_LALT | MOD_LSFT);
                register_code(KC_2);
                unregister_code(KC_2);
                unregister_mods(MOD_LALT | MOD_LSFT);
            }
            return false;
        case SYMBOL_ARROW:
            if (record->event.pressed) {
                SEND_STRING("=>");  // Arrow symbol
            }
            return false;
        case SYMBOL_PERCENT:
            if (record->event.pressed) {
                SEND_STRING("%");  // Percent symbol
            }
            return false;
        case SYMBOL_CARET:
            if (record->event.pressed) {
                SEND_STRING("^");  // Caret symbol
            }
            return false;
        case SYMBOL_DOLLAR:
            if (record->event.pressed) {
                SEND_STRING("$");  // Dollar symbol
            }
            return false;
        case EMOJI_HANDS:
            if (record->event.pressed) {
                // Open emoji picker and type "raised hands"
                register_mods(MOD_LCTL | MOD_LGUI);
                register_code(KC_SPC);
                unregister_code(KC_SPC);
                unregister_mods(MOD_LCTL | MOD_LGUI);
                wait_ms(200);
                SEND_STRING("raised hands");
                wait_ms(50);
                tap_code(KC_ENT);
            }
            return false;
        case EMOJI_LAUGH:
            if (record->event.pressed) {
                // Open emoji picker and type "joy"
                register_mods(MOD_LCTL | MOD_LGUI);
                register_code(KC_SPC);
                unregister_code(KC_SPC);
                unregister_mods(MOD_LCTL | MOD_LGUI);
                wait_ms(200);
                SEND_STRING("joy");
                wait_ms(50);
                tap_code(KC_ENT);
            }
            return false;
        case EMOJI_HEART:
            if (record->event.pressed) {
                // Open emoji picker and type "heart"
                register_mods(MOD_LCTL | MOD_LGUI);
                register_code(KC_SPC);
                unregister_code(KC_SPC);
                unregister_mods(MOD_LCTL | MOD_LGUI);
                wait_ms(200);
                SEND_STRING("heart");
                wait_ms(50);
                tap_code(KC_ENT);
            }
            return false;
        case CAPS_TOGGLE:
            if (record->event.pressed) {
                tap_code(KC_CAPS);  // Toggle caps lock
            }
            return false;
        case TOGGLE_GAMING:
            if (record->event.pressed) {
                layer_on(_GAME);  // Turn on gaming layer
            }
            return false;
        case EXIT_GAMING:
            if (record->event.pressed) {
                layer_off(_GAME);  // Turn off gaming layer
                chat_mode_active = false;  // Reset chat mode when exiting gaming
            }
            return false;
        case CHAT_MODE:
            if (record->event.pressed) {
                chat_mode_active = true;
                // Switch to base layer temporarily (QWERTY or COLEMAK based on default)
                if (get_highest_layer(default_layer_state) == _COLEMAK) {
                    layer_on(_COLEMAK);
                } else {
                    layer_on(_QWERTY);
                }
                layer_off(_GAME);  // Turn off gaming layer temporarily
            }
            return false;
    }

    // Handle Enter key in chat mode - return to gaming
    if (keycode == KC_ENT && chat_mode_active && record->event.pressed) {
        chat_mode_active = false;
        // Turn off base layers and return to gaming
        layer_off(_QWERTY);
        layer_off(_COLEMAK);
        layer_on(_GAME);
        // Let the Enter key press through
        return true;
    }

    // Handle shift key tracking for caps lock toggle
    // Check for both shift keys being pressed (regular shift keys)
    if (keycode == KC_LSFT) {  // Left shift
        if (record->event.pressed) {
            left_shift_pressed = true;
        } else {
            left_shift_pressed = false;
            caps_toggle_triggered = false;  // Reset when releasing
        }
    } else if (keycode == KC_RSFT) {  // Right shift
        if (record->event.pressed) {
            right_shift_pressed = true;
        } else {
            right_shift_pressed = false;
            caps_toggle_triggered = false;  // Reset when releasing
        }
    }

    // Check if both shifts are pressed and we haven't triggered yet
    if (left_shift_pressed && right_shift_pressed && !caps_toggle_triggered) {
        caps_toggle_triggered = true;
        tap_code(KC_CAPS);  // Toggle caps lock
        return false;  // Don't process the key further
    }

    // Custom combo logic removed - now using QMK native combos

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
    if (mouse_layer_active && timer_elapsed32(mouse_timer) > MOUSE_LAYER_TIMEOUT) {
        layer_off(_MOUSE);
        mouse_layer_active = false;
    }
}

// Mouse handling function
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
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

// No initialization needed - combo_should_trigger handles layout switching automatically
