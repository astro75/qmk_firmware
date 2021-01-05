/* Copyright 2019 Thomas Baart <thomas@splitkb.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#ifdef TRACKBALL_ENABLE
#    include "trackball.h"
#endif

#include "print.h"

enum layers {
    _BASE = 0,
    _SYMB,
    _ACCS,
    _NAVI,
    _MOUS,
    _CONF,
};

// U: User key, dynamic behaviour
// M: Config mode
// C: Composed keys
//    Diacritic suffixes:
//      A  Acute (')
//      G  Grave (`)
//      T  Trema (")
//    Circumflex (^) is missing, because we only have three rows
//      maybe something for combos?
enum custom_keycodes {
    DUMMY = SAFE_RANGE,
    U_ENC1,
    U_ENC2,
    U_BALL,
    M_E1, // Encoder settings
    M_E2,
    M_E3,
    M_E4,
    M_E5,
    M_E6,
    C_EUR,
    C_A_A,
    C_A_G,
    C_A_T,
    C_E_A,
    C_E_G,
    C_E_T,
    C_U_A,
    C_U_G,
    C_U_T,
    C_I_A,
    C_I_G,
    C_I_T,
    C_O_A,
    C_O_G,
    C_O_T,
};

void composed(uint16_t keycode) {
  uint8_t mod = get_mods();
  uint8_t osm = get_oneshot_mods();

  clear_mods();
  clear_oneshot_mods();
  tap_code16(KC_COMP);
  set_mods(mod);
  set_oneshot_mods(osm);

  switch (keycode) {
    case C_EUR:
      SEND_STRING("e=");
      break;
  }
}

void diacritic(uint16_t keycode) {
  const uint16_t dia_accents[] = { KC_QUOTE, KC_GRAVE, KC_DOUBLE_QUOTE };
  const uint16_t dia_letters[] = { KC_A, KC_E, KC_U, KC_I, KC_O };

  uint16_t letter = dia_letters[(keycode - C_A_A) / 3];
  uint16_t accent = dia_accents[(keycode - C_A_A) % 3];

  uint8_t mod = get_mods();
  uint8_t osm = get_oneshot_mods();

  clear_mods();
  clear_oneshot_mods();
  tap_code16(KC_COMP);
  tap_code16(accent);
  set_mods(mod);
  set_oneshot_mods(osm);

  tap_code16(letter);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      case C_EUR ... C_EUR:
        if (record->event.pressed)
          composed(keycode);
        break;
      case C_A_A ... C_O_T:
        if (record->event.pressed)
          diacritic(keycode);
        break;
    }
    return true;
}

void matrix_init_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_init(TB_PRESS_MATRIX(6,1), TB_MOVE_MANUAL, TB_LED_MIRROR(10));
  #endif
}

void matrix_scan_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_scan();
  #endif
}

// Legend:
// ▲    Layer key
// ▼    Transparent, uses layer below
// ╳    Dead key, unused
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

// Configuration
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// | Enc 1 | Enc 2 | Sat + | Hue + | Val + |       |                                  |       |       |       |       |       | PrScr |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// | Enc 3 | Enc 4 | <Mode | On/Off| Mode> | EEPROM|                                  | EEPROM|       |       |       |       |  Ins  |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// | Enc 5 | Enc 6 | Sat - | Hue - | Val - | Reset |   ▼   |   ▼   |  |   ▼   |   ▼   | Reset |       |       |       |       |  Caps |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_CONF] = LAYOUT(
    M_E1, M_E2,  RGB_SAI, RGB_HUI, RGB_VAI, _______,                                       _______, _______, _______, _______, _______, KC_PSCR,
    M_E3, M_E4, RGB_RMOD, RGB_TOG, RGB_VAD, EEP_RST,                                       EEP_RST, _______, _______, _______, _______, KC_INS,
    M_E5, M_E6,  RGB_SAD, RGB_HUD, RGB_MOD,   RESET, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   RESET, _______, _______, _______, _______, KC_CAPS,
                          KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Mouse
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |       |       |       |       |       |       |                                  |       |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |       |       |       |       |       |                                  |       |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |       |       |       |       |       |       |       |  |       |       |       |       |       |       |       |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         `---------------------------------------'  `---------------------------------------'
  [_MOUS] = LAYOUT(
    KC_TRNS, _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______, KC_TRNS,
    _______, _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, _______, _______, _______, _______, _______, _______,
                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Navigation
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |   ▼   |  F13  |  F14  |  F15  |  F16  |       |                                  |       |       |       |       |       |   ▼   |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |  F17  |  F18  |  F19  |  F20  |       |                                  |       |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |  F21  |  F22  |  F23  |  F24  |       |   ▼   |   ▼   |  |   ▼   |   ▼   |       |       |       |       |       |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_NAVI] = LAYOUT(
    KC_TRNS, KC_F13, KC_F14,  KC_F15,  KC_F16, _______,                                       _______, _______, _______, _______, _______, KC_TRNS,
    _______, KC_F17, KC_F18,  KC_F19,  KC_F20, _______,                                       _______, _______, _______, _______, _______, _______,
    _______, KC_F21, KC_F22,  KC_F23,  KC_F24, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, _______, _______, _______, _______, _______, _______,
                             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Accents
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |   ▼   |  F1   |  F2   |  F3   |  F4   |   €   |                                  |   à   |   è   |   ù   |   ì   |   ò   |   ▼   |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |  F5   |  F6   |  F7   |  F8   |       |                                  |   ä   |   ë   |   ü   |   ï   |   ö   |   `   |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |  F9   |  F10  |  F11  |  F12  |       |   ▼   |   ▼   |  |   ▼   |   ▼   |   á   |   é   |   ú   |   í   |   ó   |Compose|
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-------+---------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_ACCS] = LAYOUT(
    KC_TRNS, KC_F1,  KC_F2,   KC_F3,   KC_F4,   C_EUR,                                         C_A_G,   C_E_G,   C_U_G, C_I_G, C_O_G, KC_TRNS,
    _______, KC_F5,  KC_F6,   KC_F7,   KC_F8, _______,                                         C_A_T,   C_E_T,   C_U_T, C_I_T, C_O_T, KC_GRV,
    _______, KC_F9, KC_F10,  KC_F11,  KC_F12, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS,   C_A_A,   C_E_A,   C_U_A, C_I_A, C_O_A, KC_COMP,
                            KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Symbols
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |   ▼   |   1   |   2   |   3   |   4   |   5   |                                  |   6   |   7   |   8   |   9   |   0   |   ▼   |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |   _   |   !   |   @   |   #   |   $   |   |   |                                  |   /   |   (   |   )   |   <   |   >   |   =   |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |   -   |   %   |   ^   |   &   |   *   |   ~   |   ▼   |   ▼   |  |   ▼   |   ▼   |   \   |   {   |   }   |   [   |   ]   |   +   |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_SYMB] = LAYOUT(
    KC_TRNS,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                          KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_TRNS,
    KC_UNDS, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PIPE,                                       KC_SLSH, KC_LPRN, KC_RPRN,   KC_LT,   KC_GT,  KC_EQL,
    KC_MINS, KC_PERC, KC_CIRC, KC_AMPR, KC_ASTR, KC_TILD, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_BSLS, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, KC_PLUS,
                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Base
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |  Esc  |   q   |   w   |   e   |   r   |   t   |                                  |   y   |   u   |   i   |   o   |   p   |  Bspc |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |  Tab  |   a   |   s   |   d   |   f   |   g   |                                  |   h   |   j   |   k   |   l   |  ' "  | Enter |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// | S-Tab |   z   |   x   |   c   |   v   |   b   | ▲Mouse| ▲Conf |  | ▲Conf |  BALL |   n   |   m   |  , ;  |  . :  |  ? !  |  Del  |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         | ENC1  |▲Accent|  ▲Nav | Space |▲Symbol|  |  Ctrl | Shift |  Alt  |  GUI  | ENC2  |
//                         `---------------------------------------'  `---------------------------------------'
  [_BASE] = LAYOUT(
       KC_ESC, KC_Q, KC_W,    KC_E,       KC_R,       KC_T,                                                 KC_Y,    KC_U,     KC_I,      KC_O,    KC_P, KC_BSPC,
       KC_TAB, KC_A, KC_S,    KC_D,       KC_F,       KC_G,                                                 KC_H,    KC_J,     KC_K,      KC_L, KC_QUOT,  KC_ENT,
    S(KC_TAB), KC_Z, KC_X,    KC_C,       KC_V,       KC_B, MO(_MOUS),  MO(_CONF),   MO(_CONF),  U_BALL,    KC_N,    KC_M, KC_COMMA,    KC_DOT, KC_QUES,  KC_DEL,
                            U_ENC1, OSL(_ACCS), OSL(_NAVI),    KC_SPC, OSL(_SYMB),    KC_LCTRL, KC_LSFT, KC_LALT, KC_LGUI,   U_ENC2
  ),

// Layer template
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |       |       |       |       |       |       |                                  |       |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |       |       |       |       |       |                                  |       |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |       |       |       |       |       |       |       |  |       |       |       |       |       |       |       |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         `---------------------------------------'  `---------------------------------------'
//  [_LAYERINDEX] = LAYOUT(
//    _______, _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______, _______,
//    _______, _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______, _______,
//    _______, _______, _______, _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______, _______, _______, _______,
//                               _______, _______, _______, _______, _______,   _______, _______, _______, _______, _______
//  ),
};
