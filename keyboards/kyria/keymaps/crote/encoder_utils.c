#include QMK_KEYBOARD_H
#include "encoder_utils.h"

#include "pointing_device.h"

// Subdivisions for 72 steps:
// 36, 24, 18, 12, 9, 8, 6, 4, 3, 2

//// State data
// Mode-specific data
typedef struct {

} scroll_t;
typedef struct {
	bool window;
	//uint8_t steps;
	uint16_t timer;
} tab_t;
typedef struct {
	bool turned;
} media_t;


typedef struct {
	uint8_t mode;
	bool pressed;
	union {
		scroll_t scroll;
		tab_t tab;
		media_t media;
	} data;
} encoder_state_t;

static encoder_state_t state[ENCODER_COUNT];

//// Callbacks
typedef struct {
	void (*init)(encoder_state_t *s);
	void (*press)(encoder_state_t *s, keyrecord_t *record);
	void (*turn)(encoder_state_t *s, bool clockwise);
	void (*tick)(encoder_state_t *s);
} encoder_mode_t;

// Scroll mode
void scroll_turn(encoder_state_t *s, bool clockwise) {
	int8_t offset = clockwise ? -1 : 1;
	if (s->pressed) {
			offset *= 5;
	}
	report_mouse_t report = {.v = offset};
	host_mouse_send(&report);
};

// Alt-tab mode
void tab_init(encoder_state_t *s) {
	s->data.tab.window = false;
	s->data.tab.timer = 0;
	//s->data.tab.steps = (ENCODER_TICKS / 6) / 2;
}

void tab_press(encoder_state_t *s, keyrecord_t *record) {
	if (s->pressed) {
		s->data.tab.timer = timer_read();

		if (! s->data.tab.window) {
			s->data.tab.window = true;
			register_code(KC_LALT);
			tap_code16(KC_TAB);
			tap_code16(S(KC_TAB)); // Go back so we start on current window
		} else {
			unregister_code(KC_LALT);
			tab_init(s);
		}
	}
}

void tab_turn(encoder_state_t *s, bool clockwise) {
	s->data.tab.timer = timer_read();

	if (s->data.tab.window) {
		if (clockwise) {
			tap_code16(KC_TAB);
		} else {
			tap_code16(S(KC_TAB));
		}
	} else {
		if (clockwise) {
			tap_code16(C(KC_TAB));
		} else {
			tap_code16(C(S(KC_TAB)));
		}
	}
}

void tab_tick(encoder_state_t *s) {
	if (s->data.tab.timer != 0 && timer_elapsed(s->data.tab.timer) > ENCODER_TIMEOUT) {
		if (s->data.tab.window) {
			unregister_code(KC_LALT);
		}
		tab_init(s);
	}
}

// Media control
void media_press(encoder_state_t *s, keyrecord_t *record) {
	if (s->pressed) {
		s->data.media.turned = false;
	} else {
		if (!s->data.media.turned) {
			tap_code(KC_MEDIA_PLAY_PAUSE);
		}
	}
}

void media_turn(encoder_state_t *s, bool clockwise) {
	if (s->pressed) {
		if (clockwise) {
			tap_code(KC_MEDIA_NEXT_TRACK);
		} else {
			tap_code(KC_MEDIA_PREV_TRACK);
		}
		s->data.media.turned = true;
	} else {
		if (clockwise) {
			tap_code(KC_AUDIO_VOL_UP);
		} else {
			tap_code(KC_AUDIO_VOL_DOWN);
		}
	}
}

static encoder_mode_t modes[] = {
	[ENC_SCROLL] = {.init = NULL, .press = NULL, .turn = &scroll_turn, .tick = NULL},
	[ENC_TAB] = {.init = &tab_init, .press = &tab_press, .turn = &tab_turn, .tick = &tab_tick},
	[ENC_MEDIA] = {.init = NULL, .press = &media_press, .turn = &media_turn, .tick = NULL},
};

//// Dispatching code
void enc_init(void) {
	for (int i = 0; i < ENCODER_COUNT; i++) {
		state[i].pressed = false;
		enc_set_mode(i, 0); // TODO: get default mode from somewhere?
	}
};

void enc_set_mode(uint8_t index, uint8_t mode) {
	state[index].mode = mode;
	if (modes[state[index].mode].init != NULL) {
	  	modes[state[index].mode].init(&state[index]);
	}
};

void enc_press(uint8_t index, keyrecord_t *record) {
	state[index].pressed = record->event.pressed;
	if (modes[state[index].mode].press != NULL) {
	  	modes[state[index].mode].press(&state[index], record);
	}
};

void enc_turn(uint8_t index, bool clockwise) {
	if (modes[state[index].mode].turn != NULL) {
	  	modes[state[index].mode].turn(&state[index], clockwise);
	}
};

void enc_tick(void) {
	for (int i = 0; i < ENCODER_COUNT; i++) {
		if (modes[state[i].mode].tick != NULL) {
		  	modes[state[i].mode].tick(&state[i]);
		}
	}
};
