#pragma once
#include "quantum.h"

#ifndef ENCODER_TICKS
	#error "Set `ENCODER_TICKS` to the amount of pulses your encoders have per rotation"
#endif
#ifndef ENCODER_COUNT
	#error "Set `ENCODER_COUNT` to the number of encoders"
#endif
#ifndef ENCODER_TIMEOUT
	#define ENCODER_TIMEOUT 5000
#endif

#define NUMBER_OF_ENCODERS (sizeof(encoders_pad_a) / sizeof(pin_t))

enum encoder_modes {
	ENC_SCROLL,
	ENC_TAB,
	ENC_MEDIA,
};

void enc_init(void);
void enc_set_mode(uint8_t index, uint8_t mode);
void enc_press(uint8_t index, keyrecord_t *record);
void enc_turn(uint8_t index, bool clockwise);
void enc_tick(void);
