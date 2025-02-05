#include QMK_KEYBOARD_H

//Last edited on Oct 13, 2021

//Enables dynamic layer lighting (from https://tinyurl.com/8e6bnus2)

#ifdef RGBLIGHT_ENABLE
#include "rgblight.h"
#endif

//Defines names for use in layer keycodes and the keymap
enum layer_names {
  _BASE = 0,
  _L1,
  _L2,
  _L3,
  _L4,
};

//Macro Definitions
enum custom_keycodes {
  minim = SAFE_RANGE, //restore window down/minimize window
  os_perp,  //perpindicular o-snap
  os_node,  //node o-snap
  scale,    //scale command
  dtxt,     //d-text command
  fil,      //fillet command, uses multiple sub-command
  trim,     //trim command
  j_key,    //just inputs j_enter, used for both justification and join
  stre,     //stretch command
  era,      //erase command
  p_ln,     //p-line command
  mid,      //mid-point o-snap
  str_sft,  //press for str macro hold for shift function
	n_win,	//to open a new acrobat window and restore it down
};

/* This is a little extra that was mentioned by u/riding_qwerty
*   in this post: 
*   https://www.reddit.com/r/olkb/comments/afm9ii/qmk_macro_in_modtap_keys/
*/
uint16_t key_timer;

//Macro Declarations
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case minim:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING(SS_DOWN(X_LGUI)SS_TAP(X_DOWN)SS_UP(X_LGUI));
      } else {
        //when key with this macro assignment is released:
      }
      break;
    case os_perp:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("per" SS_TAP(X_ENT));
      } else {
      }
      break;
    case os_node:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("node" SS_TAP(X_ENT));
      } else {
      }
      break;
    case scale:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("scale" SS_TAP(X_ENT));
      } else {
      }
      break;
    case dtxt:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("dt" SS_TAP(X_ENT));
      } else {
      }
      break;
    case fil:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("fil" SS_TAP(X_ENT) SS_DELAY(200) "m" SS_TAP(X_ENT));
      } else {
      }
      break;
    case trim:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("trim" SS_TAP(X_ENT));
      } else {
      }
      break;
    case j_key:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("j" SS_TAP(X_ENT));
      } else {
      }
      break;
    case stre:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("str" SS_TAP(X_ENT));
      } else {
      }
      break;
    case era:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("era" SS_TAP(X_ENT));
      } else {
      }
      break;
    case p_ln:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("pl" SS_TAP(X_ENT));
      } else {
      }
      break;
    case mid:
      if (record->event.pressed)  {
        //when key with this macro assignment is pressed:
        SEND_STRING("mid" SS_TAP(X_ENT));
      } else {
      }
      break;
    case str_sft:
      if (record->event.pressed)  {
        key_timer = timer_read(); register_code(KC_LSFT);
      } else {
        unregister_code(KC_LSFT); if (timer_elapsed(key_timer)< TAPPING_TERM) {
        SEND_STRING("str" SS_TAP(X_ENT));}
      }
      break;
	case n_win:
		if (record->event.pressed)  {
			//when key with this macro assignment is pressed:
			SEND_STRING(SS_TAP(X_LALT) "wn" SS_DELAY (200) SS_DOWN(X_LGUI)SS_TAP(X_DOWN)SS_UP(X_LGUI));
		} else {
        		//when key with this macro assignment is released:
		}
		break;
   	}
    return true;
};

//Tap Dance Declarations
enum {
  SHIFT_LOCK = 0,
};

//Tap Dance Definitions
qk_tap_dance_action_t tap_dance_actions[] = {};

//Actual layer layout definitions

#define _______ KC_TRNS
#define XXXXXXX KC_NO

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

  [_BASE] = LAYOUT(
    KC_ESC,     KC_MS_BTN3,     scale,
    str_sft,    TT(_L1),        trim,
    dtxt,       j_key,          minim
  ),

  [_L1] = LAYOUT(
    era,        mid,            p_ln,
    os_perp,    _______,        fil,
    os_node,    TG(_L2),        TG(_L3)
  ),

  [_L2] = LAYOUT(
    KC_ESC,     XXXXXXX,        KC_WFAV,
    LT(_L4, KC_WBAK),    TG(_L2),      KC_WFWD,
    KC_LCTL,    C(KC_W),        minim
  ),

  [_L3] = LAYOUT(
    KC_ESC,     XXXXXXX,        KC_PGUP,
    n_win,    TG(_L3),      KC_PGDN,
    KC_SPC,     C(KC_0),        _______
  ),

  [_L4] = LAYOUT(
    QK_BOOT,     XXXXXXX,        RGB_VAI,
    XXXXXXX,     XXXXXXX,        RGB_VAD,
    XXXXXXX,     XXXXXXX,        RGB_TOG
  )
};

void matrix_init_user(void) {


rgblight_enable();
}

void matrix_scan_user(void) {
	#ifdef RGBLIGHT_ENABLE

	static uint8_t old_layer = 255;
	uint8_t new_layer = biton32(layer_state);

	if (old_layer != new_layer) {
		switch (new_layer) {
			case _BASE:
				rgblight_setrgb(0x00, 0x00, 0x00);
				/*for a very low lit white
				* use (0x0d, 0x0d, 0x0d)
				*/
				break;
			case _L1:
				rgblight_setrgb(0x01, 0x16, 0x18);
				//teal
				break;
			case _L2:
				rgblight_setrgb(0x31, 0x1d, 0x00);
				//orange
				break;
			case _L3:
				rgblight_setrgb(0x4d, 0x00, 0x00);
				//red
				break;
			case _L4:
				rgblight_setrgb(0x33, 0x00, 0x66);
				//purple
				break;
		}

		old_layer = new_layer;
	}

	#endif //RGBLIGHT_ENABLE
}
