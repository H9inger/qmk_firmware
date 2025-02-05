/*
Copyright 2020 Jonathon Carstens jonathon@lizardtrick.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H

// Defines names for use in layer keycodes and the keymap
enum layer_names {
    _BASE = 0,
    _L1,
    _L2,
    _L3,
};

//Macro definitions
enum custom_keycodes {
	pcentx2 = SAFE_RANGE,
	LWRx,
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
	switch (keycode) {
	case pcentx2:
		if (record->event.pressed) {
			// when keycode percent percent is pressed
			SEND_STRING("%%");
		} else {
			// when keycode percent percent is released
		}
		break;
	case LWRx:
		if (record->event.pressed) {
			// when keycode lowercase x is pressed
			SEND_STRING(SS_LSFT("x"));
		} else {
		}
		break;
	}
	return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
     *
     *  Left    Middle  Right
     *  VolDn   PgDn    Alt+Tab    (Rotary Counterclockwise)
     *  VolUp   PgUp    Tab        (Rotary Clockwise)
     *  Mute    Play    Next       (Rotary Click)
     *
     *
     *
     *   ┌───┬───┬───┬───┐
     *   │TG1│ / │ * │ - │
     *   ├───┼───┼───┼───┤
     *   │ 7 │ 8 │ 9 │   │
     *   ├───┼───┼───┤ + │
     *   │ 4 │ 5 │ 6 │   │
     *   ├───┼───┼───┼───┤
     *   │ 1 │ 2 │ 3 │   │
     *   ├───┴───┼───┤Ent│
     *   │   0   │ . │   │
     *   └───────┴───┴───┘
     */

    [_BASE] = LAYOUT(
        KC_BTN3,  TG(_L2),  TG(_L3),
        S(KC_QUOT),    KC_SLSH,  KC_QUOT,  KC_MINS,
        KC_7,     KC_8,     KC_9,     pcentx2,
        KC_4,     KC_5,     KC_6,
        KC_1,     KC_2,     KC_3,     LT(_L1,KC_ENT),
        KC_0,               KC_DOT
    ),

    /*
     *
     * ┌─────────┬─────────┬─────────┬─────────┐
     * │   TG1   │    /    │    *    │    -    │
     * ├─────────┼─────────┼─────────┼─────────┤
     * │  Alt 7  │  Alt 8  │  Alt 9  │         │
     * ├─────────┼─────────┼─────────┤    +    │
     * │  Alt 4  │  Alt 5  │  Alt 6  │         │
     * ├─────────┼─────────┼─────────┼─────────┤
     * │  Alt 1  │  Alt 2  │  Alt 3  │         │
     * ├─────────┴─────────┼─────────┤   Ent   │
     * │      Escape       │   Del   │         │
     * └───────────────────┴─────────┴─────────┘
     */

    [_L1] = LAYOUT(
        _______,        _______,        _______,
        KC_TAB,       LSFT(KC_6),     XXXXXXX,    KC_DEL,
        KC_HOME,     KC_UP,     KC_END,   KC_BSPC,
        KC_LEFT,     XXXXXXX,     KC_RIGHT,
        KC_PGUP,     KC_DOWN,   KC_PGDN,   _______,
        LWRx,                 KC_SPC
    ),

	[_L2] = LAYOUT(
		_______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,			_______
	),


	[_L3] = LAYOUT(
		_______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,        _______,        _______,
		_______,        _______,        _______,        _______,
		_______,			_______
	)
};


// used guide here: https://tinyurl.com/y3spe8zw under advanced heading

bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {           /* Left Encoder */
		if (clockwise) {
			tap_code16(KC_WH_U);
		} else {
			tap_code16(KC_WH_D);
		}

	} else if (index == 1) {    /* Middle Encoder */
		if (IS_LAYER_ON(_L2)) {
			if (clockwise) {
				tap_code16(LCTL(KC_TAB));
			} else {
				tap_code16(RCS(KC_TAB));
			}
		} else {	//on other layers besides what is specified, do this:
			if (clockwise) {
				tap_code16(KC_PGDN);
			} else {
				tap_code16(KC_PGUP);
			}
		}

	} else if (index == 2) {    /* Right Encoder */
		if (IS_LAYER_ON(_L3)) {
			if (clockwise) {
				tap_code16(KC_DOWN);
			} else {
				tap_code16(KC_UP);
			}
		} else {	//on other layers besides what is specified, do this:
			if (clockwise) {
				tap_code16(KC_RGHT);
			} else {
				tap_code16(KC_LEFT);
			}
		}
	}
	return true;
}