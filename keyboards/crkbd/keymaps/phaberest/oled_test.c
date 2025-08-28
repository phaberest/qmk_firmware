#include QMK_KEYBOARD_H
#include <stdbool.h>
#include <stdint.h>
#include "quantum.h"

// Character mapping test to find where special characters are located
bool oled_task_user(void) {
    oled_clear();

    // Test basic ASCII first
    oled_write_P(PSTR("ASCII: ABC123\n"), false);

    // Test the range where modifier graphics should be (0x85-0x90)
    oled_write_P(PSTR("Test 0x85-90:\n"), false);
    char test_str[3] = {0x85, 0x86, 0};
    oled_write(test_str, false);
    oled_write_P(PSTR(" "), false);

    test_str[0] = 0x87; test_str[1] = 0x88;
    oled_write(test_str, false);
    oled_write_P(PSTR(" "), false);

    test_str[0] = 0x89; test_str[1] = 0x8A;
    oled_write(test_str, false);
    oled_write_P(PSTR("\n"), false);

    // Test higher range
    oled_write_P(PSTR("Test 0xA5-B0:\n"), false);
    test_str[0] = 0xA5; test_str[1] = 0xA6;
    oled_write(test_str, false);
    oled_write_P(PSTR(" "), false);

    test_str[0] = 0xAF; test_str[1] = 0xB0;
    oled_write(test_str, false);

    return false;
}
