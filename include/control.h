#ifndef CONTROL_H
#define CONTROL_H

#include <types.h>

enum { 
  CONTROL__NO_SUCH_PLAYER = 1,
  CONTROL__MAPPING_FILE_NOT_FOUND,
  CONTROL__NO_DEVICES_TO_MAP,
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

  struct control__analog_t up2;
  struct control__analog_t down2;
  struct control__analog_t left2;
  struct control__analog_t right2;

  struct control__binary_t jump;
  struct control__binary_t fire;
};

enum control_type_e {
  NO_CONTROL_MAPPING,
  ANALOG,
  BINARY
};

struct control_mapping_t{

  enum control_type_e type;

  float min_input;
  float max_input;

  union {
    struct control__analog_t* analog;
    struct control__binary_t* binary;
  } control_type;
};

int control__setup(const char* mapping_file);
int control__teardown();

const struct control__state_t* control__get_state( uint32_t player );

bool_t control__is_full( const struct control__analog_t* control );
bool_t control__is_high( const struct control__analog_t* control );
bool_t control__is_medium( const struct control__analog_t* control );
bool_t control__is_low( const struct control__analog_t* control );

bool_t control__at_least_high( const struct control__analog_t* control );
bool_t control__at_least_medium( const struct control__analog_t* control );
bool_t control__at_least_low( const struct control__analog_t* control );

bool_t control__at_most_high( const struct control__analog_t* control );
bool_t control__at_most_medium( const struct control__analog_t* control );
bool_t control__at_most_low( const struct control__analog_t* control );

bool_t control__button_pressed( const struct control__binary_t* control );
bool_t control__button_released( const struct control__binary_t* control );

#endif
