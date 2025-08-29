#include QMK_KEYBOARD_H
#include "combo_mappings.h"

// Physical position mappings (same positions across layouts)
static const combo_key_mapping_t combo_key_mappings[] = {
    // Top row (positions 0-11)
    {KC_Q,    KC_Q,    1},   // Q stays Q
    {KC_W,    KC_W,    2},   // W stays W
    {KC_E,    KC_F,    3},   // E -> F
    {KC_R,    KC_P,    4},   // R -> P
    {KC_T,    KC_B,    5},   // T -> B
    {KC_Y,    KC_J,    6},   // Y -> J
    {KC_U,    KC_L,    7},   // U -> L
    {KC_I,    KC_U,    8},   // I -> U
    {KC_O,    KC_Y,    9},   // O -> Y
    {KC_P,    KC_SCLN, 10}, // P -> ; (SCLN)

    // Home row (positions 12-23)
    {KC_A,    KC_A,    13},  // A stays A
    {KC_S,    KC_R,    14},  // S -> R
    {KC_D,    KC_S,    15},  // D -> S
    {KC_F,    KC_T,    16},  // F -> T
    {KC_G,    KC_G,    17},  // G stays G
    {KC_H,    KC_M,    18},  // H -> M
    {KC_J,    KC_N,    19},  // J -> N
    {KC_K,    KC_E,    20},  // K -> E
    {KC_L,    KC_I,    21},  // L -> I
    {KC_SCLN, KC_O,    22},  // ; -> O

    // Bottom row (positions 24-35)
    {KC_Z,    KC_Z,    25},  // Z stays Z
    {KC_X,    KC_X,    26},  // X stays X
    {KC_C,    KC_C,    27},  // C stays C
    {KC_V,    KC_D,    28},  // V -> D
    {KC_B,    KC_V,    29},  // B -> V
    {KC_N,    KC_K,    30},  // N -> K
    {KC_M,    KC_H,    31},  // M -> H
    {KC_COMM, KC_COMM, 32},  // , stays ,
    {KC_DOT,  KC_DOT,  33},  // . stays .
    {KC_SLSH, KC_SLSH, 34},  // / stays /

    // Number keys for combos (same across layouts)
    {KC_4,    KC_4,    36},
    {KC_5,    KC_5,    37},
};

#define NUM_COMBO_MAPPINGS (sizeof(combo_key_mappings) / sizeof(combo_key_mapping_t))

static const combo_definition_t combo_definitions[] = {
    // Parentheses
    {15, 16, KC_9,    MOD_BIT(KC_LSFT), "Left parenthesis ("},   // D,F positions -> S,T in Colemak
    {19, 20, KC_0,    MOD_BIT(KC_LSFT), "Right parenthesis )"},  // J,K positions -> N,E in Colemak

    // Brackets
    {3,  4,  KC_LBRC, 0,                "Left bracket ["},       // E,R positions -> F,P in Colemak
    {7,  8,  KC_RBRC, 0,                "Right bracket ]"},      // U,I positions -> L,U in Colemak

    // Braces
    {27, 28, KC_LBRC, MOD_BIT(KC_LSFT), "Left brace {"},         // C,V positions -> C,D in Colemak
    {31, 32, KC_RBRC, MOD_BIT(KC_LSFT), "Right brace }"},        // M,COMM positions -> H,COMM in Colemak

    // Comparison operators
    {2,  3,  KC_COMM, MOD_BIT(KC_LSFT), "Less than <"},          // W,E positions -> W,F in Colemak
    {8,  9,  KC_DOT,  MOD_BIT(KC_LSFT), "Greater than >"},       // I,O positions -> U,Y in Colemak

    // Special characters
    {13, 14, KC_GRV,  MOD_BIT(KC_LSFT), "Tilde ~"},              // A,S positions -> A,R in Colemak
    {14, 15, KC_GRV,  0,                "Grave `"},               // S,D positions -> R,S in Colemak

    // Math operators
    {4,  7,  KC_EQL,  MOD_BIT(KC_LSFT), "Plus +"},               // R,U positions -> P,L in Colemak
    {28, 31, KC_MINS, 0,                "Minus -"},               // V,M positions -> D,H in Colemak
    {16, 19, KC_EQL,  0,                "Equal ="},               // F,J positions -> T,N in Colemak
    {3,  8,  KC_8,    MOD_BIT(KC_LSFT), "Asterisk *"},           // E,I positions -> F,U in Colemak
    {29, 30, KC_MINS, MOD_BIT(KC_LSFT), "Underscore _"},         // B,N positions -> V,K in Colemak

    // Special functions
    {36, 37, KC_BSPC, MOD_BIT(KC_LALT), "Word delete (Alt+Backspace)"}, // 4,5 on number layer
};

#define NUM_COMBO_DEFINITIONS (sizeof(combo_definitions) / sizeof(combo_definition_t))

// Function to get key position based on current layout and keycode
int8_t get_key_position_for_layout(uint16_t keycode, bool is_colemak) {
    for (int i = 0; i < NUM_COMBO_MAPPINGS; i++) {
        uint16_t target_keycode = is_colemak ? combo_key_mappings[i].colemak_keycode : combo_key_mappings[i].qwerty_keycode;
        if (keycode == target_keycode) {
            return combo_key_mappings[i].position;
        }
    }
    return -1; // Key not found in combo mappings
}

// Function to check and execute combos
bool check_and_execute_combo(int8_t key_pos, bool* combo_keys) {
    for (int i = 0; i < NUM_COMBO_DEFINITIONS; i++) {
        const combo_definition_t* combo = &combo_definitions[i];

        // Check if both positions for this combo are pressed
        if ((key_pos == combo->pos1 || key_pos == combo->pos2) &&
            combo_keys[combo->pos1] && combo_keys[combo->pos2]) {

            // Execute the combo
            if (combo->modifier != 0) {
                register_mods(combo->modifier);
            }
            register_code(combo->output_keycode);
            unregister_code(combo->output_keycode);
            if (combo->modifier != 0) {
                unregister_mods(combo->modifier);
            }

            // Clear the combo keys
            combo_keys[combo->pos1] = false;
            combo_keys[combo->pos2] = false;

            return true; // Combo was triggered
        }
    }
    return false; // No combo triggered
}
