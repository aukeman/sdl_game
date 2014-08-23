#ifndef PLAYER_H
#define PLAYER_H

#include <entity.h>

#include <stdint.h>

struct player_t{
  
  int player_idx;

  geo__point_t position;
  geo__point_t velocity;

  uint8_t color[3];
};

#endif
