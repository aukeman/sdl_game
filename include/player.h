#ifndef PLAYER_H
#define PLAYER_H

#include <geometry.h>
#include <control.h> 

#include <stdint.h>

struct player_t;
struct background_t;

typedef void player__draw_fxn( const struct player_t* player );
typedef void player__update_fxn( struct player_t* player, milliseconds_t frame_length );

struct player_prototype_t{

  struct geo__rect_t bounding_box;

  player__draw_fxn* draw_fxn;
  player__update_fxn* update_fxn;
  const struct video__texture_handle_t* texture;
};

struct player_t{

  struct geo__point_t position;
  struct geo__vector_t velocity;

  const struct player_prototype_t* prototype;
  const struct control__state_t* control;
  
  struct background_t* background;

  uint8_t color[3];
};

void player__basic_draw( const struct player_t* player );
void player__basic_update( struct player_t* player, milliseconds_t frame_length );

#endif
