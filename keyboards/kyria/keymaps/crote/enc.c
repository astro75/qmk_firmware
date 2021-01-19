#include "crote.h"

void scroll(enc_action_t action) {
    int8_t offset = 0;
    switch (action & ENC_MSK) {
        case ENC_CW:
            offset = -1;
            break;
        case ENC_CCW:
            offset = 1;
            break;
        case ENC_DOWN:
            tap_code16(KC_Q); // cancel scroll in tmux
        default: return;
    }
    if (action & ENC_PRESSED) offset *= 5;
    if (offset != 0) {
        report_mouse_t report = {.v = offset};
        host_mouse_send(&report);
    }
}

void zoom(enc_action_t action) {
    switch (action & ENC_MSK) {
        case ENC_CW:
            tap_code16(C(KC_PLUS));
            break;
        case ENC_CCW:
            tap_code16(C(KC_MINUS));
            break;
        case ENC_DOWN:
            tap_code16(C(KC_0));
            break;
        default: return;
    }
}

void volume(enc_action_t action) {
    switch (action & ENC_MSK) {
        case ENC_CW:
            tap_code16(KC_AUDIO_VOL_UP);
            break;
        case ENC_CCW:
            tap_code16(KC_AUDIO_VOL_DOWN);
            break;
        case ENC_DOWN:
            tap_code16(KC_AUDIO_MUTE);
            break;
        default: return;
    }
}

void media(enc_action_t action) {
    switch (action & ENC_MSK) {
        case ENC_CW:
            tap_code16(KC_MEDIA_NEXT_TRACK);
            break;
        case ENC_CCW:
            tap_code16(KC_MEDIA_PREV_TRACK);
            break;
        case ENC_DOWN:
            tap_code16(KC_MEDIA_PLAY_PAUSE);
            break;
        default: return;
    }
}

void tab(enc_action_t action) {
    switch (action & ENC_MSK) {
        case ENC_CW:
            tap_code16(C(KC_TAB));
            break;
        case ENC_CCW:
            tap_code16(S(C(KC_TAB)));
            break;
        default: return;
    }
}

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
const uint16_t alt_tab_timeout = 500;
void window(enc_action_t action) {
    uint16_t kc;
    switch (action & ENC_MSK) {
        case ENC_TICK:
            if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > alt_tab_timeout) {
                unregister_code(KC_LALT);
                is_alt_tab_active = false;
            }
            return;
        case ENC_CW:
            kc = KC_TAB;
            break;
        case ENC_CCW:
            kc = S(KC_TAB);
            break;
        default: return;
    }
    if (!is_alt_tab_active) {
        register_code(KC_LALT);
    }
    tap_code16(kc);
    is_alt_tab_active = true;
    alt_tab_timer = timer_read();
}

void workspace(enc_action_t action) {
    switch (action) {
        case ENC_CW:
            tap_code16(G(KC_PGDOWN));
            break;
        case ENC_CCW:
            tap_code16(G(KC_PGUP));
            break;
        case ENC_DOWN:
            tap_code16(G(KC_HOME));
            break;
        default: return;
    }
}

void brightness(enc_action_t action) {
    switch (action & ENC_MSK) {
        case ENC_CW:
            tap_code16(KC_BRIGHTNESS_UP);
            break;
        case ENC_CCW:
            tap_code16(KC_BRIGHTNESS_DOWN);
            break;
        case ENC_DOWN:
            tap_code16(KC_SYSTEM_POWER);
        default: return;
    }
}

void encoder_execute(uint8_t index, enc_action_t action) {
    layer_state_t layer = get_highest_layer(layer_state);
    if (index == 0) {  // Left encoder
        switch (layer) {
            case _BASE:
                scroll(action);
                break;
            case _SYMB:
                media(action);
                break;
            case _NUMB:
                volume(action);
                break;
            case _NAVI:
                zoom(action);
            default:
                // Do nothing
                break;
        }
    } else if (index == 1) {  // Right encoder
        switch (get_highest_layer(layer_state)) {
            case _BASE:
                tab(action);
                break;
            case _SYMB:
                window(action);
                break;
            case _NUMB:
                workspace(action);
                break;
            case _NAVI:
                brightness(action);
                break;
            default:
                // Do nothing
                break;
        }
    }

    // We use one-shot layers, but that doesn't work well when
    // the oneshot is an encoder action.
    // So we need to ensure that oneshot is reset
    clear_oneshot_layer_state(ONESHOT_OTHER_KEY_PRESSED);
}

// Some actions have a timeout, so they
// need to get a tick every so often
void matrix_scan_enc(void) {
    window(ENC_TICK);
}

bool pressed[2];
void encoder_update_user(uint8_t index, bool clockwise) {
    enc_action_t action;
    if (clockwise) {
        action = ENC_CW;
    } else {
        action = ENC_CCW;
    }
    if (pressed[index]) {
        action |= ENC_PRESSED;
    }
    encoder_execute(index, action);
}

void matrix_init_enc(void) {
    pressed[0] = false;
    pressed[1] = false;
}

bool process_record_encoder(uint16_t keycode, keyrecord_t *record) {
    uint8_t idx = 0; // 0 for left, 1 for right
    switch (keycode) {
        case ENC_R:
            idx++;
        case ENC_L:
            if (record->event.pressed) {
                pressed[idx] = true;
                encoder_execute(idx, ENC_DOWN | ENC_PRESSED);
            } else {
                pressed[idx] = false;
                encoder_execute(idx, ENC_UP);
            };
            return false;
        default:
            return true;
    }
}
