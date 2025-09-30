# Phaberest's Corne Keyboard Configuration

This is my custom QMK firmware configuration for the Corne (CRKBD) keyboard with integrated trackpoint. This configuration features dual-layout support (QWERTY/Colemak-DH), extensive combos, OLED displays, and a unique bitwise function key input system.

![Keymap Layout](/keyboards/crkbd/keymaps/phaberest/keymap.svg)

📖 **[View complete keymap documentation →](/keyboards/crkbd/keymaps/phaberest/keymaps_corne.pdf)**

## ✨ Features

### 🎯 Dual Layout Support
- **QWERTY** (Layer 0) - Traditional layout
- **Colemak-DH** (Layer 1) - Optimized for finger rolling and hand alternation
- Persistent layout switching with EEPROM storage
- Layout-aware combo system

### ⚡ Advanced Input Methods
- **Comprehensive Combos** for symbols, brackets, and special characters - mapped to equivalent key positions on both layouts
- **Bitwise Function Keys** - Use left home row to input F1-F24 via binary combinations
- **Gaming Layer** with chat mode for seamless gaming/typing transitions
- **Mouse Layer** with auto-activation via trackpoint movement

### 🖥️ Hardware Integration
- **OLED Displays** showing layer status, WPM, and custom animations
- **Trackpoint** integration with auto mouse layer
- **Split keyboard** optimizations for reliable communication

## 🔧 Building

```bash
# Build the firmware using the included script
./build.sh

# This runs:
# SKIP_FLASHING_SUPPORT=1 util/docker_build.sh crkbd/rev1:phaberest \
#   -e USER_NAME=phaberest \
#   -e POINTING_DEVICE=trackpoint \
#   -e POINTING_DEVICE_POSITION=right \
#   -e OLED=yes \
#   -j8
```

The build outputs `crkbd_rev1_phaberest.uf2` ready for flashing.

## 🎹 Layer Overview

### Layer 0: QWERTY Base
```
┌─────┬─────┬─────┬─────┬─────┬─────┐     ┌─────┬─────┬─────┬─────┬─────┬─────┐
│ GRV │  Q  │  W  │  E  │  R  │  T  │     │  Y  │  U  │  I  │  O  │  P  │BSPC │
├─────┼─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┼─────┤
│ESC/6│  A  │  S  │  D  │  F  │  G  │     │  H  │  J  │  K  │  L  │ ; : │ ' " │
├─────┼─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┼─────┤
│LSFT │  Z  │  X  │  C  │  V  │  B  │     │  N  │  M  │ , < │ . > │ / ? │RSFT │
└─────┴─────┴─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │LALT │TAB/2│SPC/3│     │ENT/⇧│LGUI │RCTL │
                  └─────┴─────┴─────┘     └─────┴─────┴─────┘
```

### Layer 1: Colemak-DH Base  
```
┌─────┬─────┬─────┬─────┬─────┬─────┐     ┌─────┬─────┬─────┬─────┬─────┬─────┐
│ GRV │  Q  │  W  │  F  │  P  │  B  │     │  J  │  L  │  U  │  Y  │ ; : │BSPC │
├─────┼─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┼─────┤
│ESC/6│  A  │  R  │  S  │  T  │  G  │     │  M  │  N  │  E  │  I  │  O  │ ' " │
├─────┼─────┼─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┼─────┼─────┤
│LSFT │  Z  │  X  │  C  │  D  │  V  │     │  K  │  H  │ , < │ . > │ / ? │RSFT │
└─────┴─────┴─────┼─────┼─────┼─────┤     ├─────┼─────┼─────┼─────┴─────┴─────┘
                  │LALT │TAB/2│SPC/3│     │ENT/⇧│LGUI │RCTL │
                  └─────┴─────┴─────┘     └─────┴─────┴─────┘
```

### Layer 2: Navigation/Media
- Arrow keys, page navigation, word navigation
- Media controls (play/pause, volume, brightness)
- Copy/paste shortcuts

### Layer 3: Numbers/Symbols
- Number pad on right side
- Symbol keys on left side
- Mathematical operators

### Layer 4: Function Keys (Bitwise Input)
- **Innovative bitwise input system**
- Left home row keys act as binary bits: A(1) + S(2) + D(4) + F(8) + G(16)
- Examples: A+D = 1+4 = 5 = F5, S+F = 2+8 = 10 = F10
- Access to all F1-F24 function keys

### Layer 5: Gaming *(Work in Progress)*
- Standard WASD layout
- Optimized for gaming with direct access to common keys
- Chat mode for quick typing during games

*Note: This layer is still being refined. First, I probably need to find time to actually game! 🎮*

### Layer 6: Mouse/Utilities
- Mouse movement and scroll wheel
- Mouse buttons
- Keyboard reset and utilities

## 🎯 Combo System

The keyboard features comprehensive combos that adapt to your current layout (QWERTY/Colemak-DH). Each combo is mapped to equivalent key positions on both layouts, so you learn one set of finger movements that work regardless of your active layout:

### Brackets & Parentheses
| Combo | QWERTY Keys | Colemak Keys | Output |
|-------|-------------|--------------|--------|
| `()` | D+F | S+T | ( ) |
| `[]` | E+R | F+P | [ ] |
| `{}` | C+V | C+D | { } |
| `<>` | W+E, I+O | W+F, U+Y | < > |

### Mathematical Operators
| Combo | QWERTY Keys | Colemak Keys | Output |
|-------|-------------|--------------|--------|
| Plus | R+U | P+L | + |
| Minus | V+M | D+H | - |
| Equal | F+J | T+N | = |
| Asterisk | E+I | F+U | * |
| Underscore | R+T | P+B | _ |

### Special Characters
| Combo | QWERTY Keys | Colemak Keys | Output |
|-------|-------------|--------------|--------|
| Tilde | A+S | A+R | ~ |
| Grave | S+D | R+S | ` |
| Exclamation | Y+U | J+L | ! |
| At symbol | H+J | M+N | @ |
| Hash | F+G | T+G | # |
| Dollar | S+L | R+I | $ |
| Percent | V+B | D+V | % |
| Caret | N+M | K+H | ^ |
| Ampersand | D+K | S+E | & |

### Emojis (macOS optimized)
| Combo | QWERTY Keys | Colemak Keys | Output |
|-------|-------------|--------------|--------|
| Hands | T+Y | B+J | 🙌🏼 |
| Laugh | G+H | G+M | 😂 |
| Heart | B+N | V+K | ❤️ |

*Note: Emoji combos use macOS emoji picker (Cmd+Ctrl+Space) and may need adjustment for other operating systems.*

### Utility Combos
| Combo | Keys | Action |
|-------|------|--------|
| Word Delete | 4+5 | Alt+Backspace |

## 🚀 Special Features

> **macOS Optimized**: Some features like emoji input and certain key combinations are specifically optimized for macOS. Adjustments may be needed for Windows or Linux.

### Layout Switching
- **Toggle Layout**: Access via Function layer to switch between QWERTY and Colemak-DH
- **Persistent Storage**: Your layout preference is saved to EEPROM
- **Smart Combos**: Combos automatically map to equivalent key positions on your active layout

### Bitwise Function Keys
The most unique feature - use binary combinations on the left home row:
- A = 1, S = 2, D = 4, F = 8, G = 16
- Hold combinations to generate F-keys: A+D = F5, S+F = F10, etc.
- Access all F1-F24 keys with simple finger combinations

*Idea brutally copied from [this excellent article](https://leanrada.com/notes/developing-bitwise-input-method/) - full credit to the original author!*

### Gaming Mode *(Work in Progress)*
- **Gaming Layer**: Optimized WASD layout
- **Chat Mode**: Seamlessly switch to typing mode during games
- **Exit Combos**: Easy return to normal layout

*This feature is still being refined - need to find time to actually game first! 🎮*

### Auto Mouse Layer
- Automatically activates when trackpoint movement is detected
- 1-second timeout returns to previous layer
- Full mouse control with buttons and scroll wheel

*Note: My trackpoint is currently fucked up, so I'm relying on my trackpad or manually triggering the mouse layer via the ESC/Layer 6 key on the left side. The auto-activation feature works great when the hardware cooperates! 🖱️*

### Caps Lock Toggle
- Press both shift keys simultaneously to toggle Caps Lock
- Visual feedback on OLED display

## 🔧 Configuration

### Hardware Setup
- **Master Side**: Right half (configured in config.h)
- **Trackpoint**: Right side, integrated with auto mouse layer (when it worked, now temporarily disabled)
- **OLED**: Both halves with custom animations and status indicators for layer and modifier keys

### Build Environment
- Uses Docker for consistent builds (especially since it wouldn't build on Apple Silicon)
- Pre-configured for CRKBD rev1

## 📁 File Structure

```
keyboards/crkbd/keymaps/phaberest/
├── keymap.c        # Main keymap logic and combos
├── config.h        # Hardware and feature configuration
├── rules.mk        # Build feature enables
├── oled.c          # OLED display customizations (mainly my Oh My Bugs logo)
└── glcdfont.c      # Custom font (contains mac keys icons)
```

---

*This configuration is continuously evolved for optimal typing efficiency and comfort. The combination of layout flexibility, extensive combos, and innovative input methods makes this a truly personalized typing experience.*
