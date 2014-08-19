#include <control.h>
#include <joystick.h>
#include <events.h>
#include <linked_list.h>
#include <constants.h>
#include <stdio.h>
#include <string.h>

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

  int result = SUCCESS;

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

  FILE* fin = fopen(mapping_file, "r");

  if ( !fin ){
    result = CONTROL__MAPPING_FILE_NOT_FOUND;
  }
  else{

    while ( !feof(fin) ){

      int player_idx;

      int rc = fscanf(fin, "%d%*c", &player_idx);

      if ( rc != 1 ){
	result = CONTROL__BAD_MAPPING_FILE;
	break;
      }
      else{
	while ( 0 < rc ){

	  struct control_mapping_t* mapping = 
	    (struct control_mapping_t*)malloc(sizeof(struct control_mapping_t));
	  memset(mapping, '\0', sizeof(struct control_mapping_t));

	  char control_type[16];
	  char device_type[16];
	  
	  rc = fscanf(fin, "%15s %15s", control_type, device_type);

	  if ( !strncmp(device_type, "joystick", sizeof(device_type)) ){
	    int joystick_id;
	    char input_type[16];

	    rc = fscanf(fin, "%d %15s", &joystick_id, input_type);

	    if ( !strncmp(input_type, "axis", sizeof(input_type)) ){
	      int axis_id;
	      float min_input, max_input;

	      rc = fscanf(fin, "%d %f %f%*c", &axis_id, &min_input, &max_input);

	      mapping->min_input = min_input;
	      mapping->max_input = max_input;

	      linked_list__add( mapping,
				&js_axis_mappings[joystick_id][axis_id] );
	    }
	    else if ( !strncmp(input_type, "button", sizeof(input_type)) ){

	      int button_id;
	      rc = fscanf(fin, "%d%*c", &button_id);

	      linked_list__add( mapping,
				&js_button_mappings[joystick_id][button_id] );
	    }
	  }
	  else if ( !strncmp(device_type, "keyboard", sizeof(device_type) ) ){

	      int key_id;
	      rc = fscanf(fin, "%d%*c", &key_id);

	      linked_list__add( mapping,
				&keyboard_mappings[key_id] );
	  }

	  if ( !strncmp(control_type, "up", sizeof(control_type)) ){
	    mapping->type = ANALOG;
	    mapping->analog = &control_state[player_idx].up;
	  }
	  else if ( !strncmp(control_type, "down", sizeof(control_type)) ){
	    mapping->type = ANALOG;
	    mapping->analog = &control_state[player_idx].down;
	  }
	  else if ( !strncmp(control_type, "left", sizeof(control_type)) ){
	    mapping->type = ANALOG;
	    mapping->analog = &control_state[player_idx].left;
	  }
	  else if ( !strncmp(control_type, "right", sizeof(control_type)) ){
	    mapping->type = ANALOG;
	    mapping->analog = &control_state[player_idx].right;
	  }
	  else if ( !strncmp(control_type, "jump", sizeof(control_type)) ){
	    mapping->type = BINARY;
	    mapping->binary = &control_state[player_idx].jump;
	  }
	  else if ( !strncmp(control_type, "fire", sizeof(control_type)) ){
	    mapping->type = BINARY;
	    mapping->binary = &control_state[player_idx].fire;
	  }
	}
      }
    }

    fclose(fin);
  }

  return result;
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
