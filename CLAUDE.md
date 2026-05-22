# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

ErgoDox keyboard firmware for Teensy 2.0 (ATmega32U4). All source lives flat in `src/`.

## Build

```sh
cd src
make          # build firmware.hex + firmware.eep
make load     # flash via teensy_loader_cli (Teensy must be in bootloader mode)
make clean    # git clean -dX -f  (removes ignored files only)
```

Toolchain required: `avr-gcc`, `avr-objcopy`, `avr-size`, `teensy_loader_cli`.

## Hardware split

| Half  | Controller       | Matrix columns | Interface        |
|-------|-----------------|----------------|------------------|
| Left  | MCP23018 expander | cols 0–6      | I²C via `twi.c`  |
| Right | Teensy 2.0 GPIO  | cols 7–D      | Direct port I/O  |

Matrix is hardcoded 6×14. Compile-time assertions in `teensy.c` and `mcp23018.c` will fire if `KB_ROWS`/`KB_COLUMNS` drift from reality.

## PROGMEM pattern

Layout arrays live in flash to save the 2.5 KB RAM. Never read them directly — always use the accessor macros:

```c
// read keycode
kb_layout_get(layer, row, col)          // pgm_read_byte

// read function pointer
kb_layout_press_get(layer, row, col)    // pgm_read_word
kb_layout_release_get(layer, row, col)
```

Declarations and `#include <avr/pgmspace.h>` are all in `layout.h`.

## Compile-time constants

Adjust in `src/Makefile` — they're passed as `-D` flags:

| Constant               | Default | Meaning                        |
|------------------------|---------|--------------------------------|
| `MAKEFILE_LED_BRIGHTNESS` | `0.5` | PWM duty cycle for LEDs (0–1) |
| `MAKEFILE_DEBOUNCE_TIME`  | `20`  | Scan debounce in milliseconds  |

## Code style

Match what's already there: minimal, no unnecessary abstractions, no comments unless the *why* is non-obvious. Commit messages are terse lowercase imperative (`add arrow keys`, `fix capslock`).
