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

#include "i2c_master.h"
#include "print.h"

#include "timer.h"

#include "timer_hires.h"

enum layers {
    _QWERTY = 0,
};

enum custom_keycodes {
    DUMMY = SAFE_RANGE,
    ENC_L,
    ENC_R
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uprintf("KL: kc: %u, col: %u, row: %u, pressed: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed);
    switch (keycode) {
        /*case KC_ENC_L:
            if (record->event.pressed) {
              encoder_mode++;
              if (encoder_mode == _enc_done) {
                encoder_mode = 0;
              }
            }
            break;*/
    }
    return true;
}


/*void housekeeping_task_user(void) {
    if (encoder_in_use && timer_elapsed(encoder_pulse_timer) > 1000) {
        encoder_in_use = false;
    }
}*/

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

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |RAIS/ESC|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  | \   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Ctrl/BS |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | LShift |   Z  |   X  |   C  |   V  |   B  |LShift|LShift|  |LShift|LShift|   N  |   M  | ,  < | . >  | /  ? |  - _   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | ENC  | Del  | Enter| Space| Esc  |  | Enter| Space| Tab  | Bksp | AltGr|
 *                        |      |      | Alt  | Lower| Raise|  | Lower| Raise|      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_A, KC_A, RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,                              RGB_SAI, RGB_HUI, RGB_VAI, RGB_MOD,  KC_A, KC_A,
      KC_A, KC_A, KC_A,    KC_A,    KC_A,    KC_A,                                 KC_A,    KC_A,    KC_A,    KC_A,     KC_A, KC_A,
      KC_A, KC_A, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, KC_A, KC_A,     KC_A, KC_A, RGB_SAD, RGB_HUD, RGB_VAD, RGB_RMOD, KC_A, KC_A,
                           ENC_L,   KC_A,    KC_A,     KC_A, KC_A,     KC_A, KC_A, KC_A,    KC_A,    ENC_R
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
