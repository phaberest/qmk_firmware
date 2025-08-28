#pragma once

#define MASTER_RIGHT

// Configure the global tapping term (default: 200ms)
#define TAPPING_TERM 180

// Enable rapid switch from tap to hold, disables double tap hold auto-repeat.
#define TAPPING_FORCE_HOLD

// Apply the modifier on keys that are tapped during a short hold of a modtap
#define PERMISSIVE_HOLD

#define OLED_TIMEOUT 5000

#define SPLIT_MODS_ENABLE
#define SPLIT_LAYER_STATE_ENABLE
#define OLED_DRIVER_ENABLE

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
