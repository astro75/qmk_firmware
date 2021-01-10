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
#ifdef ENCODER_ENABLE
#    include "encoder_utils.h"
#endif

#include "print.h"

enum layers {
    _BASE = 0,
    _SYMB,
    _NUMB,
    _NAVI,
    _MOUS,
    _CONF,
};

enum custom_keycodes {
    // Unused, placeholder for the start value
    DUMMY = SAFE_RANGE,
    // Mode-dependent keys
    U_ENC1,
    U_ENC2,
    U_BALL,
    // Mode configuration
    /// Encoder
    M_E1,
    M_E2,
    M_E3,
    /// Ball
    M_B1,
    M_B2,
    M_B3,
    // Composed symbols
    C_EUR,
    // Diacritic prefixes
    C_ACUTE, // á
    C_GRAVE, // à
    C_CIRCU, // â
    C_TREMA, // ä
};

void matrix_init_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_init(TB_PRESS_MATRIX(6,1), TB_MOVE_MANUAL, TB_LED_MIRROR(10));
  #endif
  #ifdef ENCODER_ENABLE
    enc_init();
  #endif
}

void matrix_scan_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_scan();
  #endif
  #ifdef ENCODER_ENABLE
    enc_tick();
  #endif
}

void composed(uint16_t keycode) {
  uint8_t mod = get_mods();

  clear_mods();
  tap_code16(KC_COMP);
  set_mods(mod);

  switch (keycode) {
    case C_EUR:
      SEND_STRING("e=");
      break;
  }
}

void diacritic(uint16_t keycode) {
  //                               á         à         â              ä
  const uint16_t dia_accents[] = { KC_QUOTE, KC_GRAVE, KC_CIRCUMFLEX, KC_DOUBLE_QUOTE};

  uint16_t accent = dia_accents[keycode - C_ACUTE];

  uint8_t mod = get_mods();
  clear_mods();
  tap_code16(KC_COMP);
  tap_code16(accent);
  set_mods(mod);
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
      #ifdef ENCODER_ENABLE
        case U_ENC1 ... U_ENC2:
          enc_press(keycode - U_ENC1, record);
          return false;
        case M_E1:
          enc_set_mode(0, ENC_SCROLL);
          return false;
        case M_E2:
          enc_set_mode(0, ENC_TAB);
          return false;
        case M_E3:
          enc_set_mode(0, ENC_MEDIA);
          return false;
      #endif

      case C_EUR ... C_EUR:
        if (record->event.pressed)
          composed(keycode);
        return false;
      case C_ACUTE ... C_TREMA:
        if (record->event.pressed)
          diacritic(keycode);
        return false;
    }
    return true;
}

#ifdef ENCODER_ENABLE
  void encoder_update_user(uint8_t index, bool clockwise) {
    enc_turn(index, clockwise);
  }
#endif

// https://config.qmk.fm/#/test
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
// Legend:
// ▲    Layer key
// ▼    Transparent, uses layer below
// ⥮    Mode-dependent key
// ⇌    Modesetting key
// ╳    Dead key, unused
// ⇉    Compose accent: press vowel afterwards to combine
// ⇶    Composed character

// Note that all layers are listed top-to-bottom!

// Configuration
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// | ⇌Enc 1|⇌Ball 1| Sat + | Hue + | Val + |       |                                  |       |       |       |       |       | PrScr |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// | ⇌Enc 2|⇌Ball 2| <Mode | On/Off| Mode> | EEPROM|                                  | EEPROM|       |       |       |       |  Ins  |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// | ⇌Enc 3|⇌Ball 3| Sat - | Hue - | Val - | Reset |Standby|   ▼   |  |   ▼   |   ▼   | Reset |       |       |       |       |  Caps |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_CONF] = LAYOUT(
    M_E1, M_B1,  RGB_SAI, RGB_HUI, RGB_VAI, _______,                                        _______, _______, _______, _______, _______, KC_PSCR,
    M_E2, M_B2, RGB_RMOD, RGB_TOG, RGB_MOD, EEP_RST,                                        EEP_RST, _______, _______, _______, _______, KC_INS,
    M_E3, M_B3,  RGB_SAD, RGB_HUD, RGB_VAD,   RESET, KC_POWER, KC_TRNS,   KC_TRNS, KC_TRNS,   RESET, _______, _______, _______, _______, KC_CAPS,
                          KC_TRNS, KC_TRNS, KC_TRNS,  KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Mouse
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |                                  |  M 4  |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |                                  |  M 5  |       |       |       |       |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |  | Acc 2 |   ▼   | Acc 3 |       |       |       |       |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ╳   |   ╳   |   ╳   |   ╳   |   ╳   |  |   L   |   M   |   R   | Scroll|       |
//                         `---------------------------------------'  `---------------------------------------'
  [_MOUS] = LAYOUT(
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       KC_BTN4, _______, _______, _______, _______, _______,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       KC_BTN5, _______, _______, _______, _______, _______,
    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   _______, KC_TRNS, _______, _______, _______, _______, _______, _______,
                               XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,   KC_BTN1, KC_BTN2, KC_BTN3, _______, _______
  ),

// Navigation
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |       |  F13  |  F14  |  F15  |  F16  |       |                                  |       |   ↥   |   ⬆   |   ⇈   |       |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |  F17  |  F18  |  F19  |  F20  |       |                                  |       |   ⬅   |   ↓   |   ➡   |       |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |  F21  |  F22  |  F23  |  F24  |       |   ▼   |   ▼   |  |   ▼   |   ▼   |       |   ↧   |       |   ⇊   |       |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_NAVI] = LAYOUT(
    _______, KC_F13, KC_F14,  KC_F15,  KC_F16, _______,                                       _______, KC_HOME,   KC_UP, KC_PGUP, _______, _______,
    _______, KC_F17, KC_F18,  KC_F19,  KC_F20, _______,                                       _______, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
    _______, KC_F21, KC_F22,  KC_F23,  KC_F24, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, _______,  KC_END, _______, KC_PGDN, _______, _______,
                             KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Numbers
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |Compose|  ⇉ â  |  ⇉ à  |  ⇉ ä  |  ⇉ á  |  ⇶ €  |                                  |       |  F1   |  F2   |  F3   |  F4   |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |   0   |   1   |   2   |   3   |   4   |   5   |                                  |       |  F5   |  F6   |  F7   |  F8   |       |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |   6   |   7   |   8   |   9   |       |   ▼   |   ▼   |  |   ▼   |   ▼   |       |  F9   |  F10  |  F11  |  F12  |       |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_NUMB] = LAYOUT(
    KC_COMP, C_CIRCU, C_GRAVE, C_TREMA, C_ACUTE,   C_EUR,                                       _______,   KC_F1,  KC_F2,  KC_F3,  KC_F4, _______,
       KC_0,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                       _______,   KC_F5,  KC_F6,  KC_F7,  KC_F8, _______,
    _______,    KC_6,    KC_7,    KC_8,    KC_9, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, _______,   KC_F9, KC_F10, KC_F11, KC_F12, _______,
                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Symbols
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |       |   ?   |   \   |   |   |   /   |       |                                  |       |   <   |   =   |   _   |   >   |       |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |   ~   |   !   |   @   |   #   |   $   |   %   |                                  |   {   |   [   |   (   |   )   |   ]   |   }   |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |   ^   |   &   |   *   |   `   |       |   ▼   |   ▼   |  |   ▼   |   ▼   |   -   |   "   |   ;   |   :   |   '   |   +   |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_SYMB] = LAYOUT(
    _______, KC_QUES, KC_BSLS, KC_PIPE, KC_SLSH, _______,                                       _______,   KC_LT,  KC_EQL, KC_UNDS,   KC_GT, _______,
    KC_TILD, KC_EXLM,   KC_AT, KC_HASH,  KC_DLR, KC_PERC,                                       KC_LCBR, KC_LBRC, KC_LPRN, KC_RPRN, KC_RBRC, KC_RCBR,
    _______, KC_CIRC, KC_AMPR, KC_ASTR,  KC_GRV, _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_MINS, KC_DQUO, KC_SCLN, KC_COLN, KC_QUOT, KC_PLUS,
                               KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
  ),

// Base
//  v mode?
// ,-----------------------------------------------.                                  ,-----------------------------------------------.
// |       |   q   |   w   |   e   |   r   |   t   |                                  |   y   |   u   |   i   |   o   |   p   |  Del  |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |  Esc  |   a   |   s   |   d   |   f   |   g   |                                  |   h   |   j   |   k   |   l   | Enter |  Bspc |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |   z   |   x   |   c   |   v   |   b   | Lock  | ▲Conf |  | ▲Conf | ▲Mouse|   n   |   m   |   ,   |   .   |  Tab  | S-Tab |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         | ⥮ENC1 | ▲Navi | ▲Numb | Space | ▲Symb |  |  Ctrl | Shift |  Alt  |  GUI  | ⥮ENC2 |
//                         `---------------------------------------'  `---------------------------------------'
  [_BASE] = LAYOUT(
    _______, KC_Q, KC_W,   KC_E,       KC_R,       KC_T,                                                 KC_Y,    KC_U,     KC_I,   KC_O,   KC_P,    KC_DEL,
     KC_ESC, KC_A, KC_S,   KC_D,       KC_F,       KC_G,                                                 KC_H,    KC_J,     KC_K,   KC_L, KC_ENT,   KC_BSPC,
    _______, KC_Z, KC_X,   KC_C,       KC_V,       KC_B, G(KC_L),  MO(_CONF),   MO(_CONF), TG(_MOUS),    KC_N,    KC_M, KC_COMMA, KC_DOT, KC_TAB, S(KC_TAB),
                         U_ENC1, OSL(_NAVI), OSL(_NUMB),  KC_SPC, OSL(_SYMB),    KC_LCTRL,   KC_LSFT, KC_LALT, KC_LGUI,   U_ENC2
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
