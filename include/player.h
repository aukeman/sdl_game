#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>

#include <stdint.h>

struct player_t{
  
  int player_idx;

  geo__point_t position;
  geo__point_t velocity;

  int gun_x;
  int gun_y;

  uint8_t color[3];
};

#endif
