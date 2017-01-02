#include <timing.h>
#include <constants.h>

#include <sys/time.h>

#include <string.h>
#include <stdint.h>

static timestamp_t top_of_frame_timestamp = 0;
static timestamp_t top_of_last_frame_timestamp = 0;

static uint32_t frame_count = 0;

enum { tof_history_size = 60 };
static timestamp_t tof_history[tof_history_size];
static timestamp_t* tof_history_iter = tof_history;
static const timestamp_t* tof_history_end = tof_history + tof_history_size;

static struct timeval epoch = { 0, 0 };

static 
timestamp_t _get_timestamp(){
  struct timeval now = {0, 0};

  gettimeofday(&now, NULL);
  return (now.tv_sec - epoch.tv_usec) * TIMING__SECONDS_TO_TICKS + (now.tv_usec - epoch.tv_usec);
}

int timing__setup(){

  timing__teardown();

  gettimeofday(&epoch, NULL);

  return SUCCESS;
}

int timing__teardown(){
  top_of_frame_timestamp = 0;
  top_of_last_frame_timestamp = 0;

  memset(tof_history, '\0', sizeof(tof_history));

  frame_count = 0;

  return SUCCESS;
}

int timing__declare_top_of_frame(){
 
  top_of_last_frame_timestamp = top_of_frame_timestamp;

  top_of_frame_timestamp = _get_timestamp();

  *tof_history_iter = top_of_frame_timestamp;
  ++tof_history_iter;
  if ( tof_history_iter == tof_history_end ){
    tof_history_iter = tof_history;
  }

  ++frame_count;

  return SUCCESS;
}

uint32_t timing__get_frame_count(){
  return frame_count;
}

timestamp_t timing__get_top_of_frame(){
  return top_of_frame_timestamp;
}

ticks_t timing__get_frame_length(){
  return top_of_frame_timestamp - top_of_last_frame_timestamp;
}

float timing__get_average_fps(){
  return ((float)frame_count / top_of_frame_timestamp) * TIMING__SECONDS_TO_TICKS;
}

float timing__get_instantaneous_fps(){

  if ( *tof_history_iter == 0 ){
    return timing__get_average_fps();
  }
  else{
    return ((float)tof_history_size / 
	    (top_of_frame_timestamp - *tof_history_iter)) * TIMING__SECONDS_TO_TICKS;
  }
}

