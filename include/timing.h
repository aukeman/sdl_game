#ifndef TIMING_H
#define TIMING_H

#include <types.h>

int timing__setup();
int timing__teardown();

uint32_t timing__get_frame_count();

int timing__declare_top_of_frame();

timestamp_t timing__get_top_of_frame();
milliseconds_t timing__get_frame_length();

uint32_t timing__get_frame_count();

float timing__get_average_fps();

float timing__get_instantaneous_fps();

#endif
