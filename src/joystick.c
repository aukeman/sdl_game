#include <joystick.h>
#include <events.h>

#include <string.h>
#include <stdio.h>

#include <SDL/SDL.h>

typedef struct{ 
  js__joystick_state_t state;
  SDL_Joystick* context;
  const char* name;
} js__joystick_info_t;

js__joystick_info_t js__joystick_info[JS__MAX_JOYSTICKS];

int _setup_joystick(int idx);
int _teardown_joystick(int idx);

void handle_axis_event(events__type_e event, 
		       const events__event_parameter_t* param, 
		       void* context){

  int joystick_id = param->js_axis.joystick_id;
  int axis_id = param->js_axis.axis_id; 
  float value = param->js_axis.value; 

  if ( joystick_id < JS__MAX_JOYSTICKS && axis_id < JS__MAX_AXES )
  {
    js__joystick_info[joystick_id].state.axis[axis_id].timestamp = 0;
    js__joystick_info[joystick_id].state.axis[axis_id].value = value;
  }
}

void handle_button_event(events__type_e event, 
			 const events__event_parameter_t* param, 
			 void* context){

  int joystick_id = param->js_button.joystick_id;
  int button_id = param->js_button.button_id; 
  bool_t value = param->js_button.value;

  if ( joystick_id < JS__MAX_JOYSTICKS && button_id < JS__MAX_BUTTONS )
  {
    js__joystick_info[joystick_id].state.button[button_id].timestamp = 0;
    js__joystick_info[joystick_id].state.axis[button_id].value = value;
  }
}

int js__setup(){

  int joystick_idx = 0;
  int number_of_joysticks;

  SDL_Init(SDL_INIT_JOYSTICK);

  events__add_callback( EVENTS__TYPE_JOYSTICK_AXIS, handle_axis_event, NULL );
  events__add_callback( EVENTS__TYPE_JOYSTICK_BUTTON, handle_button_event, NULL );

  number_of_joysticks = SDL_NumJoysticks();

  for ( joystick_idx = 0; joystick_idx < number_of_joysticks; ++joystick_idx ){
    _setup_joystick(joystick_idx);
  }

  for ( ; joystick_idx < JS__MAX_JOYSTICKS; ++joystick_idx){
    _teardown_joystick(joystick_idx);
  }

  SDL_JoystickEventState(SDL_ENABLE);

  return SUCCESS;
}

int js__teardown(){
  int joystick_idx;

  for ( joystick_idx = 0; joystick_idx < JS__MAX_JOYSTICKS; ++joystick_idx ){
    _teardown_joystick(joystick_idx);
  }

  events__remove_callback( EVENTS__TYPE_JOYSTICK_AXIS, handle_axis_event );
  events__remove_callback( EVENTS__TYPE_JOYSTICK_BUTTON, handle_button_event );
  
  return SUCCESS;
}

int _setup_joystick(int idx){

  int axis_idx = 0, button_idx = 0, number_of_axes = 0, number_of_buttons = 0;

  _teardown_joystick(idx);

  js__joystick_info[idx].context = SDL_JoystickOpen(idx);
  js__joystick_info[idx].name = SDL_JoystickName(idx);

  number_of_axes = 
    SDL_JoystickNumAxes(js__joystick_info[idx].context);

  number_of_buttons = 
    SDL_JoystickNumButtons(js__joystick_info[idx].context);

  for ( axis_idx = 0; axis_idx < number_of_axes; ++axis_idx ){
    js__joystick_info[idx].state.axis[axis_idx].timestamp = 0;
    js__joystick_info[idx].state.axis[axis_idx].value = 0.0f;
    js__joystick_info[idx].state.axis[axis_idx].enabled = TRUE;
  }

  for ( button_idx = 0; button_idx < number_of_buttons; ++button_idx ){
    js__joystick_info[idx].state.button[button_idx].timestamp = 0;
    js__joystick_info[idx].state.button[button_idx].value = 0.0f;
    js__joystick_info[idx].state.button[button_idx].enabled = TRUE;
  }

  js__joystick_info[idx].state.enabled = TRUE;

  return SUCCESS;
}

int _teardown_joystick(int idx){
  if ( js__joystick_info[idx].context ){
    SDL_JoystickClose(js__joystick_info[idx].context);
  }

  memset(&js__joystick_info[idx], '\0', sizeof(js__joystick_info[idx]));

  return SUCCESS;
}

const js__joystick_state_t* js__get_joystick_state(uint32_t joystick_index){
  if ( joystick_index < JS__MAX_JOYSTICKS &&
       js__joystick_info[joystick_index].state.enabled ){
    return &js__joystick_info[joystick_index].state;
  }
  else{
    return NULL;
  }
}
