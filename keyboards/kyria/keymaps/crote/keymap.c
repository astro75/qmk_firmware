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
#include "crote.h"

#ifdef TRACKBALL_ENABLE
#    include "trackball.h"
#endif

#include "print.h"

void matrix_init_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_init(TB_PRESS_MATRIX(6,1), TB_MOVE_MANUAL, TB_LED_MIRROR(10));
  #endif
  #ifdef ENCODER_ENABLE
    matrix_init_enc();
  #endif
}

void matrix_scan_user(void) {
  #ifdef TRACKBALL_ENABLE
    trackball_scan();
  #endif
  #ifdef ENCODER_ENABLE
    matrix_scan_enc();
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
    #ifdef ENCODER_ENABLE
      if (!process_record_encoder(keycode, record)) return false;
    #endif

    switch (keycode) {
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

void suspend_power_down_user(void) {

}

void suspend_wakeup_init_user(void) {

}

// This layout is intended to be used with `English (intl., with AltGr dead keys)`
// with a manually defined `Compose` on Right Ctrl.
// It will be switched to `EurKEY (US)` once the following bug is fixed:
// https://gitlab.freedesktop.org/xkeyboard-config/xkeyboard-config/-/issues/246

// Why switch to EurKEY?
// - Integrated Compose key
// - Dead key for Greek letters

// Full list of compose combinations can be found in
//   /usr/share/X11/locale/en_US.UTF-8/Compose
// or
//   https://www.x.org/releases/X11R7.7/doc/libX11/i18n/compose/en_US.UTF-8.html
// This can be extended using a per-user config file
//   https://wiki.archlinux.org/index.php/Xorg/Keyboard_configuration#Configuring_compose_key
//   https://linux.die.net/man/5/compose
// Mutter uses libxkbcommon and that definitely has support for it,
// but does it actually use it?
// Note that composed keys do not support press & hold
#define KC_COMP (KC_RCTRL)

// https://config.qmk.fm/#/test
// or `xkbcli interactive-x11` from `libxkbcommon-utils`
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
// |       |⇌Ball 1| Sat + | Hue + | Val + |       |                                  |       |       |       |       |       | PrScr |
// |-------+-------+-------+-------+-------+-------|                                  |-------+-------+-------+-------+-------+-------|
// |       |⇌Ball 2| <Mode | On/Off| Mode> | EEPROM|                                  | EEPROM|       |       |       |       |  Ins  |
// |-------+-------+-------+-------+-------+-------+---------------.  ,---------------+-------+-------+-------+-------+-------+-------|
// |       |⇌Ball 3| Sat - | Hue - | Val - | Reset |Standby|   ▼   |  |   ▼   |   ▼   | Reset |       |       |       |       |  Caps |
// `-----------------------+-------+-------+-------+-------+-------|  |-------+-------+-------+-------+-------+-----------------------'
//                         |       |       |       |       |       |  |       |       |       |       |       |
//                         |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |  |   ▼   |   ▼   |   ▼   |   ▼   |   ▼   |
//                         `---------------------------------------'  `---------------------------------------'
  [_CONF] = LAYOUT(
    _______, M_B1,  RGB_SAI, RGB_HUI, RGB_VAI, _______,                                        _______, _______, _______, _______, _______, KC_PSCR,
    _______, M_B2, RGB_RMOD, RGB_TOG, RGB_MOD, EEP_RST,                                        EEP_RST, _______, _______, _______, _______, KC_INS,
    _______, M_B3,  RGB_SAD, RGB_HUD, RGB_VAD,   RESET, KC_POWER, KC_TRNS,   KC_TRNS, KC_TRNS,   RESET, _______, _______, _______, _______, KC_CAPS,
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
    KC_COMP, ALGR(KC_CIRC), ALGR(KC_GRV), ALGR(KC_DQUO), ALGR(KC_QUOT), ALGR(KC_5),                                       _______,   KC_F1,  KC_F2,  KC_F3,  KC_F4, _______,
       KC_0,          KC_1,         KC_2,          KC_3,          KC_4,       KC_5,                                       _______,   KC_F5,  KC_F6,  KC_F7,  KC_F8, _______,
    _______,          KC_6,         KC_7,          KC_8,          KC_9,    _______, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, _______,   KC_F9, KC_F10, KC_F11, KC_F12, _______,
                                                KC_TRNS,       KC_TRNS,    KC_TRNS, KC_TRNS, KC_TRNS,   KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS
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
    _______, KC_Q, KC_W,  KC_E,       KC_R,       KC_T,                                                 KC_Y,    KC_U,     KC_I,   KC_O,   KC_P,    KC_DEL,
     KC_ESC, KC_A, KC_S,  KC_D,       KC_F,       KC_G,                                                 KC_H,    KC_J,     KC_K,   KC_L, KC_ENT,   KC_BSPC,
    _______, KC_Z, KC_X,  KC_C,       KC_V,       KC_B, G(KC_L),  MO(_CONF),   MO(_CONF), TG(_MOUS),    KC_N,    KC_M, KC_COMMA, KC_DOT, KC_TAB, S(KC_TAB),
                         ENC_L, OSL(_NAVI), OSL(_NUMB),  KC_SPC, OSL(_SYMB),    KC_LCTRL,   KC_LSFT, KC_LALT, KC_LGUI,    ENC_R
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
