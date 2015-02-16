#include <player.h>
#include <utils.h>
#include <control.h>

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

  struct geo__rect_t dest = 
    { utils__pos2screen(player->position.x), 
      utils__pos2screen(player->position.y), 
      utils__pos2screen(player->prototype->bounding_box.width), 
      utils__pos2screen(player->prototype->bounding_box.height) };

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
    player->jump_state = 
      jump_pressed ? PLAYER__JUMP_STATE_JUMPING : PLAYER__JUMP_STATE_NONE;
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
	    player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT )
  {
    if ( jump_pressed && 
	 (player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT ||
	  player->jump_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT) )
    {
      player->jump_state = PLAYER__JUMP_STATE_JUMPING_OFF_WALL;
    }
    else if ( player->left_collision && 0 < player->control->left.value )
    {
      player->jump_state = PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT;
    }
    else if ( player->right_collision && 0 < player->control->right.value )
    {
      player->jump_state = PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT;
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

  static const int per_frame_x_acceleration = 20;
  static const int per_frame_x_decceleration = 10;

  static const int per_second_gravity_acceleration = 1000;

  static const int initial_jump_velocity = 600;
  static const int final_jump_velocity = 100;

  static const int max_wall_sliding_velocity = 300;

  /* x acceleration */
  if ( player->jump_state != PLAYER__JUMP_STATE_JUMPING_OFF_WALL )
  {
    if ( -utils__screen2pos(100) < player->velocity.x && 
	   player->control->left.value )
    {
      player->velocity.x -= per_frame_x_acceleration;
    }
  
    if ( player->velocity.x < utils__screen2pos(100) && 
	 player->control->right.value )
    {
      player->velocity.x += per_frame_x_acceleration;
    }

    /* x decelleration */
    if ( player->control->left.value == 0 && 
	 player->control->right.value == 0 )
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
    /* no gravity calculations needed when on ground */
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
    if ( jump_pressed )
    {
      player->velocity.y = -(initial_jump_velocity + abs(player->velocity.x)/4);

      if ( previous_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_LEFT )
      {
	player->velocity.x = 400;
      }
      else if ( previous_state == PLAYER__JUMP_STATE_SLIDING_WALL_ON_RIGHT )
      {
	player->velocity.x = -400;
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

  struct geo__rect_t bbox = player->prototype->bounding_box;

  bbox.x = player->position.x;
  bbox.y = player->position.y;

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
}

