#ifndef TIMING_H
#define TIMING_H

#include <types.h>

#define TIMING__SECONDS_TO_TICKS 100000

int timing__setup();
int timing__teardown();

int timing__set_fixed_frame_rate(hertz_t);
hertz_t timing__get_fixed_frame_rate();

uint32_t timing__get_frame_count();

int timing__declare_top_of_frame();

timestamp_t timing__get_top_of_frame();
ticks_t timing__get_frame_length();

uint32_t timing__get_frame_count();

float timing__get_average_fps();

float timing__get_instantaneous_fps();

#endif
