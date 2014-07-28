#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

enum {
  EVENTS__ERROR_CODE_BASE = 0x10,
  EVENTS__INVALID_EVENT_TYPE
};

enum {
  EVENTS__MODIFIER_MASK = 0xFFFF0000,
  EVENTS__VALUE_MASK = 0x0000FFFF
};


enum events__type_e { 
  EVENTS__NONE = 0,
  EVENTS__QUIT,
  EVENTS__KEYDOWN,
  EVENTS__KEYUP,
  EVENTS__LAST
};

typedef void events__callback_fxn(enum events__type_e event, uint32_t event_parameter, void* context); 

int events__process_events();

int events__set_callback( enum events__type_e,
			  events__callback_fxn callback, 
			  void* context );

#endif
