#include QMK_KEYBOARD_H

// COMPILING INSTRUCTIONS: sudo make handwired/dactyl_manuform/5x7:bruce:avrdude

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.
#define _QWERTY 0
#define _NAV    1
#define _NUMPAD 2

// MACROS
enum custom_keycodes {
    THUMBS_UP = SAFE_RANGE,
    THUMBS_DOWN,
    JOY,
    ROCKET
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case THUMBS_UP:
        if (record->event.pressed) {
            // pressed
            SEND_STRING(":+1:");
        } else {
            // released
        }
        break;

    case THUMBS_DOWN:
        if (record->event.pressed) {
            // pressed
            SEND_STRING(":-1:");
        } else {
            // released
        }
        break;

    case JOY:
        if (record->event.pressed) {
            // pressed
            SEND_STRING(":joy:");
        } else {
            // released
        }
        break;

    case ROCKET:
        if (record->event.pressed) {
            // pressed
            SEND_STRING(":rocket:");
        } else {
            // released
        }
        break;
    }
    return true;
};

// TAP DANCE

typedef struct {
    bool is_press_action;
    uint8_t state;
} tap;

enum {
    SINGLE_TAP = 1,
    SINGLE_HOLD,
    DOUBLE_TAP
};

// Tap dance enums
enum {
    LSFT_CAPS,
};

uint8_t cur_dance(qk_tap_dance_state_t *state);

// For the x tap dance. Put it here so it can be used in any keymap
void x_finished(qk_tap_dance_state_t *state, void *user_data);
void x_reset(qk_tap_dance_state_t *state, void *user_data);

/* Return an integer that corresponds to what kind of tap dance should be executed.
 *
 * How to figure out tap dance state: interrupted and pressed.
 *
 * Interrupted: If the state of a dance dance is "interrupted", that means that another key has been hit
 *  under the tapping term. This is typically indicitive that you are trying to "tap" the key.
 *
 * Pressed: Whether or not the key is still being pressed. If this value is true, that means the tapping term
 *  has ended, but the key is still being pressed down. This generally means the key is being "held".
 *
 * One thing that is currenlty not possible with qmk software in regards to tap dance is to mimic the "permissive hold"
 *  feature. In general, advanced tap dances do not work well if they are used with commonly typed letters.
 *  For example "A". Tap dances are best used on non-letter keys that are not hit while typing letters.
 *
 * Good places to put an advanced tap dance:
 *  z,q,x,j,k,v,b, any function key, home/end, comma, semi-colon
 *
 * Criteria for "good placement" of a tap dance key:
 *  Not a key that is hit frequently in a sentence
 *  Not a key that is used frequently to double tap, for example 'tab' is often double tapped in a terminal, or
 *    in a web form. So 'tab' would be a poor choice for a tap dance.
 *  Letters used in common words as a double. For example 'p' in 'pepper'. If a tap dance function existed on the
 *    letter 'p', the word 'pepper' would be quite frustating to type.
 *
 * For the third point, there does exist the 'DOUBLE_SINGLE_TAP', however this is not fully tested
 *
 */
uint8_t cur_dance(qk_tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->interrupted || !state->pressed) return SINGLE_TAP;
        // Key has not been interrupted, but the key is still held. Means you want to send a 'HOLD'.
        else return SINGLE_HOLD;
    } else if (state->count == 2) {
        if (!state->interrupted && !state->pressed) return DOUBLE_TAP;
    }
    return 8; // Magic number that does nothing unless we implement an action
}

// Create an instance of 'tap' for the 'x' tap dance.
static tap xtap_state = {
    .is_press_action = true,
    .state = 0
};

void x_finished(qk_tap_dance_state_t *state, void *user_data) {
    xtap_state.state = cur_dance(state);
    switch (xtap_state.state) {
        case SINGLE_TAP: register_code(KC_LSFT); break;
        case SINGLE_HOLD: register_code(KC_LSFT); break;
        case DOUBLE_TAP: register_code(KC_CAPS); break;
    }
}
// sudo qmk flash -kb handwired/dactyl_manuform/5x7 -km default

void x_reset(qk_tap_dance_state_t *state, void *user_data) {
    switch (xtap_state.state) {
        case SINGLE_TAP: unregister_code(KC_LSFT); break;
        case SINGLE_HOLD: unregister_code(KC_LSFT); break;
        case DOUBLE_TAP: unregister_code(KC_CAPS); break;
    }
    xtap_state.state = 0;
}

qk_tap_dance_action_t tap_dance_actions[] = {
    [LSFT_CAPS] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, x_finished, x_reset)
};


// LAYOUT


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	[_QWERTY] = LAYOUT_5x7(
        // Left Hand
        KC_ESC,        KC_EXLM, KC_AT,      KC_HASH, KC_DLR,    KC_PERC,    KC_CIRC,
        KC_TAB,        KC_Q,    KC_W,       KC_E,    KC_R,      KC_T,       KC_LBRC,
        KC_LSFT,       KC_A,    KC_S,       KC_D,    KC_F,      KC_G,       KC_LPRN,
        KC_LCTL,       KC_Z,    KC_X,       KC_C,    KC_V,      KC_B,
                                THUMBS_DOWN,      KC_LALT,

                                                                KC_LGUI,        KC_ENT,
                                                                MO(_NAV),    MO(_NUMPAD),
                                                                THUMBS_DOWN,        THUMBS_UP,
        // Right Hand
                        KC_AMPR,    KC_ASTR,    KC_LPRN,    KC_RPRN,    KC_MINS, KC_EQL,    KC_GRV,
                        KC_RBRC,    KC_Y,       KC_U,       KC_I,       KC_O,    KC_P,      KC_BSLS,
                        KC_RPRN,    KC_H,       KC_J,       KC_K,       KC_L,    KC_SCLN,   KC_QUOT,
                                    KC_N,       KC_M,       KC_COMM,    KC_DOT,  KC_SLSH,   KC_SFTENT,
                                    LGUI(KC_Z),    SGUI(KC_Z),

        KC_SPC,        KC_BSPC,
        MO(_NAV),    MO(_NUMPAD),
        KC_HOME,        KC_END
    ),
	[_NAV] = LAYOUT_5x7(
        KC_TRNS, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, ROCKET, JOY, THUMBS_UP, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,

        KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_UP, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_LEFT, KC_DOWN, KC_RGHT, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS
    ),
	[_NUMPAD] = LAYOUT_5x7(
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,

        KC_TRNS, KC_TRNS, KC_NLCK, KC_TRNS, KC_PMNS, KC_PPLS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_7, KC_8, KC_9, KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS, KC_4, KC_5, KC_6, KC_PAST, KC_TRNS,
        KC_TRNS, KC_1, KC_2, KC_3, KC_PSLS, KC_TRNS,
        KC_0, KC_PDOT,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS,
        KC_TRNS, KC_TRNS
    )
};
