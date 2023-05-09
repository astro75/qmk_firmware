#pragma once

#ifdef OLED_ENABLE
  #define OLED_DISPLAY_128X64
  #define SPLIT_LAYER_STATE_ENABLE
  #define SPLIT_MODS_ENABLE
  #define OLED_FONT_HEIGHT 16 
  #define OLED_FONT_WIDTH 12
#endif

#ifdef RGBLIGHT_ENABLE
// #define RGBLIGHT_EFFECT_BREATHING
// #define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #define RGBLIGHT_EFFECT_SNAKE
// #define RGBLIGHT_EFFECT_KNIGHT
// #define RGBLIGHT_EFFECT_CHRISTMAS
// #define RGBLIGHT_EFFECT_STATIC_GRADIENT
// #define RGBLIGHT_EFFECT_RGB_TEST
// #define RGBLIGHT_EFFECT_ALTERNATING
// #define RGBLIGHT_EFFECT_TWINKLE
  #define RGBLIGHT_HUE_STEP 1
  #define RGBLIGHT_SAT_STEP 2
  #define RGBLIGHT_VAL_STEP 2
  #define RGBLIGHT_SLEEP
#endif

// EC11K encoders have a different resolution than other EC11 encoders.
// When using the default resolution of 4, if you notice your encoder skipping
// every other tick, lower the resolution to 2.
#define ENCODER_RESOLUTION 2

// The Leader key allows to flexibly assign macros to key sequences.
#define LEADER_PER_KEY_TIMING
#define LEADER_TIMEOUT 350

#define TAPPING_TERM 200

// Allows to use either side as the master. Look at the documentation for info:
// https://docs.qmk.fm/#/config_options?id=setting-handedness
#define EE_HANDS

// Allows media codes to properly register in macros and rotary encoder code
#define TAP_CODE_DELAY 10

#define OLED_TIMEOUT 600000 // 10 mins


#define PIMORONI_TRACKBALL_SCALE 1
#define SPLIT_POINTING_ENABLE
#define POINTING_DEVICE_RIGHT


#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE
#define NO_ACTION_ONESHOT
// #define NO_ACTION_TAPPING
#define LAYER_STATE_8BIT
