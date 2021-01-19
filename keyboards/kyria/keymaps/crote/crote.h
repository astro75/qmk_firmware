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
    U_BALL,
    // Mode configuration
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
