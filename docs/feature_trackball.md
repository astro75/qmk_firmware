# Trackball

## Setup
To enable support for the Pimoroni trackball, place the following lines in your `rules.mk` file:
```
TRACKBALL_ENABLE = yes
POINTING_DEVICE_ENABLE = yes
```
*If you do not wish to use your trackball as a mouse, leaving out the second line will save you some bytes.*

In addition to enabling the support itself, you probably want to set some configuration values in your `config.h` file.

### Orientation
The orientation of the trackball varies from keyboard to keyboard due to physical requirements.
The driver needs to be made aware of this by setting `TRACKBALL_ORIENTATION`. To determine your orientation, look at the orientation of the "pimoroni.com" text on the trackball:

<!-- Image sourced from https://shop.pimoroni.com/products/trackball-breakout -->
|Text direction|`TRACKBALL_ORIENTATION`|Example|
|---------|-----------------------|-------|
|Up       |0                      |<div style="transform: rotate(0deg);">![](feature_trackball.png)</div>|
|Right    |1                      |<div style="transform: rotate(90deg);">![](feature_trackball.png)</div>|
|Down     |2                      |<div style="transform: rotate(180deg);">![](feature_trackball.png)</div>|
|Left     |3                      |<div style="transform: rotate(270deg);">![](feature_trackball.png)</div>|

### Button behaviour
By default, pressing the trackball will emulate a left mouse click.
If you wish to change this to a different mouse button, you can set
`TRACKBALL_MOUSE_BTN` to a different value.
For example, setting it to `MOUSE_BTN2` will result in a right click.

You can also use the trackball button to take over the role of a key in the key matrix
by setting `TRACKBALL_MATRIX_ROW` and `TRACKBALL_MATRIX_COL` to the position it replaces.

?> For a split keyboard, the right half is positioned **below** the left: a three-row, four-column split has a six-row, four-column matrix.

### LED
The trackball features an integrated RGB LED.

The easiest way to use it is by copying the color of an underglow LED.
To configure which LED to mirror, set `TRACKBALL_RGBLIGHT` to the index of the LED you want to mirror.

## Custom code
### Input
The behaviour of the trackball on an input event can be changed by implementing
`void process_trackball_user(trackball_record_t *record)` and `void process_trackball_kb(trackball_record_t *record)`.

`record` contains the `x` and `y` movement, and `pressed` to represent the button state.
It also contains `type`, which is a bit field of `TB_MOVED` and `TB_BUTTON`.
When set, the bits indicate that an event has occured and should be handled.

If your code has handled an event, you can prevent the default behaviour from
executing by clearing the corresponding bit.

#### Intercept movement
This example uses the trackball movement to control underglow color.
Note the last line, preventing the default behavour.
```c
process_trackball_user(trackball_record_t *record) {
    if (record->type & TB_MOVED) {
        for (int i = record->x; i < 0; i++) {
            rgblight_increase_hue();
        }
        for (int i = record->x; i > 0; i--) {
            rgblight_decrease_hue();
        }
        for (int i = record->y; i < 0; i++) {
            rgblight_increase_sat();
        }
        for (int i = record->y; i > 0; i--) {
            rgblight_decrease_sat();
        }
        record->type &= ~TB_MOVED;
    }
}
```

#### Custom acceleration
This example demonstrates how to change a movement while keeping the default behavour.
When the `M_FAST` key is held, the mouse will move faster.
```c
enum custom_keycodes {
    M_FAST = SAFE_RANGE,
};

bool go_fast;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case M_FAST:
        go_fast = record->event.pressed;
        return false;
      default:
        break;
    }
    return true;
}

process_trackball_user(trackball_record_t *record) {
    if (record->type & TB_MOVED) {
        if (go_fast) {
            record->type.x *= 20;
            record->type.y *= 20;
        }
    }
}
```

### LED
You can use one of the following functions to control the integrated LED:
- `i2c_status_t trackball_setrgb(uint8_t r, uint8_t g, uint8_t b);`
- `i2c_status_t trackball_sethsv(uint8_t h, uint8_t s, uint8_t v);`

## Configuration
?> All configuration options can be set in your `config.h`.

|Configuration option|Default| |
|:--------------------|-------|-|
|`TRACKBALL_ORIENTATION`|`0`|Physical orientation of the trackball, `0`-`3`.|
|`TRACKBALL_MOUSE_BTN`|`MOUSE_BTN1`|Mouse click emulated when trackball is pressed|
|`TRACKBALL_MATRIX_ROW` & `TRACKBALL_MATRIX_COL`|*Not set*|Key matrix position replaced by trackball|
|`TRACKBALL_RGBLIGHT`|*Not set*|Index of [Underglow LED](feature_rgblight.md) mirrored by trackball|

### Advanced
!> You should not have to change those. The defaults will work best for most people.

|Configuration option|Default| |
|:--------------------|-------|-|
|`TRACKBALL_ADDRESS`|`0x0A`|Alternate address: `0x0B`.|
|`TRACKBALL_TIMEOUT`|`100` (ms)|I2C connection timeout. Too high causes lag when no trackball is connected, too low prevents it from working.|
|`TRACKBALL_INTERVAL`|`35` (ms)|Minimum time between data reads.|
