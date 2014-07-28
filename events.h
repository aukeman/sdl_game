#ifndef EVENTS_H
#define EVENTS_H

#include <stdint.h>

#include <SDL/SDL.h>

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

enum {
  EVENTS__LSHIFT = (KMOD_LSHIFT << 16),
  EVENTS__RSHIFT = (KMOD_RSHIFT << 16),
  EVENTS__LCTRL = (KMOD_LCTRL << 16),
  EVENTS__RCTRL = (KMOD_RCTRL << 16),
  EVENTS__LALT = (KMOD_LALT << 16),
  EVENTS__RALT = (KMOD_RALT << 16),
  EVENTS__LMETA = (KMOD_LMETA << 16),
  EVENTS__RMETA = (KMOD_RMETA << 16),
  EVENTS__NUM = (KMOD_NUM << 16),
  EVENTS__CAPS = (KMOD_CAPS << 16)
};

typedef void events__callback_fxn(enum events__type_e event, uint32_t event_parameter, void* context); 

int events__process_events();

int events__set_callback( enum events__type_e,
			  events__callback_fxn callback, 
			  void* context );

#endif
