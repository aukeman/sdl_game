#include <control.h>
#include <joystick.h>
#include <events.h>

#include <constants.h>

#include <math.h>

enum control_type_e {
  NO_CONTROL_MAPPING,
  ANALOG,
  BINARY
};

struct control_mapping_t{

  enum control_type_e type;

  float min_input;
  float max_input;

  union{
    struct control__analog_t* analog;
    struct control__binary_t* binary;
  };
};

struct control__state_t control_state[CONTROL__MAX_PLAYERS];

struct control_mapping_t keyboard_mappings[512];
struct control_mapping_t js_axis_mappings[JS__MAX_JOYSTICKS][JS__MAX_AXES];
struct control_mapping_t js_button_mappings[JS__MAX_JOYSTICKS][JS__MAX_BUTTONS];

void _handle_keydown(events__type_e event, 
		     const events__event_parameter_t* param, 
		     void* context);
  
void _handle_keyup(events__type_e event, 
		   const events__event_parameter_t* param, 
		   void* context);
  
void _handle_axis(events__type_e event, 
		  const events__event_parameter_t* param, 
		  void* context);
  
void _handle_button(events__type_e event, 
		    const events__event_parameter_t* param, 
		    void* context);

float _get_axis_value( float input_value, 
		       const struct control_mapping_t* control_mapping );

int control__setup(const char* mapping_file){

  events__add_callback( EVENTS__TYPE_KEYUP, _handle_keyup, NULL );
  events__add_callback( EVENTS__TYPE_KEYDOWN, _handle_keydown, NULL );

  events__add_callback( EVENTS__TYPE_JOYSTICK_AXIS, _handle_axis, NULL );
  events__add_callback( EVENTS__TYPE_JOYSTICK_BUTTON, _handle_button, NULL );

  js_axis_mappings[0][1].type = ANALOG;
  js_axis_mappings[0][1].min_input = 0.0f;
  js_axis_mappings[0][1].max_input = 1.0f;
  js_axis_mappings[0][1].analog = &control_state[0].up;
  
  js_axis_mappings[0][1].type = ANALOG;
  js_axis_mappings[0][1].min_input = 0.0f;
  js_axis_mappings[0][1].max_input = 0.0f;
  js_axis_mappings[0][1].analog = &control_state[0].up;
  

  return SUCCESS;
}

int control__teardown(){

  events__remove_callback( EVENTS__TYPE_KEYUP, _handle_keyup );
  events__remove_callback( EVENTS__TYPE_KEYDOWN, _handle_keydown );

  events__remove_callback( EVENTS__TYPE_JOYSTICK_AXIS, _handle_axis );
  events__remove_callback( EVENTS__TYPE_JOYSTICK_BUTTON, _handle_button );

  return SUCCESS;
}

const struct control__state_t* control__get_state( uint32_t player_id ){

  struct control__state_t* result = NULL;

  if ( player_id < CONTROL__MAX_PLAYERS ){
    result = &control_state[player_id];
  }

  return result;
}

void _handle_keydown(events__type_e event, 
		     const events__event_parameter_t* param, 
		     void* context){
  
  const struct control_mapping_t* mapping = &keyboard_mappings[param->key.value];

  switch (mapping->type){
  case NO_CONTROL_MAPPING:
    break;
  case ANALOG:
    mapping->analog->value = 1.0f;
    mapping->analog->timestamp = timing__get_top_of_frame();
    break;
  case BINARY:
    mapping->binary->value = TRUE;
    mapping->binary->timestamp = timing__get_top_of_frame();
    break;
  }
}

void _handle_keyup(events__type_e event, 
		   const events__event_parameter_t* param, 
		   void* context){

  const struct control_mapping_t* mapping = &keyboard_mappings[param->key.value];

  switch (mapping->type){
  case NO_CONTROL_MAPPING:
    break;
  case ANALOG:
    mapping->analog->value = 0.0f;
    mapping->analog->timestamp = timing__get_top_of_frame();
    break;
  case BINARY:
    mapping->binary->value = FALSE;
    mapping->binary->timestamp = timing__get_top_of_frame();
    break;
  }
}

void _handle_axis(events__type_e event, 
		  const events__event_parameter_t* param, 
		  void* context){

  const struct control_mapping_t* mapping = 
    &js_axis_mappings[param->js_axis.joystick_id][param->js_axis.axis_id];

  switch (mapping->type){
  case NO_CONTROL_MAPPING:
    break;
  case ANALOG:
    mapping->analog->value = _get_axis_value(param->js_axis.value, mapping);
    mapping->analog->timestamp = timing__get_top_of_frame();
    break;
 
  case BINARY:
    mapping->binary->value = (0.75 < _get_axis_value(param->js_axis.value, mapping));
    mapping->binary->timestamp = timing__get_top_of_frame();
    break;
  }
}

void _handle_button(events__type_e event, 
		    const events__event_parameter_t* param, 
		    void* context){

  const struct control_mapping_t* mapping = 
    &js_button_mappings[param->js_axis.joystick_id][param->js_button.button_id];

  switch (mapping->type){
  case NO_CONTROL_MAPPING:
    break;
  case ANALOG:
    mapping->analog->value = param->js_button.value ? 1.0f : 0.0f;
    mapping->analog->timestamp = timing__get_top_of_frame();
    break;
  case BINARY:
    mapping->binary->value = param->js_button.value;
    mapping->binary->timestamp = timing__get_top_of_frame();
    break;
  }
}

float _get_axis_value( float input_value, 
		       const struct control_mapping_t* control_mapping ){

  return fminf(1.0, 
	       fmaxf(0.0, 
		     (input_value-control_mapping->min_input) / 
		     (control_mapping->max_input-control_mapping->max_input)));
}
