#include QMK_KEYBOARD_H

uint16_t copy_paste_timer;

enum layers {
    QWERTY = 0,
    LOWER,
    RAISE,
    NAV,
    ADJUST
};

enum custom_keycodes {
    KC_CCCV = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * | ESC    |   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | LSFT   |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LCTL   |   Z  |   X  |   C  |   V  |   B  | CCCV |      |  | Del  |      |   N  |   M  | ,  < | . >  | /  ? |  - _   |
 * `----------------------+------+------+------+------+      |  |------+------+------+------+------+----------------------'
 *                        | GUI  | Alt  | Bspc | Space| Enter|  | Bspc | Space|      | Tab  | AltGr|
 *                        |      |      | Lower| Shift| Alt  |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [QWERTY] = LAYOUT(
      KC_ESC,  KC_Q,   KC_W,   KC_E,   KC_R,   KC_T,                                         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_BSLS,
      KC_LSFT, KC_A,   KC_S,   KC_D,   KC_F,   KC_G,                                         KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LCTL, KC_Z,   KC_X,   KC_C,   KC_V,   KC_B,   KC_CCCV,   XXXXXXX, KC_DEL, XXXXXXX,  KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
              KC_LGUI, KC_LALT, LT(LOWER, KC_BSPC), MT(MOD_LSFT, KC_SPC), MT(MOD_LALT, KC_ENT), KC_BSPC, LT(NAV, KC_SPC), MO(RAISE), KC_TAB, KC_RALT
    ),
/*
 * Lower Layer: Numpad, Media
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      | VolUp|      |      |                              | / ?  | 7 &  | 8 *  | 9 (  | - _  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next |      |                              | *    | 4 $  | 5 %  | 6 ^  | , <  | +      |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      | VolDn| Mute |      |      |      |  |      |      | 0 )  | 1 !  | 2 @  | 3 #  | = +  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | Lower|      |      |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [LOWER] = LAYOUT(
      _______, _______, _______, KC_VOLU, _______, _______,                                     KC_SLSH, KC_7,    KC_8,    KC_9, KC_MINS, _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______,                                     KC_ASTR, KC_4,    KC_5,    KC_6, KC_COMM, KC_PLUS,
      _______, _______, _______, KC_VOLD, KC_MUTE, _______, _______, _______, _______, _______, KC_0,    KC_1,    KC_2,    KC_3, KC_EQL,  _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Raise Layer: Symbols
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |  !   |  @   |  {   |  }   |  |   |                              |      |  _   |  €   |      |      |  \     |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |  #   |  $   |  (   |  )   |  `   |                              |   +  |  -   |  /   |  *   |  %   |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |  %   |  ^   |  [   |  ]   |  ~   |      |      |  |      |      |   &  |  =   |  ,   |  .   |  / ? | - _    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | Lower|      |      |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [RAISE] = LAYOUT(
      _______, KC_EXLM, KC_AT,   KC_LCBR, KC_RCBR, KC_PIPE,                                     _______, KC_UNDS, ALGR(KC_5),_______,_______,KC_BSLS,
      _______, KC_HASH, KC_DLR,  KC_LPRN, KC_RPRN, KC_GRV,                                      KC_PLUS, KC_MINS, KC_SLSH, KC_ASTR, KC_PERC, KC_QUOT,
      _______, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD, _______, _______, _______, _______, KC_AMPR, KC_EQL,  KC_COMM, KC_DOT,  KC_SLSH, KC_MINS,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Navigation Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              | PgUp | Home | Up   | End  |      | ScrlLk |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              | PgDn | Left | Down | Right|      | CapsLk |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      | Lower|      |      |  |      | Nav  | Raise|      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [NAV] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     KC_PGUP, KC_HOME, KC_UP,   KC_END,  _______, KC_SCRL,
      _______, _______, _______, _______, _______, _______,                                     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, _______, KC_CAPS,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Adjust Layer
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      | F7   | F8   | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | SAI  | HUI  | VAI  |      |                              |      | F4   | F5   | F6   | F11  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      | SAD  | HUD  | VAD  |      |      |      |  |      |      |      | F1   | F2   | F3   | F12  |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [ADJUST] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      _______, _______, RGB_SAI, RGB_HUI, RGB_VAI, _______,                                     _______, KC_F4,   KC_F5,   KC_F6,   KC_F11,  _______,
      _______, _______, RGB_SAD, RGB_HUD, RGB_VAD, _______, _______, _______, _______, _______, _______, KC_F1,   KC_F2,   KC_F3,   KC_F12,  _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, LOWER, RAISE, ADJUST);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case KC_CCCV:  // One key copy/paste
            if (record->event.pressed) {
                copy_paste_timer = timer_read();
            } else {
                if (timer_elapsed(copy_paste_timer) > TAPPING_TERM) {  // Hold, copy
                    tap_code16(LCTL(KC_C));
                } else { // Tap, paste
                    tap_code16(LCTL(KC_V));
                }
            }
            break;
    }
    return true;
}

// bool is_alt_tab_active = false;
// uint16_t alt_tab_timer = 0;

// void matrix_scan_user(void) {
//     if (is_alt_tab_active) {
//         if (timer_elapsed(alt_tab_timer) > 1000) {
//             unregister_code(KC_LALT);
//             is_alt_tab_active = false;
//         }
//     }
// }

// #if false
#ifdef OLED_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

void print_key(bool reverse, uint8_t x, uint8_t y, uint8_t cursorY, uint8_t cursorX);

bool oled_task_user(void) {
    void line(int a, int row, bool reverse) {
        const uint8_t left = 30;
        const uint8_t space = 7;
        print_key(reverse, a, 2, row, left + 0);
        print_key(reverse, a, 3, row, left + 12 + space);
        print_key(reverse, a, 4, row, left + 24 + space);
        print_key(reverse, a, 5, row, left + 36 + space);
        print_key(reverse, a, 6, row, left + 48 + space);
        print_key(reverse, a, 7, row, left + 60 + space + space);
    }

    if (is_keyboard_left()) {
        line(0, 0, true);
        line(1, 1, true);
        line(2, 2, true);
    } else {
        line(4, 0, false);
        line(5, 1, false);
        line(6, 2, false);
    }
    return false;
}

char keyCodeToChar(uint16_t keyCode);

void print_key(bool reverse, uint8_t x, uint8_t y, uint8_t cursorY, uint8_t cursorX) {
    uint16_t keyCode = keycode_at_keymap_location(get_highest_layer(layer_state), x, y);
    char keyChar = keyCodeToChar(keyCode);
    oled_set_cursor_raw(reverse ? 128 - 12 - cursorX : cursorX, cursorY * 2);
    bool down = matrix_is_on(x, y);
    oled_write_char(keyChar, down);
}

char keyCodeToChar(uint16_t keyCode) {
    bool shift = get_mods() & MOD_MASK_SHIFT;
    switch (keyCode) {
        case KC_A ... KC_Z: return 'A' + (keyCode - KC_A);
        case KC_0: return '0';
        case KC_1 ... KC_9: return '1' + (keyCode - KC_1);
        case KC_F1 ... KC_F9: return '1' + (keyCode - KC_F1);
        case KC_F10: return '0';
        case KC_F11: return '1';
        case KC_F12: return '2';
        case KC_ENTER: return 'e';
        case KC_ESCAPE: return 'e';
        case KC_TAB: return 't';
        case KC_SPACE: return 's';
        case KC_MINUS: return shift ? '_' : '-';
        case KC_EQUAL: return shift ? '+' : '=';
        case KC_LBRC: return shift ? '{' : '[';
        case KC_RBRC: return shift ? '}' : ']';
        case KC_BACKSLASH: return shift ? '|' : '\\';
        case KC_NONUS_HASH: return shift ? '~' : '#';
        case KC_COLON: return ':';
        case KC_SEMICOLON: return shift ? ':' : ';';
        case KC_QUOTE: return shift ? '"' : '\'';
        case KC_GRAVE: return shift ? '~' : '`';
        case KC_COMMA: return shift ? '<' : ',';
        case KC_DOT: return shift ? '>' : '.';
        case KC_SLASH: return shift ? '?' : '/';
        case KC_NONUS_BACKSLASH: return shift ? '|' : '\\';
        case KC_CAPS_LOCK: return 'c';
        case KC_SCROLL_LOCK: return 's';
        case KC_EXLM: return '!';
        case KC_AT: return '@';
        case KC_HASH: return '#';
        case KC_DLR: return '$';
        case KC_PERC: return '%';
        case KC_CIRC: return '^';
        case KC_AMPR: return '&';
        case KC_ASTR: return '*';
        case KC_LPRN: return '(';
        case KC_RPRN: return ')';
        case KC_UNDS: return '_';
        case KC_PLUS: return '+';
        case KC_LCBR: return '{';
        case KC_RCBR: return '}';
        case KC_PIPE: return '|';
        case KC_TILD: return '~';
        case KC_DEL: return 'd';
        case KC_BSPC: return 'b';
        case KC_INS: return 'i';
        case KC_HOME: return 'h';
        case KC_PGUP: return 0x1E;
        case KC_PGDN: return 0x1F;
        case KC_END: return 'e';
        case KC_PSCR: return 'p';
        case KC_PAUS: return 'p';
        case KC_UP: return 0x18;
        case KC_DOWN: return 0x19;
        case KC_LEFT: return 0x1B;
        case KC_RGHT: return 0x1A;
        case KC_PSLS: return '/';
        case KC_LEFT_SHIFT: return 0x1E;
        case KC_RIGHT_SHIFT: return 0x1E;
        case KC_LEFT_CTRL: return 'c';
        case KC_RIGHT_CTRL: return 'c';
        case KC_LEFT_ALT: return 'a';
        case KC_RIGHT_ALT: return 'a';
        case KC_LEFT_GUI: return 'g';
        case KC_RIGHT_GUI: return 'g';
        case RGB_SAI: return 's';
        case RGB_SAD: return 's';
        case RGB_HUI: return 'h';
        case RGB_HUD: return 'h';
        case RGB_VAI: return 'v';
        case RGB_VAD: return 'v';
    }
    return ' ';
}

#endif

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) {
        switch (get_highest_layer(layer_state)) {
            case QWERTY     // History scrubbing. For Adobe products, hold shift while moving
                // backward to go forward instead.
                if (clockwise) {
                    tap_code16(C(KC_Z));
                } else {
                    tap_code16(C(KC_Y));
                }
                break;
            default     // Switch between windows on Windows with alt tab.
                if (clockwise) {
                    if (!is_alt_tab_active) {
                        is_alt_tab_active = true;
                        register_code(KC_LALT);
                    }
                    alt_tab_timer = timer_read();
                    tap_code16(KC_TAB);
                } else {
                    tap_code16(S(KC_TAB));
                }
                break;
        }
    } else if (index == 1) {
        switch (get_highest_layer(layer_state)) {
            case QWERTY     // Scrolling with PageUp and PgDn.
                if (clockwise) {
                    tap_code(KC_PGDN);
                } else {
                    tap_code(KC_PGUP);
                }
                break;
            default     // Volume control.
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
                break;
        }
    }
    return true;
}
#endif
