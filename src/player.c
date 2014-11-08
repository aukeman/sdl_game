#include <player.h>
#include <utils.h>
#include <control.h>

#include <geometry.h>
#include <background.h>

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

  /* player->velocity.x = */
  /*   (player->control->right.value*speed) - (player->control->left.value*speed); */

  /* player->velocity.y = */
  /*   (player->control->down.value*speed) - (player->control->up.value*speed); */
  
  /* gravity */
  player->velocity.y += ( utils__screen2pos(200) * frame_length)/1000;

  struct geo__rect_t bbox = player->prototype->bounding_box;

  bbox.x = player->position.x;
  bbox.y = player->position.y;

  bool_t top_collision, bottom_collision, left_collision, right_collision;
  
  struct geo__vector_t movement_this_frame = 
    { (player->velocity.x * frame_length)/1000, 
      (player->velocity.y * frame_length)/1000 };

  background__collision_test( player->background,
			      &bbox,
			      &movement_this_frame,
			      &top_collision,
			      &bottom_collision,
			      &left_collision,
			      &right_collision );

  player->position.x += movement_this_frame.x;
  player->position.y += movement_this_frame.y;

  if ( (top_collision && player->velocity.y < 0) ||
       (bottom_collision && 0 < player->velocity.y) ){
    player->velocity.y = 0;
  }

  if ( (left_collision && player->velocity.x < 0) ||
       (right_collision && 0 < player->velocity.x) ){
    player->velocity.x = 0;
  }
}

