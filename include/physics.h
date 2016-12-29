#ifndef PHYSICS_H
#define PHYSICS_H

#include <geometry.h>
#include <types.h>

struct physics__location_t {
  struct geo__point_t position;
  struct geo__vector_t velocity;
};

int physics__dead_reckon( timestamp_t last_known_time, 
			  const struct physics__location_t* last_known_state, 
			  timestamp_t current_time, 
			  struct physics__location_t* current_state );

#endif
