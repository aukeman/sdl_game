#ifndef JOYSTICK
#define JOYSTICK

#include <types.h>

enum {
  JS__MAX_AXES = 8,
  JS__MAX_BUTTONS = 20,
  JS__MAX_JOYSTICKS = 8
};

typedef struct {
  timestamp_t timestamp;
  float value;
  uint8_t enabled;
} js__axis_state_t;

typedef struct {
  timestamp_t timestamp;
  bool_t value;
  uint8_t enabled;
} js__button_state_t;

typedef struct {
  js__axis_state_t axis[JS__MAX_AXES];
  js__button_state_t button[JS__MAX_BUTTONS];
  uint8_t enabled;
} js__joystick_state_t;

int js__setup();
int js__teardown();

const js__joystick_state_t* js__get_joystick_state(uint32_t joystick_index);

#endif
