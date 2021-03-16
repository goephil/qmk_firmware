#include QMK_KEYBOARD_H
#include "muse.h"

enum preonic_layers { _QWERTZ, _LOWER, _RAISE, _ADJUST };

enum preonic_keycodes {
    QWERTZ = SAFE_RANGE,
    LOWER,
    RAISE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // Qwertz
    [_QWERTZ] = LAYOUT_preonic_grid(KC_GESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_RBRC, KC_MPLY, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_NUHS, KC_ENT, KC_LSFT, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_RSFT, KC_LCTL, KC_LGUI, KC_NUBS, KC_LALT, LOWER, KC_SPC, KC_SPC, RAISE, KC_RALT, KC_RGUI, KC_APP, KC_RCTL),

    // Lower
    [_LOWER] = LAYOUT_preonic_grid(RGB_TOG, RGB_RMOD, RGB_MOD, _______, _______, _______, _______, _______, _______, _______, KC_MINS, KC_DEL, RGB_SPI, RGB_HUD, RGB_HUI, _______, _______, _______, _______, _______, _______, _______, _______, KC_LBRC, RGB_SPD, RGB_SAD, RGB_SAI, _______, _______, _______, _______, DM_PLY1, DM_PLY2, _______, KC_SCLN, KC_QUOT, KC_LSFT, RGB_VAD, RGB_VAI, _______, _______, _______, _______, DM_REC1, DM_REC2, DM_RSTP, _______, KC_RSFT, KC_LCTL, KC_LGUI, KC_NUBS, KC_LALT, _______, KC_SPC, KC_SPC, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT),

    // Raise
    [_RAISE] = LAYOUT_preonic_grid(KC_GESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, _______, _______, _______, KC_EQL, KC_DEL, KC_CAPS, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, _______, _______, KC_LBRC, _______, ALGR(KC_7), KC_TAB, ALGR(KC_0), _______, _______, _______, _______, _______, _______, KC_SCLN, KC_QUOT, KC_LSFT, ALGR(KC_8), _______, ALGR(KC_9), _______, _______, _______, _______, _______, _______, _______, KC_RSFT, KC_LCTL, KC_LGUI, KC_NUBS, KC_LALT, _______, KC_SPC, KC_SPC, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END),

    // Adjust (Lower + Raise)
    [_ADJUST] = LAYOUT_preonic_grid(KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, RESET, DEBUG, _______, _______, _______, _______, _______, KC_INSERT, _______, KC_PSCR, KC_DEL, KC_MUTE, _______, MU_MOD, AU_ON, AU_OFF, AG_NORM, AG_SWAP, QWERTZ, _______, _______, _______, _______, KC_LSFT, MUV_DE, MUV_IN, MU_ON, MU_OFF, CK_ON, CK_OFF, CK_UP, CK_DOWN, CK_RST, _______, KC_RSFT, _______, _______, _______, _______, _______, _______, _______, _______, KC_HOME, KC_PGDN, KC_PGUP, KC_END)

};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QWERTZ:
            if (record->event.pressed) {
                set_single_persistent_default_layer(_QWERTZ);
            }
            return false;
            break;
        case LOWER:
            if (record->event.pressed) {
                layer_on(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_LOWER);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
        case RAISE:
            if (record->event.pressed) {
                layer_on(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            } else {
                layer_off(_RAISE);
                update_tri_layer(_LOWER, _RAISE, _ADJUST);
            }
            return false;
            break;
    }
    return true;
};

bool     muse_mode      = false;
uint8_t  last_muse_note = 0;
uint16_t muse_counter   = 0;
uint8_t  muse_offset    = 70;
uint16_t muse_tempo     = 10;

void encoder_update_user(uint8_t index, bool clockwise) {
    if (IS_LAYER_ON(_QWERTZ)) {
        if (clockwise) {
            tap_code_delay(KC_MNXT, 10);
        } else {
            tap_code_delay(KC_MPRV, 10);
        }
    } else if (IS_LAYER_ON(_LOWER)) {
        if (clockwise) {
            tap_code(KC_RIGHT);
        } else {
            tap_code(KC_LEFT);
        }
    } else if (IS_LAYER_ON(_RAISE)) {
        if (clockwise) {
            tap_code_delay(KC_VOLU, 10);
        } else {
            tap_code_delay(KC_VOLD, 10);
        }
    } else {
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    }
}

void dip_switch_update_user(uint8_t index, bool active) {
    switch (index) {
        case 0:
            if (active) {
                layer_on(_ADJUST);
            } else {
                layer_off(_ADJUST);
            }
            break;
        case 1:
            if (active) {
                muse_mode = true;
            } else {
                muse_mode = false;
            }
    }
}

void matrix_scan_user(void) {
#ifdef AUDIO_ENABLE
    if (muse_mode) {
        if (muse_counter == 0) {
            uint8_t muse_note = muse_offset + SCALE[muse_clock_pulse()];
            if (muse_note != last_muse_note) {
                stop_note(compute_freq_for_midi_note(last_muse_note));
                play_note(compute_freq_for_midi_note(muse_note), 0xF);
                last_muse_note = muse_note;
            }
        }
        muse_counter = (muse_counter + 1) % muse_tempo;
    } else {
        if (muse_counter) {
            stop_all_notes();
            muse_counter = 0;
        }
    }
#endif
}

bool music_mask_user(uint16_t keycode) {
    switch (keycode) {
        case RAISE:
        case LOWER:
            return false;
        default:
            return true;
    }
}