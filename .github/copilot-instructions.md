# QMK Firmware Development Guidelines

## Architecture Overview

QMK is a modular keyboard firmware built around several key directories:
- `quantum/` - Core QMK functionality, keycodes, and features
- `keyboards/` - Keyboard-specific configurations and layouts 
- `tmk_core/` - Low-level hardware abstraction layer
- `platforms/` - MCU platform implementations (AVR, ARM/ChibiOS, RP2040)
- `drivers/` - Hardware drivers (I2C, SPI, audio, RGB, etc.)
- `users/` - Shared userspace code across keyboards
- `builddefs/` - Build system makefiles and rules

## Build System Patterns

QMK uses a sophisticated make-based build system with data-driven configuration:

**This Repository's Build Process:**
```bash
# Always use the provided build script for this repository
./build.sh                             # Builds crkbd/rev1:phaberest with Docker

# The build script runs:
# SKIP_FLASHING_SUPPORT=1 util/docker_build.sh crkbd/rev1:phaberest \
#   -e USER_NAME=phaberest \
#   -e POINTING_DEVICE=trackpoint \
#   -e POINTING_DEVICE_POSITION=right \
#   -e OLED=yes \
#   -j8
```

**Standard QMK Build Commands (for reference):**
```bash
# Build firmware 
make keyboard:keymap                    # Basic build
qmk compile -kb keyboard -km keymap     # CLI equivalent
qmk compile -j 0 -kb keyboard          # Parallel build

# Development builds
util/docker_build.sh keyboard:keymap    # Docker build
make keyboard:keymap:flash             # Build and flash
```

**Configuration Hierarchy (highest to lowest priority):**
1. `keyboards/*/keymaps/*/rules.mk` - Keymap-specific build rules
2. `keyboards/*/keymaps/*/config.h` - Keymap-specific configuration  
3. `keyboards/*/rules.mk` - Keyboard-specific rules
4. `keyboards/*/config.h` - Keyboard configuration
5. `keyboards/*/info.json` - Data-driven keyboard definition
6. `quantum/` defaults

## Keymap Development Patterns

**File Structure:**
```
keyboards/keyboard/keymaps/keymap_name/
├── keymap.c        # Main keymap logic
├── config.h        # Configuration overrides
├── rules.mk        # Build feature enables
└── keymap.json     # Alternative JSON format
```

**Essential QMK Includes:**
```c
#include QMK_KEYBOARD_H      // Always first, keyboard-specific headers
#include "quantum.h"         // Core QMK functionality (if needed)
```

**Layer Definitions:**
```c
enum layers {
    _BASE = 0,               // Default layer (0)
    _LOWER,
    _RAISE,
    _ADJUST
};

// Use LAYOUT_split_3x6_3 for CRKBD specifically
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(/* layout */),
};
```

**Custom Keycodes:**
```c
enum custom_keycodes {
    CUSTOM_KEY = SAFE_RANGE,    // Always start from SAFE_RANGE
    ANOTHER_KEY,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // Handle custom keycodes
    return true; // Continue normal processing
}
```

## Feature Integration Patterns

**Split Keyboard Configuration (`config.h`):**
```c
#define MASTER_LEFT             // or MASTER_RIGHT
#define SPLIT_MODS_ENABLE       // Sync modifiers across halves
#define SPLIT_LAYER_STATE_ENABLE // Sync layer state
```

**Common Feature Enables (`rules.mk`):**
```makefile
POINTING_DEVICE_ENABLE = yes
OLED_ENABLE = yes
TAP_DANCE_ENABLE = yes
COMBO_ENABLE = yes
WPM_ENABLE = yes
```

**Userspace Integration:**
- This repository uses the `phaberest` userspace: `users/phaberest/`
- Include with `USER = phaberest` in `rules.mk` (already configured for this keymap)
- Reference with `#include "phaberest.h"` (or specific userspace headers)
- Shared code across keyboards is in `users/phaberest/`

## Data-Driven Configuration

QMK increasingly uses `info.json` for keyboard definitions:
```json
{
    "manufacturer": "Company",
    "keyboard_name": "board",
    "maintainer": "username",
    "features": {
        "bootmagic": true,
        "extrakey": true,
        "oled": true
    },
    "layouts": {
        "LAYOUT_name": {
            "layout": [/* key positions */]
        }
    }
}
```

## Development Workflow

**Local Development:**
1. Make changes to keymap files in `keyboards/crkbd/keymaps/phaberest/`
2. Build with the project's build script: `./build.sh`
3. Flash the resulting `.uf2` file to your keyboard
4. Use `qmk console` for debugging output (requires `CONSOLE_ENABLE = yes`)

**This Repository Specifics:**
- Always use `./build.sh` for building firmware
- The build outputs to `crkbd_rev1_phaberest.uf2`
- Configuration is pre-set for CRKBD rev1 with trackpoint support
- Uses `phaberest` userspace with holykeebs integration

**Code Quality:**
- Follow [QMK C conventions](https://docs.qmk.fm/#/coding_conventions_c)
- Use `qmk format-json` for JSON files
- Test with `qmk compile -kb all -km keymap` for compatibility

**Advanced Features:**
- **Tap Dance:** Multi-function keys based on tap count
- **Combos:** Key combinations that trigger different actions
- **Macros:** Custom key sequences using `SEND_STRING()`
- **Layer Tap:** `LT(layer, keycode)` for layer switching
- **Mod Tap:** `MT(modifier, keycode)` for modifier/key combos

## Hardware-Specific Patterns

**OLED Integration:**
```c
bool oled_task_user(void) {
    // Custom OLED content
    return false; // Don't continue to default OLED task
}
```

**Pointing Device (Trackball/Trackpoint):**
```c
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    // Process mouse movement
    return mouse_report;
}
```

**RGB Matrix:**
```c
bool rgb_matrix_indicators_user(void) {
    // Custom RGB indicators
    return false;
}
```

## Debugging Techniques

- Enable `CONSOLE_ENABLE = yes` and use `uprintf()` for debug output
- Use `qmk console` to view debug messages
- Check `qmk doctor` for environment issues
- Validate JSON with `qmk lint` before compilation
- Use `make clean` when changing major build options
