#include <joystick.h>
#include <events.h>

#include <string.h>
#include <stdio.h>

js__joystick_state_t js__joystick_state[JS__MAX_JOYSTICKS];

void handle_axis_event(events__type_e event, 
		       const events__event_parameter_t* param, 
		       void* context){

  int joystick_id = param->js_axis.joystick_id;
  int axis_id = param->js_axis.axis_id; 
  float value = param->js_axis.value; 

  if ( joystick_id < JS__MAX_JOYSTICKS && axis_id < JS__MAX_AXES )
  {
    js__joystick_state[joystick_id].axis[axis_id].timestamp = 0;
    js__joystick_state[joystick_id].axis[axis_id].value = value;

    fprintf(stdout, "\rvalue: %3.1f", value);
  }
}

void handle_button_event(events__type_e event, 
			 const events__event_parameter_t* param, 
			 void* context){

  int joystick_id = param->js_button.joystick_id;
  int button_id = param->js_button.button_id; 
  float value = param->js_button.value ? 1.0f : 0.0f; 

  if ( joystick_id < JS__MAX_JOYSTICKS && button_id < JS__MAX_BUTTONS )
  {
    js__joystick_state[joystick_id].button[button_id].timestamp = 0;
    js__joystick_state[joystick_id].axis[button_id].value = value;
  }
}

int js__init(){

  memset(&js__joystick_state, '\0', sizeof(js__joystick_state));

  events__set_callback( EVENTS__TYPE_JOYSTICK_AXIS, handle_axis_event, NULL );
  events__set_callback( EVENTS__TYPE_JOYSTICK_BUTTON, handle_button_event, NULL );

  return SUCCESS;
}

