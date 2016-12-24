#ifndef EVENTS_H
#define EVENTS_H

#include <constants.h>
#include <stdint.h>

typedef struct {
  uint16_t modifier;
  uint16_t value;
} events__key_param_t;

typedef struct{
  float value;
  uint8_t joystick_id;
  uint8_t axis_id;
} events__joystick_axis_param_t;

typedef struct{
  uint8_t value;
  uint8_t joystick_id;
  uint8_t button_id;
} events__joystick_button_param_t;

typedef union{
  events__key_param_t key;
  events__joystick_axis_param_t js_axis;
  events__joystick_button_param_t js_button;
} events__event_parameter_t;

enum {
  EVENTS__ERROR_CODE_BASE = 0x10,
  EVENTS__INVALID_EVENT_TYPE,
  EVENTS__CALLBACK_NOT_REGISTERED
};

typedef enum { 
  EVENTS__TYPE_NONE = 0,
  EVENTS__TYPE_QUIT,
  EVENTS__TYPE_KEYDOWN,
  EVENTS__TYPE_KEYUP,
  EVENTS__TYPE_JOYSTICK_AXIS,
  EVENTS__TYPE_JOYSTICK_BUTTON,
  EVENTS__TYPE_LAST
} events__type_e;

extern const uint32_t EVENTS__KEYMOD_LSHIFT;
extern const uint32_t EVENTS__KEYMOD_RSHIFT;
extern const uint32_t EVENTS__KEYMOD_LCTRL;
extern const uint32_t EVENTS__KEYMOD_RCTRL;
extern const uint32_t EVENTS__KEYMOD_LALT;
extern const uint32_t EVENTS__KEYMOD_RALT;
extern const uint32_t EVENTS__KEYMOD_LMETA;
extern const uint32_t EVENTS__KEYMOD_RMETA;
extern const uint32_t EVENTS__KEYMOD_NUM;
extern const uint32_t EVENTS__KEYMOD_CAPS;

extern const uint32_t EVENTS__KEY_UNKNOWN;
extern const uint32_t EVENTS__KEY_FIRST;
extern const uint32_t EVENTS__KEY_BACKSPACE;
extern const uint32_t EVENTS__KEY_TAB;
extern const uint32_t EVENTS__KEY_CLEAR;
extern const uint32_t EVENTS__KEY_RETURN;
extern const uint32_t EVENTS__KEY_PAUSE;
extern const uint32_t EVENTS__KEY_ESCAPE;
extern const uint32_t EVENTS__KEY_SPACE;
extern const uint32_t EVENTS__KEY_EXCLAIM;
extern const uint32_t EVENTS__KEY_QUOTEDBL;
extern const uint32_t EVENTS__KEY_HASH;
extern const uint32_t EVENTS__KEY_DOLLAR;
extern const uint32_t EVENTS__KEY_AMPERSAND;
extern const uint32_t EVENTS__KEY_QUOTE;
extern const uint32_t EVENTS__KEY_LEFTPAREN;
extern const uint32_t EVENTS__KEY_RIGHTPAREN;
extern const uint32_t EVENTS__KEY_ASTERISK;
extern const uint32_t EVENTS__KEY_PLUS;
extern const uint32_t EVENTS__KEY_COMMA;
extern const uint32_t EVENTS__KEY_MINUS;
extern const uint32_t EVENTS__KEY_PERIOD;
extern const uint32_t EVENTS__KEY_SLASH;
extern const uint32_t EVENTS__KEY_0;
extern const uint32_t EVENTS__KEY_1;
extern const uint32_t EVENTS__KEY_2;
extern const uint32_t EVENTS__KEY_3;
extern const uint32_t EVENTS__KEY_4;
extern const uint32_t EVENTS__KEY_5;
extern const uint32_t EVENTS__KEY_6;
extern const uint32_t EVENTS__KEY_7;
extern const uint32_t EVENTS__KEY_8;
extern const uint32_t EVENTS__KEY_9;
extern const uint32_t EVENTS__KEY_COLON;
extern const uint32_t EVENTS__KEY_SEMICOLON;
extern const uint32_t EVENTS__KEY_LESS;
extern const uint32_t EVENTS__KEY_EQUALS;
extern const uint32_t EVENTS__KEY_GREATER;
extern const uint32_t EVENTS__KEY_QUESTION;
extern const uint32_t EVENTS__KEY_AT;

extern const uint32_t EVENTS__KEY_LEFTBRACKET;
extern const uint32_t EVENTS__KEY_BACKSLASH;
extern const uint32_t EVENTS__KEY_RIGHTBRACKET;
extern const uint32_t EVENTS__KEY_CARET;
extern const uint32_t EVENTS__KEY_UNDERSCORE;
extern const uint32_t EVENTS__KEY_BACKQUOTE;
extern const uint32_t EVENTS__KEY_a;
extern const uint32_t EVENTS__KEY_b;
extern const uint32_t EVENTS__KEY_c;
extern const uint32_t EVENTS__KEY_d;
extern const uint32_t EVENTS__KEY_e;
extern const uint32_t EVENTS__KEY_f;
extern const uint32_t EVENTS__KEY_g;
extern const uint32_t EVENTS__KEY_h;
extern const uint32_t EVENTS__KEY_i;
extern const uint32_t EVENTS__KEY_j;
extern const uint32_t EVENTS__KEY_k;
extern const uint32_t EVENTS__KEY_l;
extern const uint32_t EVENTS__KEY_m;
extern const uint32_t EVENTS__KEY_n;
extern const uint32_t EVENTS__KEY_o;
extern const uint32_t EVENTS__KEY_p;
extern const uint32_t EVENTS__KEY_q;
extern const uint32_t EVENTS__KEY_r;
extern const uint32_t EVENTS__KEY_s;
extern const uint32_t EVENTS__KEY_t;
extern const uint32_t EVENTS__KEY_u;
extern const uint32_t EVENTS__KEY_v;
extern const uint32_t EVENTS__KEY_w;
extern const uint32_t EVENTS__KEY_x;
extern const uint32_t EVENTS__KEY_y;
extern const uint32_t EVENTS__KEY_z;
extern const uint32_t EVENTS__KEY_DELETE;

extern const uint32_t EVENTS__KEY_KP_0;
extern const uint32_t EVENTS__KEY_KP_1;
extern const uint32_t EVENTS__KEY_KP_2;
extern const uint32_t EVENTS__KEY_KP_3;
extern const uint32_t EVENTS__KEY_KP_4;
extern const uint32_t EVENTS__KEY_KP_5;
extern const uint32_t EVENTS__KEY_KP_6;
extern const uint32_t EVENTS__KEY_KP_7;
extern const uint32_t EVENTS__KEY_KP_8;
extern const uint32_t EVENTS__KEY_KP_9;
extern const uint32_t EVENTS__KEY_KP_PERIOD;
extern const uint32_t EVENTS__KEY_KP_DIVIDE;
extern const uint32_t EVENTS__KEY_KP_MULTIPLY;
extern const uint32_t EVENTS__KEY_KP_MINUS;
extern const uint32_t EVENTS__KEY_KP_PLUS;
extern const uint32_t EVENTS__KEY_KP_ENTER;
extern const uint32_t EVENTS__KEY_KP_EQUALS;

extern const uint32_t EVENTS__KEY_UP;
extern const uint32_t EVENTS__KEY_DOWN;
extern const uint32_t EVENTS__KEY_RIGHT;
extern const uint32_t EVENTS__KEY_LEFT;
extern const uint32_t EVENTS__KEY_INSERT;
extern const uint32_t EVENTS__KEY_HOME;
extern const uint32_t EVENTS__KEY_END;
extern const uint32_t EVENTS__KEY_PAGEUP;
extern const uint32_t EVENTS__KEY_PAGEDOWN;

extern const uint32_t EVENTS__KEY_F1;
extern const uint32_t EVENTS__KEY_F2;
extern const uint32_t EVENTS__KEY_F3;
extern const uint32_t EVENTS__KEY_F4;
extern const uint32_t EVENTS__KEY_F5;
extern const uint32_t EVENTS__KEY_F6;
extern const uint32_t EVENTS__KEY_F7;
extern const uint32_t EVENTS__KEY_F8;
extern const uint32_t EVENTS__KEY_F9;
extern const uint32_t EVENTS__KEY_F10;
extern const uint32_t EVENTS__KEY_F11;
extern const uint32_t EVENTS__KEY_F12;
extern const uint32_t EVENTS__KEY_F13;
extern const uint32_t EVENTS__KEY_F14;
extern const uint32_t EVENTS__KEY_F15;

extern const uint32_t EVENTS__KEY_NUMLOCK;
extern const uint32_t EVENTS__KEY_CAPSLOCK;
extern const uint32_t EVENTS__KEY_SCROLLOCK;
extern const uint32_t EVENTS__KEY_RSHIFT;
extern const uint32_t EVENTS__KEY_LSHIFT;
extern const uint32_t EVENTS__KEY_RCTRL;
extern const uint32_t EVENTS__KEY_LCTRL;
extern const uint32_t EVENTS__KEY_RALT;
extern const uint32_t EVENTS__KEY_LALT;
extern const uint32_t EVENTS__KEY_RMETA;
extern const uint32_t EVENTS__KEY_LMETA;
extern const uint32_t EVENTS__KEY_LWIN;
extern const uint32_t EVENTS__KEY_RWIN;
extern const uint32_t EVENTS__KEY_LAST;

extern const uint32_t EVENTS__JS_AXIS;
extern const uint32_t EVENTS__JS_BUTTON;



typedef void events__callback_fxn(events__type_e event, 
				  const events__event_parameter_t* event_parameter, 
				  void* context); 

int events__setup();
int events__teardown();

int events__process_events();

int events__add_callback( events__type_e,
			  events__callback_fxn callback, 
			  void* context );

int events__remove_callback( events__type_e,
			     events__callback_fxn callback );


int events__invoke_callback( events__type_e,
			     events__event_parameter_t* );

#endif
