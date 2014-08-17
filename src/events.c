#include <events.h>

#include <constants.h>

#include <SDL/SDL.h>

const uint32_t EVENTS__KEYMOD_LSHIFT = KMOD_LSHIFT;
const uint32_t EVENTS__KEYMOD_RSHIFT = KMOD_RSHIFT;
const uint32_t EVENTS__KEYMOD_LCTRL = KMOD_LCTRL;
const uint32_t EVENTS__KEYMOD_RCTRL = KMOD_RCTRL;
const uint32_t EVENTS__KEYMOD_LALT = KMOD_LALT;
const uint32_t EVENTS__KEYMOD_RALT = KMOD_RALT;
const uint32_t EVENTS__KEYMOD_LMETA = KMOD_LMETA;
const uint32_t EVENTS__KEYMOD_RMETA = KMOD_RMETA;
const uint32_t EVENTS__KEYMOD_NUM = KMOD_NUM;
const uint32_t EVENTS__KEYMOD_CAPS = KMOD_CAPS;

const uint32_t EVENTS__KEY_UNKNOWN = SDLK_UNKNOWN;
const uint32_t EVENTS__KEY_FIRST = SDLK_FIRST;
const uint32_t EVENTS__KEY_BACKSPACE = SDLK_BACKSPACE;
const uint32_t EVENTS__KEY_TAB = SDLK_TAB;
const uint32_t EVENTS__KEY_CLEAR = SDLK_CLEAR;
const uint32_t EVENTS__KEY_RETURN = SDLK_RETURN;
const uint32_t EVENTS__KEY_PAUSE = SDLK_PAUSE;
const uint32_t EVENTS__KEY_ESCAPE = SDLK_ESCAPE;
const uint32_t EVENTS__KEY_SPACE = SDLK_SPACE;
const uint32_t EVENTS__KEY_EXCLAIM = SDLK_EXCLAIM;
const uint32_t EVENTS__KEY_QUOTEDBL = SDLK_QUOTEDBL;
const uint32_t EVENTS__KEY_HASH = SDLK_HASH;
const uint32_t EVENTS__KEY_DOLLAR = SDLK_DOLLAR;
const uint32_t EVENTS__KEY_AMPERSAND = SDLK_AMPERSAND;
const uint32_t EVENTS__KEY_QUOTE = SDLK_QUOTE;
const uint32_t EVENTS__KEY_LEFTPAREN = SDLK_LEFTPAREN;
const uint32_t EVENTS__KEY_RIGHTPAREN = SDLK_RIGHTPAREN;
const uint32_t EVENTS__KEY_ASTERISK = SDLK_ASTERISK;
const uint32_t EVENTS__KEY_PLUS = SDLK_PLUS;
const uint32_t EVENTS__KEY_COMMA = SDLK_COMMA;
const uint32_t EVENTS__KEY_MINUS = SDLK_MINUS;
const uint32_t EVENTS__KEY_PERIOD = SDLK_PERIOD;
const uint32_t EVENTS__KEY_SLASH = SDLK_SLASH;
const uint32_t EVENTS__KEY_0 = SDLK_0;
const uint32_t EVENTS__KEY_1 = SDLK_1;
const uint32_t EVENTS__KEY_2 = SDLK_2;
const uint32_t EVENTS__KEY_3 = SDLK_3;
const uint32_t EVENTS__KEY_4 = SDLK_4;
const uint32_t EVENTS__KEY_5 = SDLK_5;
const uint32_t EVENTS__KEY_6 = SDLK_6;
const uint32_t EVENTS__KEY_7 = SDLK_7;
const uint32_t EVENTS__KEY_8 = SDLK_8;
const uint32_t EVENTS__KEY_9 = SDLK_9;
const uint32_t EVENTS__KEY_COLON = SDLK_COLON;
const uint32_t EVENTS__KEY_SEMICOLON = SDLK_SEMICOLON;
const uint32_t EVENTS__KEY_LESS = SDLK_LESS;
const uint32_t EVENTS__KEY_EQUALS = SDLK_EQUALS;
const uint32_t EVENTS__KEY_GREATER = SDLK_GREATER;
const uint32_t EVENTS__KEY_QUESTION = SDLK_QUESTION;
const uint32_t EVENTS__KEY_AT = SDLK_AT;

const uint32_t EVENTS__KEY_LEFTBRACKET = SDLK_LEFTBRACKET;
const uint32_t EVENTS__KEY_BACKSLASH = SDLK_BACKSLASH;
const uint32_t EVENTS__KEY_RIGHTBRACKET = SDLK_RIGHTBRACKET;
const uint32_t EVENTS__KEY_CARET = SDLK_CARET;
const uint32_t EVENTS__KEY_UNDERSCORE = SDLK_UNDERSCORE;
const uint32_t EVENTS__KEY_BACKQUOTE = SDLK_BACKQUOTE;
const uint32_t EVENTS__KEY_a = SDLK_a;
const uint32_t EVENTS__KEY_b = SDLK_b;
const uint32_t EVENTS__KEY_c = SDLK_c;
const uint32_t EVENTS__KEY_d = SDLK_d;
const uint32_t EVENTS__KEY_e = SDLK_e;
const uint32_t EVENTS__KEY_f = SDLK_f;
const uint32_t EVENTS__KEY_g = SDLK_g;
const uint32_t EVENTS__KEY_h = SDLK_h;
const uint32_t EVENTS__KEY_i = SDLK_i;
const uint32_t EVENTS__KEY_j = SDLK_j;
const uint32_t EVENTS__KEY_k = SDLK_k;
const uint32_t EVENTS__KEY_l = SDLK_l;
const uint32_t EVENTS__KEY_m = SDLK_m;
const uint32_t EVENTS__KEY_n = SDLK_n;
const uint32_t EVENTS__KEY_o = SDLK_o;
const uint32_t EVENTS__KEY_p = SDLK_p;
const uint32_t EVENTS__KEY_q = SDLK_q;
const uint32_t EVENTS__KEY_r = SDLK_r;
const uint32_t EVENTS__KEY_s = SDLK_s;
const uint32_t EVENTS__KEY_t = SDLK_t;
const uint32_t EVENTS__KEY_u = SDLK_u;
const uint32_t EVENTS__KEY_v = SDLK_v;
const uint32_t EVENTS__KEY_w = SDLK_w;
const uint32_t EVENTS__KEY_x = SDLK_x;
const uint32_t EVENTS__KEY_y = SDLK_y;
const uint32_t EVENTS__KEY_z = SDLK_z;
const uint32_t EVENTS__KEY_DELETE = SDLK_DELETE;

const uint32_t EVENTS__KEY_KP_0 = SDLK_KP0;
const uint32_t EVENTS__KEY_KP_1 = SDLK_KP1;
const uint32_t EVENTS__KEY_KP_2 = SDLK_KP2;
const uint32_t EVENTS__KEY_KP_3 = SDLK_KP3;
const uint32_t EVENTS__KEY_KP_4 = SDLK_KP4;
const uint32_t EVENTS__KEY_KP_5 = SDLK_KP5;
const uint32_t EVENTS__KEY_KP_6 = SDLK_KP6;
const uint32_t EVENTS__KEY_KP_7 = SDLK_KP7;
const uint32_t EVENTS__KEY_KP_8 = SDLK_KP8;
const uint32_t EVENTS__KEY_KP_9 = SDLK_KP9;
const uint32_t EVENTS__KEY_KP_PERIOD = SDLK_KP_PERIOD;
const uint32_t EVENTS__KEY_KP_DIVIDE = SDLK_KP_DIVIDE;
const uint32_t EVENTS__KEY_KP_MULTIPLY = SDLK_KP_MULTIPLY;
const uint32_t EVENTS__KEY_KP_MINUS = SDLK_KP_MINUS;
const uint32_t EVENTS__KEY_KP_PLUS = SDLK_KP_PLUS;
const uint32_t EVENTS__KEY_KP_ENTER = SDLK_KP_ENTER;
const uint32_t EVENTS__KEY_KP_EQUALS = SDLK_KP_EQUALS;

const uint32_t EVENTS__KEY_UP = SDLK_UP;
const uint32_t EVENTS__KEY_DOWN = SDLK_DOWN;
const uint32_t EVENTS__KEY_RIGHT = SDLK_RIGHT;
const uint32_t EVENTS__KEY_LEFT = SDLK_LEFT;
const uint32_t EVENTS__KEY_INSERT = SDLK_INSERT;
const uint32_t EVENTS__KEY_HOME = SDLK_HOME;
const uint32_t EVENTS__KEY_END = SDLK_END;
const uint32_t EVENTS__KEY_PAGEUP = SDLK_PAGEUP;
const uint32_t EVENTS__KEY_PAGEDOWN = SDLK_PAGEDOWN;

const uint32_t EVENTS__KEY_F1 = SDLK_F1;
const uint32_t EVENTS__KEY_F2 = SDLK_F2;
const uint32_t EVENTS__KEY_F3 = SDLK_F3;
const uint32_t EVENTS__KEY_F4 = SDLK_F4;
const uint32_t EVENTS__KEY_F5 = SDLK_F5;
const uint32_t EVENTS__KEY_F6 = SDLK_F6;
const uint32_t EVENTS__KEY_F7 = SDLK_F7;
const uint32_t EVENTS__KEY_F8 = SDLK_F8;
const uint32_t EVENTS__KEY_F9 = SDLK_F9;
const uint32_t EVENTS__KEY_F10 = SDLK_F10;
const uint32_t EVENTS__KEY_F11 = SDLK_F11;
const uint32_t EVENTS__KEY_F12 = SDLK_F12;
const uint32_t EVENTS__KEY_F13 = SDLK_F13;
const uint32_t EVENTS__KEY_F14 = SDLK_F14;
const uint32_t EVENTS__KEY_F15 = SDLK_F15;

const uint32_t EVENTS__KEY_NUMLOCK = SDLK_NUMLOCK;
const uint32_t EVENTS__KEY_CAPSLOCK = SDLK_CAPSLOCK;
const uint32_t EVENTS__KEY_SCROLLOCK = SDLK_SCROLLOCK;
const uint32_t EVENTS__KEY_RSHIFT = SDLK_RSHIFT;
const uint32_t EVENTS__KEY_LSHIFT = SDLK_LSHIFT;
const uint32_t EVENTS__KEY_RCTRL = SDLK_RCTRL;
const uint32_t EVENTS__KEY_LCTRL = SDLK_LCTRL;
const uint32_t EVENTS__KEY_RALT = SDLK_RALT;
const uint32_t EVENTS__KEY_LALT = SDLK_LALT;
const uint32_t EVENTS__KEY_RMETA = SDLK_RMETA;
const uint32_t EVENTS__KEY_LMETA = SDLK_LMETA;
const uint32_t EVENTS__KEY_LWIN = SDLK_LSUPER;
const uint32_t EVENTS__KEY_RWIN = SDLK_RSUPER;


struct  events__callback_record_t{
  events__callback_fxn* callback;
  void* context;
};

struct events__callback_node_t{
  struct events__callback_record_t record;
  struct events__callback_node_t* next;
};

struct events__callback_node_t* events__callbacks[EVENTS__TYPE_LAST];

int events__setup(){
  memset(events__callbacks, '\0', sizeof(events__callbacks));

  return SUCCESS;
}

int events__teardown(){

  int idx;

  for ( idx = 0; 
	idx < EVENTS__TYPE_LAST; 
	++idx ){
    
    struct events__callback_node_t* current_node = events__callbacks[idx];
    while ( current_node ){

      struct events__callback_node_t* node_to_free = current_node;
      current_node = current_node->next;

      free(node_to_free);
    }

    events__callbacks[idx] = NULL;
  }

  return SUCCESS;
}


int events__process_events() {

  SDL_Event event;
  while ( SDL_PollEvent(&event)) {

    events__type_e event_type = EVENTS__TYPE_NONE;
    events__event_parameter_t param;
    events__event_parameter_t* param_ptr = NULL;

    switch ( event.type ) {
    case SDL_KEYDOWN:
      event_type = EVENTS__TYPE_KEYDOWN;

      param.key.value = event.key.keysym.sym;
      param.key.modifier = event.key.keysym.mod;

      param_ptr = &param;
      break;

    case SDL_KEYUP:
      event_type = EVENTS__TYPE_KEYUP;
      param.key.value = event.key.keysym.sym;
      param.key.modifier = event.key.keysym.mod;

      param_ptr = &param;
      break;

    case SDL_JOYAXISMOTION:
      event_type = EVENTS__TYPE_JOYSTICK_AXIS;
      param.js_axis.joystick_id = event.jaxis.which;
      param.js_axis.axis_id = event.jaxis.axis;
      param.js_axis.value = (float)(event.jaxis.value) / (int16_t)0x7FFF;

      param.js_axis.value = 
	(param.js_axis.value < -1.0f ? -1.0f :
	 (1.0f < param.js_axis.value ? 1.0f : param.js_axis.value));
	
      param_ptr = &param;
      break;

    case SDL_JOYBUTTONDOWN:
    case SDL_JOYBUTTONUP:
      event_type = EVENTS__TYPE_JOYSTICK_BUTTON;
      param.js_button.joystick_id = event.jbutton.which;
      param.js_button.button_id = event.jbutton.button;
      param.js_button.value = (event.jbutton.state == SDL_PRESSED);

      param_ptr = &param;
      
      break;

    case SDL_QUIT:      
      event_type = EVENTS__TYPE_QUIT;
      param_ptr = NULL;
      break;
    }

    _invoke_callback(event_type, param_ptr);

  }
}

int events__add_callback( events__type_e event_type,
			  events__callback_fxn callback, 
			  void* context ) {

  if ( EVENTS__TYPE_NONE < event_type &&
       event_type < EVENTS__TYPE_LAST ) {

    struct events__callback_node_t** current_node_ptr = &events__callbacks[event_type];

    while ( *current_node_ptr != NULL ){
      current_node_ptr = &((*current_node_ptr)->next);
    }

    *current_node_ptr = 
      (struct events__callback_node_t*)
      malloc(sizeof(struct events__callback_node_t));

    (*current_node_ptr)->record.callback = callback;
    (*current_node_ptr)->record.context = context;
    (*current_node_ptr)->next = NULL;

    return SUCCESS;
  }
  else {
    return EVENTS__INVALID_EVENT_TYPE;
  }
}

int events__remove_callback( events__type_e event_type,
			     events__callback_fxn callback ) {

  int result = EVENTS__CALLBACK_NOT_REGISTERED;

  if ( EVENTS__TYPE_NONE < event_type &&
       event_type < EVENTS__TYPE_LAST ) {

    struct events__callback_node_t** current_node_ptr = &events__callbacks[event_type];
    struct events__callback_node_t* next_node = NULL;
  
    while ( *current_node_ptr != NULL &&
	    (*current_node_ptr)->record.callback != callback ){
      current_node_ptr = &((*current_node_ptr)->next);
    }
  
    if ( *current_node_ptr ){
      next_node = (*current_node_ptr)->next;
    
      free(*current_node_ptr);
      *current_node_ptr = next_node;

      result = SUCCESS;
    }
  
  }
  else{
    result = EVENTS__INVALID_EVENT_TYPE;
  }

  return result;
}

int _invoke_callback( events__type_e event_type,
		      events__event_parameter_t* event_param ){

  int result = EVENTS__CALLBACK_NOT_REGISTERED;

  if ( EVENTS__TYPE_NONE < event_type &&
       event_type < EVENTS__TYPE_LAST ) {

    struct events__callback_node_t* current_node = events__callbacks[event_type];
    while ( current_node ){
      events__callback_fxn* callback = current_node->record.callback;
      void* context = current_node->record.context;

      if ( callback != NULL ) {
	(*callback)(event_type, event_param, context);
      }

      current_node = current_node->next;

      result = SUCCESS;
    }
  }
  else{
    result = EVENTS__INVALID_EVENT_TYPE;
  }

  return result;
}
