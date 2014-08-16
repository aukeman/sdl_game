#include <control.h>
#include <joystick.h>

#include <constants.h>

#include <math.h>

enum control_type_e{
  JOYSTICK_AXIS,
  JOYSTICK_BUTTON,
  KEYBOARD_BUTTON
};

struct joystick_axis_mapping_t{
  uint32_t joystick_id;
  uint32_t axis_id;

  float min_input;
  float max_input;
};

struct joystick_button_mapping_t{
  uint32_t joystick_id;
  uint32_t button_id;
};

struct keyboard_button_mapping_t{
  uint32_t button_id;
};

struct control_mapping_t{
  enum control_type_e type;
  union{
    struct joystick_axis_mapping_t js_axis;
    struct joystick_button_mapping_t js_button;
    struct keyboard_button_mapping_t key_button;
  };
};

struct player_mapping_t{
  struct control_mapping_t up;
  struct control_mapping_t down;
  struct control_mapping_t left;
  struct control_mapping_t right;
  struct control_mapping_t jump;
  struct control_mapping_t fire;
};

struct player_mapping_t player_map[CONTROL__MAX_PLAYERS];

int control__setup(const char* mapping_file){
  int result = SUCCESS;

  player_map[0].up.type = JOYSTICK_AXIS;
  player_map[0].up.js_axis.joystick_id = 0;
  player_map[0].up.js_axis.axis_id = 1;
  player_map[0].up.js_axis.min_input = 0.0;
  player_map[0].up.js_axis.max_input = -1.0;

  player_map[0].down.type = JOYSTICK_AXIS;
  player_map[0].down.js_axis.joystick_id = 0;
  player_map[0].down.js_axis.axis_id = 1;
  player_map[0].down.js_axis.min_input = 0.0;
  player_map[0].down.js_axis.max_input = 1.0;

  player_map[0].left.type = JOYSTICK_AXIS;
  player_map[0].left.js_axis.joystick_id = 0;
  player_map[0].left.js_axis.axis_id = 0;
  player_map[0].left.js_axis.min_input = 0.0;
  player_map[0].left.js_axis.max_input = -1.0;

  player_map[0].right.type = JOYSTICK_AXIS;
  player_map[0].right.js_axis.joystick_id = 0;
  player_map[0].right.js_axis.axis_id = 0;
  player_map[0].right.js_axis.min_input = 0.0;
  player_map[0].right.js_axis.max_input = 1.0;

  player_map[0].jump.type = JOYSTICK_BUTTON;
  player_map[0].jump.js_button.joystick_id = 0;
  player_map[0].jump.js_button.button_id = 0;

  player_map[0].fire.type = JOYSTICK_AXIS;
  player_map[0].fire.js_button.joystick_id = 0;
  player_map[0].fire.js_button.button_id = 1;

  return result;
}


int control__get_state( uint32_t player_id, 
			struct control__state_t* control_state ){

  int result = SUCCESS;

  if ( CONTROL__MAX_PLAYERS <= player_id ){
    result = CONTROL__NO_SUCH_PLAYER;
  }
  else{

    _get_analog_control(&player_map[player_id].up, &control_state->up);
    _get_analog_control(&player_map[player_id].down, &control_state->down);
    _get_analog_control(&player_map[player_id].left, &control_state->left);
    _get_analog_control(&player_map[player_id].right, &control_state->right);

    _get_switch_control(&player_map[player_id].jump, &control_state->jump);
    _get_switch_control(&player_map[player_id].fire, &control_state->fire);
  }

  return result;
}

int _get_analog_control( const struct control_mapping_t* mapping, 
			 struct control__analog_t* control ){

  int result;
  struct control__switch_t switch_control;

  switch (mapping->type){
  case JOYSTICK_AXIS:
    result = _get_joystick_axis_control( &mapping->js_axis, control );
    break;
  case JOYSTICK_BUTTON:
    result = _get_joystick_button_control( &mapping->js_button, &switch_control );
    control->value = switch_control.value ? 1.0f : 0.0f;
    control->timestamp = switch_control.timestamp;
    break;
  case KEYBOARD_BUTTON:
    result = _get_keyboard_button_control( &mapping->key_button, &switch_control );
    control->value = switch_control.value ? 1.0f : 0.0f;
    control->timestamp = switch_control.timestamp;
    break;
  }

  return result;
}

bool_t _get_switch_control( const struct control_mapping_t* mapping, 
			    struct control__switch_t* control ){

  int result;
  struct control__analog_t analog_control;

  switch (mapping->type){
  case JOYSTICK_AXIS:
    result = _get_joystick_axis_control( &mapping->js_axis, &analog_control );
    control->value = 0.75f < analog_control.value ? TRUE : FALSE;
    control->timestamp = analog_control.timestamp;
    break;
  case JOYSTICK_BUTTON:
    result = _get_joystick_button_control( &mapping->js_button, control );
    break;
  case KEYBOARD_BUTTON:
    result = _get_keyboard_button_control( &mapping->key_button, control );
    break;
  }
}

int _get_joystick_axis_control( const struct joystick_axis_mapping_t* map,
				struct control__analog_t* control ){
  int result = UNKNOWN_FAILURE;
  control->value = 0;
  control->timestamp = 0;

  const js__joystick_state_t* state = js__get_joystick_state(map->joystick_id);

  if ( !state ){
    result = CONTROL__NO_SUCH_JOYSTICK;
  }
  else if ( JS__MAX_AXES <= map->axis_id ||
	    !state->axis[map->axis_id].enabled ){
    result = CONTROL__NO_SUCH_AXIS;
  }
  else{
    float input = fmaxf( map->min_input,
			 fminf( map->max_input,
				state->axis[map->axis_id].value ));

    control->value = 
      fminf( 1.0f, 
	     fmaxf( 0.0f,
		    (state->axis[map->axis_id].value - map->min_input) /
		    (map->max_input - map->min_input) ));

    control->timestamp = state->axis[map->axis_id].timestamp;

    result = SUCCESS;
  }

  return result;
}

int _get_joystick_button_control( const struct joystick_button_mapping_t* map,
				  struct control__switch_t* control ){

  int result = UNKNOWN_FAILURE;
  control->value = FALSE;
  control->timestamp = 0;

  const js__joystick_state_t* state = js__get_joystick_state(map->joystick_id);

  if ( !state ){
    result = CONTROL__NO_SUCH_JOYSTICK;
  }
  else if ( JS__MAX_BUTTONS <= map->button_id ||
	    !state->button[map->button_id].enabled ){
    result = CONTROL__NO_SUCH_BUTTON;
  }
  else{
    control->value = state->button[map->button_id].value;
    control->timestamp = state->button[map->button_id].timestamp;

    result = SUCCESS;
  }

  return result;
}

int _get_keyboard_button_control( const struct keyboard_button_mapping_t* map,
				  struct control__switch_t* control ){
  control->value = 0;
  control->timestamp = 0;

  return SUCCESS;
}

			     
