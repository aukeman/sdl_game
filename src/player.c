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

  if ( -utils__screen2pos(100) < player->velocity.x && player->control->left.value ){
    player->velocity.x -= 10;
  }
  
  if ( player->velocity.x < utils__screen2pos(100) && player->control->right.value ){
    player->velocity.x += 10;
  }

  if ( player->control->left.value == 0 && player->control->right.value == 0 ){
    player->velocity.x = 0;
  }

  if ( player->control->up.value ){
    player->velocity.y = -200;
  }
  else if ( !player->bottom_collision ){
    /* gravity */
    player->velocity.y += ( utils__screen2pos(200) * frame_length)/1000;
  }
  else if ( player->control->jump.value ){
    player->velocity.y -= 400;
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

