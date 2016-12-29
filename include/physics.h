#ifndef PHYSICS_H
#define PHYSICS_H

#include <geometry.h>

struct physics__location_t {
  struct geo__point_t position;
  struct geo__vector_t velocity;
};

#endif
