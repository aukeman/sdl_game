#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>
#include <moveable.h>

#include <stdint.h>

struct player_t{
  
  int player_idx;

  struct moveable_t moveable;

  int gun_x;
  int gun_y;

  uint8_t color[3];
};

#endif
