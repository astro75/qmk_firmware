#pragma once

#include QMK_KEYBOARD_H

#ifdef ENCODER_ENABLE
#    include "enc.h"
#endif

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
    #ifdef ENCODER_ENABLE
      ENC_KEYS
    #endif
    // Trackball speed
    M_FAST,
    M_SLOW,
};
