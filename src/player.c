#include <player.h>
#include <utils.h>
#include <control.h>

#include <geometry.h>
#include <background.h>

#include <stdio.h>

void player__basic_draw( const struct player_t* player )
{
  struct geo__rect_t dest = 
    { utils__pos2screen(player->position.x), 
      utils__pos2screen(player->position.y), 
      utils__pos2screen(player->prototype->bounding_box.width), 
      utils__pos2screen(player->prototype->bounding_box.height) };

  video__rect( &dest, player->color[0], player->color[1], player->color[2], 255 );
}

void player__basic_update( struct player_t* player, milliseconds_t frame_length )
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

  if ( !player->bottom_collision ){
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

  printf( "before movement this frame.y: %d\n", movement_this_frame.y );

  background__collision_test( player->background,
			      &bbox,
			      &movement_this_frame,
			      &(player->top_collision),
			      &(player->bottom_collision),
			      &(player->left_collision),
			      &(player->right_collision) );

  printf( "after movement this frame.y: %d\n", movement_this_frame.y );

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

