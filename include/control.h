#ifndef CONTROL_H
#define CONTROL_H

#include <types.h>

enum { 
  CONTROL__NO_SUCH_PLAYER = 1,
  CONTROL__MAPPING_FILE_NOT_FOUND,
  CONTROL__BAD_MAPPING_FILE,
  CONTROL__NO_SUCH_JOYSTICK,
  CONTROL__NO_SUCH_AXIS,
  CONTROL__NO_SUCH_BUTTON
};

enum {
  CONTROL__MAX_PLAYERS = 8
};

struct control__analog_t{
  float value;
  timestamp_t timestamp;
};

struct control__binary_t{
  bool_t value;
  timestamp_t timestamp;
};

struct control__state_t {
  struct control__analog_t up;
  struct control__analog_t down;
  struct control__analog_t left;
  struct control__analog_t right;
  struct control__binary_t jump;
  struct control__binary_t fire;
};

int control__setup(const char* mapping_file);
int control__teardown();

const struct control__state_t* control__get_state( uint32_t player );



#endif
