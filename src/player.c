#include <player.h>
#include <utils.h>
#include <control.h>
#include <constants.h>

#include <geometry.h>
#include <background.h>

#include <stdio.h>

void player__basic_draw( int32_t pos_x, 
			 int32_t pos_y, 
			 const struct player_t* player )
{

  int32_t screen_pos_x = utils__pos2screen(pos_x);
  int32_t screen_pos_y = utils__pos2screen(pos_y);

  video__translate( -screen_pos_x, -screen_pos_y );

  const struct geo__rect_t* bbox = player__get_bounding_box(player);

  struct geo__rect_t dest = 
    { utils__pos2screen(player->position.x + bbox->x), 
      utils__pos2screen(player->position.y + bbox->y), 
      utils__pos2screen(bbox->width), 
      utils__pos2screen(bbox->height) };

  video__rect( &dest, player->color[0], player->color[1], player->color[2], 255 );

  video__translate( screen_pos_x, screen_pos_y );

}

void player__basic_update( struct player_t* player, 
			   const struct  background_t* terrain,
			   milliseconds_t frame_length )
{
  const int speed = utils__screen2pos(100);

  const bool_t jump_pressed = control__button_pressed( &player->control->jump );
  const bool_t jump_released = control__button_released( &player->control->jump );

  const enum player__jump_state_e previous_state = player->jump_state;

  /* figure out current jump state */
  if ( player->bottom_collision ){

    if ( jump_pressed )
    {
      player->jump_state = PLAYER__JUMP_STATE_JUMPING;
    }
    else if ( 0.75 < player->control->down.value )
    {
      player->jump_state = PLAYER__JUMP_STATE_DUCKING;
    }
    else
    {
      player->jump_state = PLAYER__JUMP_STATE_NONE;
    }
  }
  else if ( (player->jump_state == PLAYER__JUMP_STATE_JUMPING ||
	     player->jump_state == PLAYER__JUMP_STATE_JUMPING_OFF_WALL) &&
            player->velocity.y < 0 &&
	    0 < player->control->jump.value )
  {
    /* still jumping; don't change state */
  }
  else if ( player->jump_state == PLAYER__JUMP_STATE_FALLING ||
	    player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT ||
	    player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT ||
	    player->jump_state == PLAYER__JUMP_STATE_HANGING_ON_LEDGE )
  {
    if ( jump_pressed && 
	 (player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT ||
	  player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT) )
    {
      player->jump_state = PLAYER__JUMP_STATE_JUMPING_OFF_WALL;
    }
    else if ( player->left_collision && 0 < player->control->left.value )
    {
      player->jump_state = 
	(player->against_ledge ? 
	 PLAYER__JUMP_STATE_HANGING_ON_LEDGE :
	 PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT);
    }
    else if ( player->right_collision && 0 < player->control->right.value )
    {
      player->jump_state = 
	(player->against_ledge ? 
	 PLAYER__JUMP_STATE_HANGING_ON_LEDGE :
	 PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT);
    }
    else
    {
      player->jump_state = PLAYER__JUMP_STATE_FALLING;
    }
  }
  else
  {
    player->jump_state = PLAYER__JUMP_STATE_FALLING;
  }

  static const int running_velocity_limit = 400;
  static const int walking_velocity_limit = 150;

  static const int per_frame_x_acceleration = 20;
  static const int per_frame_x_decceleration = 10;

  static const int per_second_gravity_acceleration = 1000;

  static const int initial_jump_velocity = 600;
  static const int initial_jump_from_ledge_velocity = 400;
  static const int final_jump_velocity = 100;

  static const int max_wall_sliding_velocity = 300;

  int maximum_x_velocity = 0;

  /* x acceleration */
  if ( player->jump_state == PLAYER__JUMP_STATE_DUCKING )
  {
      if ( player->velocity.x < -20 )
      {
	player->velocity.x += per_frame_x_decceleration;
      }
      else if ( 20 < player->velocity.x )
      {
	player->velocity.x -= per_frame_x_decceleration;
      }
      else
      {
	player->velocity.x = 0;
      }
  }
  else if ( player->jump_state != PLAYER__JUMP_STATE_JUMPING_OFF_WALL )
  {
    if ( player->control->left.value )
    {
      maximum_x_velocity = 
	(0.75 < player->control->left.value ? running_velocity_limit : walking_velocity_limit);

      if (-maximum_x_velocity < (player->velocity.x-per_frame_x_acceleration))
      {
	player->velocity.x -= per_frame_x_acceleration;
      }
      else if ( player->velocity.x+per_frame_x_acceleration < -maximum_x_velocity )
      {
	player->velocity.x += per_frame_x_acceleration;
      }
      else
      {
	player->velocity.x = -maximum_x_velocity;
      }
    }
    else if ( player->control->right.value )
    {
      maximum_x_velocity = 
	(0.75 < player->control->right.value ? running_velocity_limit : walking_velocity_limit);

      if ((player->velocity.x+per_frame_x_acceleration) < maximum_x_velocity)
      {
	player->velocity.x += per_frame_x_acceleration;
      }
      else if ( maximum_x_velocity < player->velocity.x-per_frame_x_acceleration )
      {
	player->velocity.x -= per_frame_x_acceleration;
      }
      else
      {
	player->velocity.x = maximum_x_velocity;
      }
    }
    else /* x decceleration */
    {

      if ( player->velocity.x < -20 )
      {
	player->velocity.x += per_frame_x_decceleration;
      }
      else if ( 20 < player->velocity.x )
      {
	player->velocity.x -= per_frame_x_decceleration;
      }
      else
      {
	player->velocity.x = 0;
      }
    }
  }

  /* y acceleration and velocity */

  /* apply gravity */
  switch ( player->jump_state )
  {
  case PLAYER__JUMP_STATE_NONE:
  case PLAYER__JUMP_STATE_HANGING_ON_LEDGE:
    /* no gravity calculations needed when on ground or hanging on ledge*/
    player->velocity.y = 0;
    break;
  default:
    player->velocity.y += 
      ( per_second_gravity_acceleration * frame_length)/1000;
    break;
  }

  /* other accelerations */
  switch ( player->jump_state )
  {
  case PLAYER__JUMP_STATE_NONE:
    player->velocity.y = 0;
    break;

  case PLAYER__JUMP_STATE_JUMPING:
  case PLAYER__JUMP_STATE_JUMPING_OFF_WALL:
  case PLAYER__JUMP_STATE_HANGING_ON_LEDGE:
    if ( jump_pressed )
    {
      if ( previous_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT )
      {
	player->velocity.x = 400;
      }
      else if ( previous_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT )
      {
	player->velocity.x = -400;
      }

      if ( previous_state == PLAYER__JUMP_STATE_HANGING_ON_LEDGE )
      {
	player->velocity.y = -initial_jump_from_ledge_velocity;
      }
      else
      {
	player->velocity.y = -(initial_jump_velocity + abs(player->velocity.x)/4);
      }
    }
    break;

  case PLAYER__JUMP_STATE_FALLING:
    if ( jump_released && player->velocity.y < -final_jump_velocity)
    {
      player->velocity.y = -final_jump_velocity;
    }

    break;

  case PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT:
  case PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT:

    if ( max_wall_sliding_velocity < player->velocity.y )
    {
      player->velocity.y = max_wall_sliding_velocity;
    }
    break;

  default:
    /* no other vertical accelerations */
    break;
  }

  struct geo__rect_t bbox = *player__get_bounding_box(player);

  bbox.x += player->position.x;
  bbox.y += player->position.y;

  struct geo__vector_t movement_this_frame = 
    { (player->velocity.x * frame_length)/1000, 
      (player->velocity.y * frame_length)/1000 };

  background__collision_test( terrain,
			      &bbox,
			      &movement_this_frame,
			      &(player->top_collision),
			      &(player->bottom_collision),
			      &(player->left_collision),
			      &(player->right_collision) );

  player->position.x += movement_this_frame.x;
  player->position.y += movement_this_frame.y;

  if ( (player->top_collision && player->velocity.y < 0) ||
       (player->bottom_collision && 0 < player->velocity.y) ){
    player->velocity.y = 0;
  }

  if ( (player->left_collision && player->velocity.x < 0) ||
       (player->right_collision && 0 < player->velocity.x) ){
       player->velocity.x = 0;
  }

  /* check for a ledge to grab */
  if ( player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT || 
       player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT ||
       player->jump_state == PLAYER__JUMP_STATE_HANGING_ON_LEDGE )
  {
    struct geo__vector_t zero_vector = {0, 0};
    struct geo__rect_t grabbing_box = { 0, 0, bbox.width, 4 };

    grabbing_box.x = bbox.x;
    grabbing_box.y = bbox.y-grabbing_box.height;

    bool_t top, bottom, left, right;

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

const struct geo__rect_t* player__get_bounding_box( const struct player_t* player )
{
  const struct geo__rect_t* result = NULL;

  if ( player )
  {
    switch ( player->jump_state )
    {
    case PLAYER__JUMP_STATE_DUCKING:
      result = &(player->prototype->bounding_box_ducking);
      break;
    default:
      result = &(player->prototype->bounding_box_standing);
      break;
    }
  }

  return result;
}
