#include <events.h>

#include <constants.h>

#include <SDL/SDL.h>

struct events__callback_record_t {
  events__callback_fxn* callback;
  void* context;
};

struct events__callback_record_t events__callbacks[EVENTS__LAST] = {
  {NULL, NULL},
  {NULL, NULL}
};

int events__process_events() {

  SDL_Event event;
  while ( SDL_PollEvent(&event)) {

    enum events__type_e event_type = EVENTS__NONE;

    switch ( event.type ) {
    case SDL_KEYDOWN:
      if ( event.key.keysym.sym == SDLK_ESCAPE ) {
	event_type = EVENTS__ON_QUIT;
      }
      break;

    case SDL_QUIT:      
      event_type = EVENTS__ON_QUIT;
      break;
    }

    events__callback_fxn* callback = events__callbacks[event_type].callback;
    void* context = events__callbacks[event_type].context;

    if ( callback != NULL ) {
      (*callback)(context);
    }
  }
}

int events__set_callback( enum events__type_e event_type,
			  events__callback_fxn callback, 
			  void* context ) {

  if ( EVENTS__NONE < event_type &&
       event_type < EVENTS__LAST ) {

    events__callbacks[event_type].callback = callback;
    events__callbacks[event_type].context = context;

    return SUCCESS;
  }
  else {
    return EVENTS__INVALID_EVENT_TYPE;
  }

}
