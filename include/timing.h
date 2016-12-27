#ifndef TIMING_H
#define TIMING_H

#include <types.h>

#define TIMING__TICKS_PER_SECOND 100000

int timing__setup();
int timing__teardown();

hertz_t timing__set_frame_rate(hertz_t);
hertz_t timing__get_frame_rate();

uint32_t timing__get_frame_count();

int timing__declare_top_of_frame(bool_t* update_this_frame);

timestamp_t timing__get_top_of_frame();
milliseconds_t timing__get_frame_length();

uint32_t timing__get_frame_count();

float timing__get_average_fps();

float timing__get_instantaneous_fps();

timestamp_t timing__get_timestamp();

ticks_t timing__get_ticks_between_updates();

#endif
