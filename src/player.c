#include <player.h>
#include <utils.h>
#include <control.h>
#include <constants.h>
#include <timing.h>

#include <geometry.h>
#include <background.h>
#include <video.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool_t _apply_config_value( const char* name, int value, struct player_prototype_t* prototype );

static void _walk_or_run_or_coast( const struct control__analog_t* left,
				   const struct control__analog_t* right,
				   const player__config_t* config,
				   int this_frame_x_acceleration,
				   int this_frame_x_decceleration,
				   int* velocity_x_ptr );

static void _coast( int this_frame_x_decceleration,
		    int* velocity_x_ptr );

void player__basic_draw( const struct player_t* player )
{
  const struct geo__rect_t* bbox = player__get_bounding_box(player);

  struct geo__rect_t dest;
  geo__init_rect( &dest, 
		  utils__pos2screen(player->position.x + bbox->x), 
		  utils__pos2screen(player->position.y + bbox->y), 
		  utils__pos2screen(bbox->width), 
		  utils__pos2screen(bbox->height) );
  video__rect( &dest, player->color[0], player->color[1], player->color[2], 255 );
}

void player__basic_update( struct player_t* player, 
			   const struct  background_t* terrain,
			   ticks_t frame_length )
{
  enum player__state_e previous_state = player->state.value;
  enum player__state_e new_state = player__calculate_new_state( player );
  struct geo__rect_t bbox = *player__get_bounding_box(player);
  struct geo__vector_t movement_this_frame;
  movement_this_frame.x = (player->velocity.x * frame_length)/TIMING__SECONDS_TO_TICKS;
  movement_this_frame.y = (player->velocity.y * frame_length)/TIMING__SECONDS_TO_TICKS;
  
  

  if ( new_state != previous_state )
  {
    player->state.value = new_state;
    player->state.timestamp = timing__get_top_of_frame();
  }

  player__calculate_new_velocity( previous_state,
				  player, 
				  frame_length, 
				  &player->velocity );

  bbox.x += player->position.x;
  bbox.y += player->position.y;

  background__collision_test( terrain,
			      &bbox,
			      &movement_this_frame,
			      &(player->top_collision),
			      &(player->bottom_collision),
			      &(player->left_collision),
			      &(player->right_collision) );

  player->position.x += movement_this_frame.x;
  player->position.y += movement_this_frame.y;

  bbox.x += movement_this_frame.x;
  bbox.y += movement_this_frame.y;

  if ( (player->top_collision && player->velocity.y < 0) ||
       (player->bottom_collision && 0 < player->velocity.y) ){
    player->velocity.y = 0;
  }

  if ( (player->left_collision && player->velocity.x < 0) ||
       (player->right_collision && 0 < player->velocity.x) ){
       player->velocity.x = 0;
  }

  /* check for a ledge to grab */
  if ( player->state.value == PLAYER__STATE_SLIDING_WALL_ON_LEFT || 
       player->state.value == PLAYER__STATE_SLIDING_WALL_ON_RIGHT ||
       player->state.value == PLAYER__STATE_HANGING_ON_LEDGE )
  {
    struct geo__vector_t zero_vector;
    struct geo__rect_t grabbing_box;

    bool_t top = FALSE, bottom = FALSE, left = FALSE, right = FALSE;

    zero_vector.x = 0;
    zero_vector.y = 0;
    
    geo__init_rect(&grabbing_box, 0, 0, bbox.width, player->prototype->config.ledge_grab_height);

    grabbing_box.x = bbox.x;
    grabbing_box.y = bbox.y-grabbing_box.height;

    background__collision_test( terrain,
				&grabbing_box,
				&zero_vector,
				&top,
				&bottom,
				&left,
				&right );

    if (!top && !bottom && !left && !right)
    {
      grabbing_box.y = bbox.y + grabbing_box.height;

      background__collision_test( terrain,
				  &grabbing_box,
				  &zero_vector,
				  &top,
				  &bottom,
				  &left,
				  &right );

      player->against_ledge = (left || right);
    }
    else
    {
      player->against_ledge = FALSE;
    }
  }
  else
  {
    player->against_ledge = FALSE;
  }
}

enum player__state_e player__calculate_new_state( const struct player_t* player )
{
  enum player__state_e result = player->state.value;

  const bool_t jump_pressed = control__button_pressed( &player->control->jump );
  const bool_t jump_released = control__button_released( &player->control->jump );

  const bool_t pushing_against_left_wall = 
    (player->left_collision && 
     control__at_least_low(&player->control->left));
  const bool_t pushing_against_right_wall = 
    (player->right_collision && 
     control__at_least_low(&player->control->right));

  const player__config_t* config = &(player->prototype->config);

  switch ( player->state.value )
  {
  case PLAYER__STATE_NONE:
  case PLAYER__STATE_STANDING:
  case PLAYER__STATE_DUCKING:
  case PLAYER__STATE_WALKING:
  case PLAYER__STATE_RUNNING:
    if ( !player->bottom_collision )
    {
      result = PLAYER__STATE_FALLING;
    }
    else if ( jump_pressed )
    {
      if (( control__at_least_low(&player->control->left) && 
	    config->minimum_backflip_starting_velocity < player->velocity.x) ||
	  ( control__at_least_low(&player->control->right) && 
	    player->velocity.x < -config->minimum_backflip_starting_velocity))
      {
	result = PLAYER__STATE_START_BACK_FLIP;
      }
      else
      {
	result = PLAYER__STATE_START_JUMPING;
      }
    }
    else if ( control__at_least_high(&player->control->down) )
    {  
      result = PLAYER__STATE_DUCKING;
    }
    else if ( config->velocity_limit_walking < abs(player->velocity.x) )
    {
      result = PLAYER__STATE_RUNNING;
    }
    else if ( 0 < abs(player->velocity.x) )
    {
      result = PLAYER__STATE_WALKING;
    }
    else
    {
      result = PLAYER__STATE_STANDING;
    }
    break;

  case PLAYER__STATE_JUMPING:
  case PLAYER__STATE_JUMPING_OFF_WALL:
  case PLAYER__STATE_BACK_FLIP:
    if ( 0 < player->velocity.y )
    {
      result = PLAYER__STATE_FALLING;
    }
    else if ( jump_released )
    {
      result = PLAYER__STATE_CEASE_JUMPING;
    }
    else
    {
      /* no-op, maintain state */
    }
    break;

  case PLAYER__STATE_FALLING:
    if ( player->bottom_collision )
    {
      result = PLAYER__STATE_STANDING;
    }
    else if ( pushing_against_left_wall )
    {
      result = PLAYER__STATE_SLIDING_WALL_ON_LEFT;
    }
    else if ( pushing_against_right_wall )
    {
      result = PLAYER__STATE_SLIDING_WALL_ON_RIGHT;
    }
    else
    { 
      /* still falling, no-op */
    }
    break;

  case PLAYER__STATE_SLIDING_WALL_ON_LEFT:
    if ( jump_pressed )
    {
      result = PLAYER__STATE_START_JUMPING_OFF_WALL_ON_LEFT;
    }
    else if ( pushing_against_left_wall )
    {
      if ( player->against_ledge )
      {
	result = PLAYER__STATE_HANGING_ON_LEDGE;
      }
      else
      {
	/* no-op, maintain state */
      }
    }
    else
    { 
      result = PLAYER__STATE_FALLING;
    }
    break;

  case PLAYER__STATE_SLIDING_WALL_ON_RIGHT:
    if ( jump_pressed )
    {
      result = PLAYER__STATE_START_JUMPING_OFF_WALL_ON_RIGHT;
    }
    else if ( pushing_against_right_wall )
    {
      if ( player->against_ledge )
      {
	result = PLAYER__STATE_HANGING_ON_LEDGE;
      }
      else
      {
	/* no-op, maintain state */
      }
    }
    else
    { 
      result = PLAYER__STATE_FALLING;
    }
    break;

  case PLAYER__STATE_HANGING_ON_LEDGE:
    if ( jump_pressed )
    {
      result = PLAYER__STATE_START_CLIMBING_LEDGE;
    }
    else if ( player->against_ledge &&
	      (pushing_against_left_wall || pushing_against_right_wall) )
    {
      /* no-op, maintain state */
    }
    else
    {
      result = PLAYER__STATE_FALLING;
    }
    break;

  case PLAYER__STATE_START_JUMPING:
  case PLAYER__STATE_START_CLIMBING_LEDGE:
    result = PLAYER__STATE_JUMPING;
    break;

  case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_LEFT:
  case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_RIGHT:
    result = PLAYER__STATE_JUMPING_OFF_WALL;
    break;
    
  case PLAYER__STATE_START_BACK_FLIP:
    result = PLAYER__STATE_BACK_FLIP;
    break;
    
  case PLAYER__STATE_CEASE_JUMPING:
    result = PLAYER__STATE_FALLING;
    break;
      
  default:
    fprintf( stderr, 
	     "WARNING: unhandled player state: %d\n", 
	     player->state.value );
    break;
  }

  return result;
}

int player__calculate_new_velocity( enum player__state_e previous_state,
				    const struct player_t* player, 
				    ticks_t frame_length,
				    struct geo__vector_t* new_velocity )
{
  if ( !new_velocity ) {
    return UNKNOWN_FAILURE;
  }
  else {

    struct geo__vector_t velocity = player->velocity;

    const player__config_t* config = &(player->prototype->config);

    const int this_frame_x_acceleration = (config->per_second_x_acceleration * frame_length) / TIMING__SECONDS_TO_TICKS;
    const int this_frame_x_decceleration = (config->per_second_x_decceleration * frame_length) / TIMING__SECONDS_TO_TICKS;

    int gravity_this_frame = 0;

    /* x velocity */
    switch ( player->state.value )
      {
      case PLAYER__STATE_STANDING:
      case PLAYER__STATE_WALKING:
      case PLAYER__STATE_RUNNING:
      case PLAYER__STATE_START_JUMPING:
      case PLAYER__STATE_JUMPING:
      case PLAYER__STATE_CEASE_JUMPING:
      case PLAYER__STATE_FALLING:
	_walk_or_run_or_coast( &(player->control->left),
			       &(player->control->right),
			       &(player->prototype->config),
			       this_frame_x_acceleration,
			       this_frame_x_decceleration,
			       &velocity.x );
	break;

      case PLAYER__STATE_DUCKING:
	_coast( this_frame_x_decceleration, &velocity.x );
	break;
    
      case PLAYER__STATE_SLIDING_WALL_ON_LEFT:
      case PLAYER__STATE_SLIDING_WALL_ON_RIGHT:
      case PLAYER__STATE_HANGING_ON_LEDGE:
      case PLAYER__STATE_START_CLIMBING_LEDGE:
	velocity.x = 0;
	break;

      case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_LEFT:
	velocity.x = config->wall_jump_initial_x_velocity;
	break;

      case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_RIGHT:
	velocity.x = -config->wall_jump_initial_x_velocity;
	break;

      case PLAYER__STATE_START_BACK_FLIP:
	velocity.x = config->backflip_initial_x_velocity*(velocity.x < 0 ? 1 : -1);
	break;

      case PLAYER__STATE_JUMPING_OFF_WALL:
      case PLAYER__STATE_BACK_FLIP:
	/* no-op, maintain same x velocity */
	break;

      default:
	fprintf(stderr, 
		"unhandled state in setting x velocity: %d\n", 
		player->state.value);
	break;
      }

    gravity_this_frame = 
      (config->per_second_gravity_acceleration * frame_length)/TIMING__SECONDS_TO_TICKS;

    /* y velocity */
    switch ( player->state.value )
      {
      case PLAYER__STATE_STANDING:
      case PLAYER__STATE_WALKING:
      case PLAYER__STATE_RUNNING:
      case PLAYER__STATE_DUCKING:
      case PLAYER__STATE_HANGING_ON_LEDGE:
	velocity.y = 0;
	break;

      case PLAYER__STATE_JUMPING:
      case PLAYER__STATE_JUMPING_OFF_WALL:
      case PLAYER__STATE_BACK_FLIP:
	velocity.y += gravity_this_frame;
	break;

      case PLAYER__STATE_FALLING:
	velocity.y += gravity_this_frame;
	if ( config->velocity_limit_falling < velocity.y )
	  {
	    velocity.y = config->velocity_limit_falling;
	  }
	break;

      case PLAYER__STATE_CEASE_JUMPING:
	velocity.y += gravity_this_frame;
	if ( velocity.y < config->jump_final_y_velocity )
	  {
	    velocity.y = -config->jump_final_y_velocity;
	  }

	break;
    
      case PLAYER__STATE_SLIDING_WALL_ON_LEFT:
      case PLAYER__STATE_SLIDING_WALL_ON_RIGHT:
	velocity.y += gravity_this_frame;
	if ( config->velocity_limit_wall_sliding < velocity.y )
	  {
	    velocity.y = config->velocity_limit_wall_sliding;
	  }
	break;

      case PLAYER__STATE_START_JUMPING:
	velocity.y = -( config->jump_initial_y_velocity + 
			abs(config->jump_x_velocity_percent*velocity.x)/100 ); 
	break;

      case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_LEFT:
      case PLAYER__STATE_START_JUMPING_OFF_WALL_ON_RIGHT:
	velocity.y = -(config->wall_jump_initial_y_velocity);
	break;

      case PLAYER__STATE_START_CLIMBING_LEDGE:
	velocity.y = -(config->climb_ledge_initial_y_velocity);
	break;

      case PLAYER__STATE_START_BACK_FLIP:
	velocity.y = -(config->backflip_initial_y_velocity);

	break;
    
      default:
	fprintf(stderr, 
		"unhandled state in setting x velocity: %d\n", 
		player->state.value);
	break;
      }

    *new_velocity = velocity;

    return SUCCESS;
  }
}

int player__load_config( const char* config_file,
			 struct player_prototype_t* prototype )
{
    int result = UNKNOWN_FAILURE;
    FILE* fin = NULL;

    char name[64];
    int value;

    memset(name, '\0', sizeof(name));
    
    memset(&(prototype->bounding_box_standing), '\0', sizeof(prototype->bounding_box_standing));
    memset(&(prototype->bounding_box_ducking), '\0', sizeof(prototype->bounding_box_ducking));
    memset(&(prototype->config), '\0', sizeof(prototype->config));

    if ( config_file && prototype )
    {
      fin = fopen(config_file, "r");

      while ( fin )
      {
	if ( 2 == fscanf(fin, "%63s %d*c", name, &value) &&
	     _apply_config_value( name, value, prototype ) )
	{	  
	  result = SUCCESS;
	}

	if ( feof(fin) )
	{
	  fclose(fin);
	  fin = NULL;
	}
      }
    }

    return result;
}

bool_t _apply_config_value( const char* name, 
			    int value, 
			    struct player_prototype_t* prototype )
{
  player__config_t* config = &(prototype->config);

#define APPLY_CONFIG(config_name, config_value) \
  if ( !strcmp(config_name, name) ){		\
    config_value = utils__screen2pos(value);	\
    return TRUE;				\
  } 

#define APPLY_PERCENT(config_name, config_value) \
  if ( !strcmp(config_name, name) ){ \
    config_value = value;	     \
    return TRUE;                     \
  } 

  APPLY_CONFIG("velocity_limit_running", config->velocity_limit_running);
  APPLY_CONFIG("velocity_limit_walking", config->velocity_limit_walking);
  APPLY_CONFIG("velocity_limit_wall_sliding", 
	       config->velocity_limit_wall_sliding);
  APPLY_CONFIG("velocity_limit_falling", config->velocity_limit_falling);
  APPLY_CONFIG("per_second_x_acceleration", config->per_second_x_acceleration);
  APPLY_CONFIG("per_second_x_decceleration",config->per_second_x_decceleration);
  APPLY_CONFIG("per_second_gravity_acceleration", 
	       config->per_second_gravity_acceleration);
  APPLY_CONFIG("jump_initial_y_velocity", config->jump_initial_y_velocity);
  APPLY_PERCENT("jump_x_velocity_percent", config->jump_x_velocity_percent);
  APPLY_CONFIG("climb_ledge_initial_y_velocity", 
	       config->climb_ledge_initial_y_velocity);
  APPLY_CONFIG("wall_jump_initial_x_velocity",     
	       config->wall_jump_initial_x_velocity);
  APPLY_CONFIG("wall_jump_initial_y_velocity",     
	       config->wall_jump_initial_y_velocity);
  APPLY_CONFIG("jump_final_y_velocity", config->jump_final_y_velocity);
  APPLY_CONFIG("minimum_backflip_starting_velocity", 
	       config->minimum_backflip_starting_velocity );
  APPLY_CONFIG("backflip_initial_x_velocity",
	       config->backflip_initial_x_velocity );
  APPLY_CONFIG("backflip_initial_y_velocity",
	       config->backflip_initial_y_velocity );
  APPLY_CONFIG("bounding_box_standing_x", prototype->bounding_box_standing.x);
  APPLY_CONFIG("bounding_box_standing_y", prototype->bounding_box_standing.y);
  APPLY_CONFIG("bounding_box_standing_width", prototype->bounding_box_standing.width);
  APPLY_CONFIG("bounding_box_standing_height",prototype->bounding_box_standing.height);
  APPLY_CONFIG("bounding_box_ducking_x", prototype->bounding_box_ducking.x);
  APPLY_CONFIG("bounding_box_ducking_y", prototype->bounding_box_ducking.y);
  APPLY_CONFIG("bounding_box_ducking_width", prototype->bounding_box_ducking.width);
  APPLY_CONFIG("bounding_box_ducking_height",prototype->bounding_box_ducking.height);
  APPLY_CONFIG("ledge_grab_height",config->ledge_grab_height);


  fprintf( stderr, "unknown value %s in player configuration file\n", name );
  return FALSE;

}

const struct geo__rect_t* player__get_bounding_box( const struct player_t* player )
{
  const struct geo__rect_t* result = NULL;

  if ( player )
  {
    switch ( player->state.value )
    {
    case PLAYER__STATE_DUCKING:
      result = &(player->prototype->bounding_box_ducking);
      break;
    default:
      result = &(player->prototype->bounding_box_standing);
      break;
    }
  }

  return result;
}

void _walk_or_run_or_coast( const struct control__analog_t* left,
			    const struct control__analog_t* right,
			    const player__config_t* config,
			    int this_frame_x_acceleration,
			    int this_frame_x_decceleration,
			    int* velocity_x_ptr )
{
  int maximum_x_velocity = 0;

  if ( control__at_least_low(left) )
  {
    maximum_x_velocity =
      (control__at_least_high(left) ?
       config->velocity_limit_running :
       config->velocity_limit_walking);

    if (-maximum_x_velocity < ((*velocity_x_ptr)-this_frame_x_acceleration))
    {
      *velocity_x_ptr -= this_frame_x_acceleration;
    }
    else if ( (*velocity_x_ptr)+this_frame_x_acceleration < -maximum_x_velocity )
    {
      *velocity_x_ptr += this_frame_x_acceleration;
    }
    else
    {
      *velocity_x_ptr = -maximum_x_velocity;
    }
  }
  else if ( control__at_least_low(right) )
  {
    maximum_x_velocity =
      (control__at_least_high(right) ?
       config->velocity_limit_running :
       config->velocity_limit_walking);
    if (((*velocity_x_ptr)+this_frame_x_acceleration) < maximum_x_velocity)
    {
      *velocity_x_ptr += this_frame_x_acceleration;
    }
    else if ( maximum_x_velocity < (*velocity_x_ptr)-this_frame_x_acceleration )
    {
      *velocity_x_ptr -= this_frame_x_acceleration;
    }
    else
    {
      *velocity_x_ptr = maximum_x_velocity;
    }
  }
  else /* x decceleration */
  {
    _coast( this_frame_x_decceleration, velocity_x_ptr );
  }
}

void _coast( int this_frame_x_decceleration, int* velocity_x_ptr )
{
    if ( *velocity_x_ptr < -this_frame_x_decceleration )
    {
      *velocity_x_ptr += this_frame_x_decceleration;
    }
    else if ( this_frame_x_decceleration < *velocity_x_ptr )
    {
      *velocity_x_ptr -= this_frame_x_decceleration;
    }
    else
    {
      *velocity_x_ptr = 0;
    }
}
