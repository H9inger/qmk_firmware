/* Copyright 2021 Kyle McCreery
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

// Defines names for use in layer keycodes and the keymap
enum layer_names {
  _BASE,
  _FN1,
	_FN2,
	_RGB
};

//Macro definitions
enum custom_keycodes {
	LLOCK = SAFE_RANGE,
    //Got idea from here: https://getreuer.info/posts/keyboards/layer-lock/index.html
};

#include "features/layer_lock.h"

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
	if (!process_layer_lock(keycode, record, LLOCK)) { return false; }

	return true;
};


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /* Base */
    [_BASE] = LAYOUT(
                  XXXXXXX,   TG(_FN2),   TG(_RGB),   KC_MPLY,
				          KC_CALC, KC_PSLS, KC_PAST, KC_PMNS,
                  KC_7,   KC_8,   KC_9,   KC_PLUS,
        KC_ESC,  KC_4,   KC_5,   KC_6,   _______,
        KC_BTN3, KC_1,   KC_2,   KC_3,   LT(_FN1,KC_ENT),
        KC_LCTL,  KC_0,   _______, KC_DOT, _______,

				          KC_F5,   KC_F6,   KC_F7

    ),
    [_FN1] = LAYOUT(
                 C(KC_A), C(KC_C), C(KC_V), _______,
                  KC_BSPC, XXXXXXX, S(KC_SCLN), KC_DEL,
                  KC_HOME, KC_UP, KC_END, XXXXXXX,
        _______,  KC_LEFT, LLOCK, KC_RGHT, _______,
        KC_TAB,  KC_LPRN, KC_DOWN, KC_RPRN, _______,
        _______,  KC_SPC, _______, KC_LSFT, _______,

                  _______, _______, _______

    ),
    [_FN2] = LAYOUT(
                  TG(_FN2),   XXXXXXX,   XXXXXXX,   KC_MPLY,
				          KC_ESC, KC_QUOT, KC_PMNS, S(KC_QUOT),
                  KC_7,   KC_8,   KC_9,   KC_PLUS,
        KC_ESC,  KC_4,   KC_5,   KC_6,   _______,
        KC_BTN3, KC_1,   KC_2,   KC_3,   LT(_FN1,KC_ENT),
        KC_LCTL,  KC_0,   _______, KC_DOT, _______,
        
                  _______, _______, _______

    ),
	  [_RGB] = LAYOUT(
                  _______,  _______, TG(_RGB), RESET,
                 _______,  _______, _______, _______,
                 RGB_HUD,  RGB_SPI, RGB_HUI, _______,
        _______, RGB_RMOD, RGB_TOG, RGB_MOD, _______,
        _______, RGB_VAD,  RGB_SPD, RGB_VAI, _______,
        _______, RGB_SAD,  _______, RGB_SAI, _______,

                 _______,  _______, _______

    )
};

enum combo_events {
  PRD_SLASH_AM,
  PRD_ASTR_PM,
  COMBO_LENGTH
};
uint16_t COMBO_LEN = COMBO_LENGTH; // remove the COMBO_COUNT define and use this instead!

const uint16_t PROGMEM am_time[] = {KC_PLUS, KC_PSLS, COMBO_END};
const uint16_t PROGMEM pm_time[] = {KC_PLUS, KC_PAST, COMBO_END};

combo_t key_combos[] = {
  [PRD_SLASH_AM] = COMBO_ACTION(am_time),
  [PRD_ASTR_PM] = COMBO_ACTION(pm_time),
};
/* COMBO_ACTION(x) is same as COMBO(x, KC_NO) */

void process_combo_event(uint16_t combo_index, bool pressed) {
  switch(combo_index) {
    case PRD_SLASH_AM:
      if (pressed) {
        SEND_STRING("am");
      }
      break;
    case PRD_ASTR_PM:
      if (pressed) {
        SEND_STRING("pm");
      }
      break;
  }
}

#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (index) {
        case 0:
            if (clockwise) {
                tap_code(KC_WH_D);
            } else {
                tap_code(KC_WH_U);
            }
        break;
		case 1:
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        break;
    }
    return true;
}
#endif

#ifdef OLED_ENABLE
	oled_rotation_t oled_init_user(oled_rotation_t rotation) {
		return OLED_ROTATION_270;       // flips the display 270 degrees
	}

	bool oled_task_user(void) {
		oled_set_cursor(0,0);

		oled_write_ln_P(PSTR("Layer"), false);

    switch (get_highest_layer(layer_state)) {
        case _BASE:
            oled_write_ln_P(PSTR("Base"), false);
            break;
        case _FN1:
            oled_write_ln_P(PSTR("Edits"), false);
            break;
        case _FN2:
            oled_write_ln_P(PSTR("Dims"), false);
            break;
        case _RGB:
            oled_write_ln_P(PSTR("RGB"), false);
            break;
        default:
            oled_write_ln_P(PSTR("Undef"), false);
    }
	oled_write_ln_P(PSTR(""), false);
    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_ln_P(led_state.caps_lock ? PSTR("CAPS ") : PSTR("    "), false);
    oled_write_ln_P(led_state.scroll_lock ? PSTR("SCR ") : PSTR("    "), false);
    return false;
	}

    void suspend_power_down_user(void) {  // shutdown oled when powered down to prevent OLED from showing Mercutio all the time
        oled_off();
        rgblight_disable_noeeprom();
    }

    void suspend_wakeup_init_user(void) {
        rgblight_enable_noeeprom();
    }
#endif
