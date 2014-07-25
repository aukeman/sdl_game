#ifndef EVENTS_H
#define EVENTS_H

enum {
  EVENTS__ERROR_CODE_BASE = 0x10,
  EVENTS__INVALID_EVENT_TYPE
};

typedef void events__callback_fxn(void* context); 
enum events__type_e { 
  EVENTS__NONE = 0,
  EVENTS__ON_QUIT,
  EVENTS__LAST
};

int events__process_events();

int events__set_callback( enum events__type_e,
			  events__callback_fxn, 
			  void* context );

#endif
