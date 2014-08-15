#ifndef CONTROL_H
#define CONTROL_H

#include <types.h>

enum { 
  CONTROL__NO_SUCH_PLAYER = 1
};


struct control__state_t {
  float up;
  timestamp_t up_timestamp;

  float down;
  timestamp_t down_timestamp;

  float left;
  timestamp_t left_timestamp;

  float right;
  timestamp_t right_timestamp;

  float jump;
  timestamp_t jump_timestamp;
  
};

int control__get_state( uint32_t player, struct control__state_t* state );

#endif
