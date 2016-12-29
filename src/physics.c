#include <physics.h>
#include <types.h>
#include <timing.h>
#include <constants.h>

int physics__dead_reckon( timestamp_t last_known_time, 
			  const struct physics__location_t* last_known_state, 
			  timestamp_t current_time, 
			  struct physics__location_t* current_state ){
  
  ticks_t d_time = current_time - last_known_time;

  current_state->position.x = last_known_state->position.x + (last_known_state->velocity.x * d_time) / TIMING__TICKS_PER_SECOND;

  current_state->position.y = last_known_state->position.y + (last_known_state->velocity.y * d_time) / TIMING__TICKS_PER_SECOND;

  return SUCCESS;
}
