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


int _map_player_controls(FILE* fin);
bool_t _map_device(FILE* fin, int player_idx);
struct control_mapping_t* _load_joystick(FILE* fin);
struct control_mapping_t* _load_keyboard(FILE* fin);

bool_t _apply_mapping( const char* control_type, int player_idx, struct control_mapping_t* mapping );

int control__setup(const char* mapping_file){

  int result = CONTROL__BAD_MAPPING_FILE;

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
      result = _map_player_controls(fin);
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

bool_t _apply_mapping( const char* control_type, int player_idx, struct control_mapping_t* mapping ){

  int result = TRUE;

  if ( !strcmp("up", control_type) ){
    mapping->type = ANALOG;
    mapping->analog = &control_state[player_idx].up;
  }
  else if ( !strcmp("down", control_type) ){
    mapping->type = ANALOG;
    mapping->analog = &control_state[player_idx].down;
  }
  else if ( !strcmp("left", control_type) ){
    mapping->type = ANALOG;
    mapping->analog = &control_state[player_idx].left;
  }
  else if ( !strcmp("right", control_type) ){
    mapping->type = ANALOG;
    mapping->analog = &control_state[player_idx].right;
  }
  else if ( !strcmp("jump", control_type) ){
    mapping->type = BINARY;
    mapping->binary = &control_state[player_idx].jump;
  }
  else if ( !strcmp("fire", control_type) ){
    mapping->type = BINARY;
    mapping->binary = &control_state[player_idx].fire;
  }
  else{
    result = FALSE;
  }

  return result;
}

int _map_player_controls(FILE* fin){
  
  int player_idx = 0;

  int result = CONTROL__BAD_MAPPING_FILE;

  if ( 1 == fscanf(fin, "%d%*c", &player_idx) ){

    fpos_t fin_pos;
    fgetpos(fin, &fin_pos);

    while ( !feof(fin) &&
	    0 == fscanf(fin, "%d%*c", &player_idx) ){

      fsetpos(fin, &fin_pos);

      if ( _map_device(fin, player_idx) ){
	result = SUCCESS;
	fgetpos(fin, &fin_pos);
      }
      else{
	result = CONTROL__BAD_MAPPING_FILE;
	fseek(fin, 0, SEEK_END);
	fgetc(fin);
      }

    }

    fsetpos(fin, &fin_pos);
  }

  if ( result != SUCCESS ){
    fseek(fin, 0, SEEK_END);
    fgetc(fin);
  }

  return result;
}

bool_t _map_device(FILE* fin, int player_idx){

  char control_type[16];
  char device_type[16];

  bool_t result = FALSE;
  struct control_mapping_t* mapping = NULL;
      
  memset(control_type, '\0', sizeof(control_type));
  memset(device_type, '\0', sizeof(device_type));

  if (2 == fscanf(fin, "%15s %15s", control_type, device_type) ){

    if ( !strcmp(device_type, "joystick") ){
      mapping = _load_joystick(fin);
    }
    else if ( !strcmp(device_type, "keyboard") ){
      mapping = _load_keyboard(fin);
    }

    if ( mapping ){
      result = _apply_mapping(control_type, player_idx, mapping);
    }
  }

  return result;
}

struct control_mapping_t* _load_joystick(FILE* fin){

  int joystick_id;
  char input_type[16];

  struct control_mapping_t* mapping = NULL;

  memset(input_type, '\0', sizeof(input_type));
  
  if ( 2 == fscanf(fin, "%d %15s", &joystick_id, input_type) ){

    if ( !strcmp("axis", input_type) ){

      int axis_id;
      float min_input, max_input;
      
      if ( 2 == fscanf(fin, "%d %f %f%*c", &axis_id, &min_input, &max_input) ){

	mapping = 
	  (struct control_mapping_t*)
	  malloc(sizeof(struct control_mapping_t));

	mapping->min_input = min_input;
	mapping->max_input = max_input;

	linked_list__add( mapping,
			  &js_axis_mappings[joystick_id][axis_id] );
      }
    }
    else if ( !strcmp("button", input_type) ){

      int button_id;
      if ( 1 == fscanf(fin, "%d%*c", &button_id) ){
      
	mapping = 
	  (struct control_mapping_t*)
	  malloc(sizeof(struct control_mapping_t));

	linked_list__add( mapping,
			  &js_button_mappings[joystick_id][button_id] );
      }
    }
  }

  return mapping;
}

struct control_mapping_t* _load_keyboard(FILE* fin){

  int key_id;

  struct control_mapping_t* mapping = NULL;

  if ( 1 == fscanf(fin, "%d%*c", &key_id) ){

    mapping = 
      (struct control_mapping_t*)
      malloc(sizeof(struct control_mapping_t));

    linked_list__add( mapping,
		      &keyboard_mappings[key_id] );

  }

  return mapping;
}

