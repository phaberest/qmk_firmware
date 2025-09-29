#pragma once

#define MASTER_RIGHT

// Configure the global tapping term (default: 200ms) - increased for better reliability
#define TAPPING_TERM 250

// Require a short idle period before allowing hold-tap to activate
#define QUICK_TAP_TERM 120  // Prevent accidental holds during fast typing

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
// #define TAPPING_FORCE_HOLD  // Disabled - can make typing feel less responsive

// Apply the modifier on keys that are tapped during a short hold of a modtap
// #define PERMISSIVE_HOLD  // Disabled - can interfere with fast typing

#define OLED_TIMEOUT 5000

#define SPLIT_MODS_ENABLE
#define SPLIT_LAYER_STATE_ENABLE
#define OLED_DRIVER_ENABLE

// Split communication optimizations - DEBUGGING MODE
#define SPLIT_MAX_CONNECTION_ERRORS 20  // Increase tolerance for debugging
#define SPLIT_CONNECTION_CHECK_TIMEOUT 1000  // Increase timeout for debugging
// #define SPLIT_TRANSPORT_MIRROR  // Disable mirroring for debugging

// Auto mouse layer configuration
#define AUTO_MOUSE_DEFAULT_LAYER 6
#define AUTO_MOUSE_TIME 1000
#define AUTO_MOUSE_DELAY 100
#define AUTO_MOUSE_THRESHOLD 1

// Combo configuration - Re-enabled with conservative settings for split debugging
#define COMBO_TERM 300  // Increased timeout to reduce accidental combo triggers during typing
#define COMBO_SHOULD_TRIGGER  // Enable combo conditional triggering
#define COMBO_MUST_HOLD_MODS  // Require modifiers to be held during combo detection
#define COMBO_MUST_TAP_PER_COMBO  // Require all keys to be tapped within the combo term

// Additional optimizations for typing responsiveness
#define DEBOUNCE 5  // Default is 5ms, ensure it's not higher
#define USB_POLLING_INTERVAL_MS 1  // 1000Hz USB polling for responsiveness

// EEPROM configuration for persistent layer storage
// EECONFIG_USER_DATA_SIZE is defined in users/phaberest/config.h (64 bytes)
#define EECONFIG_USER_DATA_VERSION 1  // Version for user data format

// Split pointing device optimizations - TEMPORARILY DISABLED FOR DEBUGGING
// #define POINTING_DEVICE_TASK_THROTTLE_MS 8  // Throttle pointing device updates to reduce comm load
// #define SPLIT_POINTING_ENABLE  // Ensure split pointing is enabled
// #define POINTING_DEVICE_COMBINED  // Enable combined pointing device mode

#define HK_MAIN_DEFAULT_POINTER_DEFAULT_MULTIPLIER 3.5
#define HK_PERIPHERAL_DEFAULT_POINTER_DEFAULT_MULTIPLIER 1

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X32
  // #define OLED_DISPLAY_128X64 // Uncomment this if you have a 128x64 display
  //#define OLED_TIMEOUT 30000  // Optional: turns off the OLED after 30 seconds of inactivity. Set to 0 to disable.
  #define OLED_BRIGHTNESS 120 // Optional: reduces brightness; default is 255
  #define OLED_SCROLL_TIMEOUT 0 // Disable OLED scrolling
  #define OLED_UPDATE_INTERVAL 66 // Set refresh rate (default is 0)

  // Font configuration
  #define OLED_FONT_WIDTH 6
  #define OLED_FONT_HEIGHT 8
  #define OLED_FONT_START 0
  #define OLED_FONT_END 223  // 1344 bytes ÷ 6 = 224 characters (0-223)

  // Use custom font file
  #define OLED_FONT_H "keyboards/crkbd/keymaps/phaberest/glcdfont.c"
#endif
