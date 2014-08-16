#ifndef CONTROL_H
#define CONTROL_H

#include <types.h>

enum { 
  CONTROL__NO_SUCH_PLAYER = 1,
  CONTROL__NO_SUCH_JOYSTICK,
  CONTROL__NO_SUCH_AXIS,
  CONTROL__NO_SUCH_BUTTON
};

enum {
  CONTROL__MAX_PLAYERS = 1
};

struct control__analog_t{
  float value;
  timestamp_t timestamp;
};

struct control__switch_t{
  bool_t value;
  timestamp_t timestamp;
};

struct control__state_t {
  struct control__analog_t up;
  struct control__analog_t down;
  struct control__analog_t left;
  struct control__analog_t right;
  struct control__switch_t jump;
  struct control__switch_t fire;
};

int control__setup(const char* mapping_file);

int control__get_state( uint32_t player, struct control__state_t* state );

#endif