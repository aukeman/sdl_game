#include <player.h>
#include <utils.h>
#include <control.h>

void player__basic_draw( const struct player_t* player )
{
  static geo__rect_t dest = { 0, 0, 32, 32 };

  dest.x = utils__pos2screen(player->position.x);
  dest.y = utils__pos2screen(player->position.y);

  video__rect( &dest, player->color[0], player->color[1], player->color[2], 255 );
}

void player__basic_update( struct player_t* player, milliseconds_t frame_length )
{
  const int speed = utils__screen2pos(100);

  player->velocity.x =
    (player->control->right.value*speed) - (player->control->left.value*speed);

  player->velocity.y =
    (player->control->down.value*speed) - (player->control->up.value*speed);

  player->position.x += (player->velocity.x * frame_length)/1000;
  player->position.y += (player->velocity.y * frame_length)/1000;
}

