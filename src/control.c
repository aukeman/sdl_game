#include <control.h>
#include <joystick.h>
#include <events.h>
#include <linked_list.h>
#include <constants.h>

#include <math.h>
#include <stdlib.h>

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

struct linked_list_t keyboard_mappings[512];
struct linked_list_t js_axis_mappings[JS__MAX_JOYSTICKS][JS__MAX_AXES];
struct linked_list_t js_button_mappings[JS__MAX_JOYSTICKS][JS__MAX_BUTTONS];

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

  int idx1, idx2;
  for (idx1 = 0; idx1 < 512; ++idx1){
    linked_list__setup( &keyboard_mappings[512] );
  }

  for (idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1){
    for (idx2 = 0; idx2 < JS__MAX_AXES; ++idx2){
      linked_list__setup( &js_axis_mappings[idx1][idx2] );
    }

    for (idx2 = 0; idx2 < JS__MAX_BUTTONS; ++idx2){
      linked_list__setup( &js_button_mappings[idx1][idx2] );
    }
  }

  struct control_mapping_t* up_mapping = 
    (struct control_mapping_t*)malloc(sizeof(struct control_mapping_t));

  up_mapping->type = ANALOG;
  up_mapping->min_input = 0.0f;
  up_mapping->max_input = -1.0f;
  up_mapping->analog = &control_state[0].up;

  struct control_mapping_t* down_mapping = 
    (struct control_mapping_t*)malloc(sizeof(struct control_mapping_t));

  down_mapping->type = ANALOG;
  down_mapping->min_input = 0.0f;
  down_mapping->max_input = 1.0f;
  down_mapping->analog = &control_state[0].down;

  struct control_mapping_t* left_mapping = 
    (struct control_mapping_t*)malloc(sizeof(struct control_mapping_t));

  left_mapping->type = ANALOG;
  left_mapping->min_input = 0.0f;
  left_mapping->max_input = -1.0f;
  left_mapping->analog = &control_state[0].left;

  struct control_mapping_t* right_mapping = 
    (struct control_mapping_t*)malloc(sizeof(struct control_mapping_t));

  right_mapping->type = ANALOG;
  right_mapping->min_input = 0.0f;
  right_mapping->max_input = 1.0f;
  right_mapping->analog = &control_state[0].right;

  linked_list__add( up_mapping, &js_axis_mappings[0][1] );
  linked_list__add( down_mapping, &js_axis_mappings[0][1] );
  
  linked_list__add( left_mapping, &js_axis_mappings[0][0] );
  linked_list__add( right_mapping, &js_axis_mappings[0][0] );

  return SUCCESS;
}

int control__teardown(){

  events__remove_callback( EVENTS__TYPE_KEYUP, _handle_keyup );
  events__remove_callback( EVENTS__TYPE_KEYDOWN, _handle_keydown );

  events__remove_callback( EVENTS__TYPE_JOYSTICK_AXIS, _handle_axis );
  events__remove_callback( EVENTS__TYPE_JOYSTICK_BUTTON, _handle_button );

  int idx1, idx2;
  for (idx1 = 0; idx1 < 512; ++idx1){
    linked_list__teardown( &keyboard_mappings[512], TRUE );
  }

  for (idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1){
    for (idx2 = 0; idx2 < JS__MAX_AXES; ++idx2){
      linked_list__teardown( &js_axis_mappings[idx1][idx2], TRUE );
    }

    for (idx2 = 0; idx2 < JS__MAX_BUTTONS; ++idx2){
      linked_list__teardown( &js_button_mappings[idx1][idx2], TRUE );
    }
  }

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
  
  const struct control_mapping_t* mapping =
    (const struct control_mapping_t*) 
    linked_list__begin(&keyboard_mappings[param->key.value]);

  while ( mapping ){

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
    
    mapping = 
      (const struct control_mapping_t*)linked_list__next();
  }
}

void _handle_keyup(events__type_e event, 
		   const events__event_parameter_t* param, 
		   void* context){

  const struct control_mapping_t* mapping =
    (const struct control_mapping_t*) 
    linked_list__begin(&keyboard_mappings[param->key.value]);

  while ( mapping ){

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

    mapping = 
      (const struct control_mapping_t*)linked_list__next();
  }
}

void _handle_axis(events__type_e event, 
		  const events__event_parameter_t* param, 
		  void* context){

  const struct control_mapping_t* mapping =
    (const struct control_mapping_t*) 
    linked_list__begin(&js_axis_mappings[param->js_axis.joystick_id][param->js_axis.axis_id]);

  while ( mapping ){

    float axis_value = _get_axis_value(param->js_axis.value, mapping);
    
    switch (mapping->type){
    case NO_CONTROL_MAPPING:
      break;
    case ANALOG:
      if ( mapping->analog->value != axis_value ){
	mapping->analog->value = axis_value;
	mapping->analog->timestamp = timing__get_top_of_frame();
      }
      break;
 
    case BINARY:
      if ( mapping->binary->value != (0.75f < axis_value) ){
	mapping->binary->value = (0.75f < axis_value);
	mapping->binary->timestamp = timing__get_top_of_frame();
      }
      break;
    }

    mapping = 
      (const struct control_mapping_t*)linked_list__next();
  }
}

void _handle_button(events__type_e event, 
		    const events__event_parameter_t* param, 
		    void* context){

  const struct control_mapping_t* mapping =
    (const struct control_mapping_t*) 
    linked_list__begin(&js_button_mappings[param->js_button.joystick_id][param->js_button.button_id]);

  while ( mapping ){

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

    mapping = 
      (const struct control_mapping_t*)linked_list__next();
  }
}

float _get_axis_value( float input_value, 
		       const struct control_mapping_t* control_mapping ){

  return fminf(1.0, 
	       fmaxf(0.0, 
		     (input_value-control_mapping->min_input) / 
		     (control_mapping->max_input-control_mapping->min_input)));
}
