#include <control.h>
#include <joystick.h>

#include <constants.h>

#include <math.h>


int control__get_state( uint32_t player, 
			struct control__state_t* control_state ){

  int result = SUCCESS;

  if ( player != 1 ){
    result = CONTROL__NO_SUCH_PLAYER;
  }
  else{

    const js__joystick_state_t* joystick_state = js__get_joystick_state(0);

    control_state->up = -fminf( 0.0f, joystick_state->axis[1].value );
    control_state->up_timestamp = joystick_state->axis[1].timestamp;

    control_state->down = fmaxf( 0.0f, joystick_state->axis[1].value );
    control_state->down_timestamp = joystick_state->axis[1].timestamp;

    control_state->left = -fminf( 0.0f, joystick_state->axis[0].value );
    control_state->right_timestamp = joystick_state->axis[0].timestamp;

    control_state->right = fmaxf( 0.0f, joystick_state->axis[0].value );
    control_state->right_timestamp = joystick_state->axis[0].timestamp;

    control_state->jump = joystick_state->button[0].value;
    control_state->right_timestamp = joystick_state->button[0].timestamp;
  }

  return result;
}
