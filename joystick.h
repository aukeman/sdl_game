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
} js__control_state_t;

typedef struct {
  js__control_state_t axis[JS__MAX_AXES];
  js__control_state_t button[JS__MAX_BUTTONS];
} js__joystick_state_t;

int js__init();

const js__joystick_state_t* js__get_joystick_state(uint32_t joystick_index);

#endif
