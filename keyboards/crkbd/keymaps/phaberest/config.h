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

#define OLED_TASK_USER_OVERRIDE

#ifdef OLED_DRIVER_ENABLE
  #define OLED_DISPLAY_128X32
  // #define OLED_DISPLAY_128X64 // Uncomment this if you have a 128x64 display
  //#define OLED_TIMEOUT 30000  // Optional: turns off the OLED after 30 seconds of inactivity. Set to 0 to disable.
  #define OLED_BRIGHTNESS 120 // Optional: reduces brightness; default is 255
#endif
