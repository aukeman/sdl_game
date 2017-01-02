#include <timing.h>
#include <constants.h>

#include <SDL/SDL.h>

#include <sys/time.h>
#include <stdint.h>

static timestamp_t top_of_frame_timestamp = 0;
static timestamp_t top_of_last_frame_timestamp = 0;

static timestamp_t top_of_update_timestamp = 0;
static timestamp_t top_of_last_update_timestamp = 0;

static uint32_t frame_count = 0;
static uint32_t update_count = 0;

enum { tof_history_size = 60 };
static timestamp_t tof_history[tof_history_size];
static timestamp_t* tof_history_iter = tof_history;
static const timestamp_t* tof_history_end = tof_history + tof_history_size;

static timestamp_t update_history[tof_history_size];
static timestamp_t* update_history_iter = update_history;
static const timestamp_t* update_history_end = update_history + tof_history_size;

static hertz_t update_rate = 60;
static ticks_t ticks_between_updates = 1666;
static timestamp_t start_of_next_update_frame=0;

static struct timeval epoch = { 0, 0 };

int timing__setup(){

  timing__teardown();

  SDL_Init(SDL_INIT_TIMER);
  gettimeofday( &epoch, NULL );

  return SUCCESS;
}

int timing__teardown(){
  top_of_frame_timestamp = 0;
  top_of_last_frame_timestamp = 0;

  memset(tof_history, '\0', sizeof(tof_history));

  frame_count = 0;
  update_count = 0;

  epoch.tv_sec = 0;
  epoch.tv_usec = 0;

  return SUCCESS;
}

hertz_t timing__set_update_rate(hertz_t new_update_rate){
  ticks_between_updates = 100000 / new_update_rate;
  return update_rate = new_update_rate;
}

hertz_t timing__get_update_rate(){
  return update_rate;
}

ticks_t timing__get_ticks_between_updates(){
  return ticks_between_updates;
}

int timing__declare_top_of_frame(bool_t* update_this_frame){
 
  top_of_last_frame_timestamp = top_of_frame_timestamp;
  top_of_frame_timestamp = timing__get_timestamp();

  if ( start_of_next_update_frame <= top_of_frame_timestamp ){
    *update_this_frame = TRUE;
    top_of_last_update_timestamp = top_of_update_timestamp;
    top_of_update_timestamp = top_of_frame_timestamp;
    start_of_next_update_frame += ticks_between_updates;

printf("start of next update: %u\n", start_of_next_update_frame);

    *update_history_iter = top_of_frame_timestamp;
    ++update_history_iter;
    if ( update_history_iter == update_history_end ){
      update_history_iter = update_history;
    }

    ++update_count;
  }
  else{
    *update_this_frame = FALSE;
  }

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

uint32_t timing__get_update_count(){
  return update_count;
}

timestamp_t timing__get_top_of_frame(){
  return top_of_frame_timestamp;
}

timestamp_t timing__get_top_of_update(){
  return top_of_update_timestamp;
}

ticks_t timing__get_frame_length(){
  return top_of_frame_timestamp - top_of_last_frame_timestamp;
}

ticks_t timing__get_update_length(){
  return top_of_update_timestamp - top_of_last_update_timestamp;
}

float timing__get_average_fps(){
  return ((float)frame_count / top_of_frame_timestamp) * TIMING__TICKS_PER_SECOND;
}

float timing__get_average_ups(){
  return ((float)update_count / top_of_frame_timestamp) * TIMING__TICKS_PER_SECOND;
}

float timing__get_instantaneous_fps(){

  if ( *tof_history_iter == 0 ){
    return timing__get_average_fps();
  }
  else{
    return ((float)tof_history_size / 
	    (top_of_frame_timestamp - *tof_history_iter)) * TIMING__TICKS_PER_SECOND;
  }
}

float timing__get_instantaneous_ups(){

  if ( *update_history_iter == 0 ){
    return 0.0f;
  }
  else{
    return ((float)tof_history_size / 
	    (top_of_frame_timestamp - *update_history_iter)) * TIMING__TICKS_PER_SECOND;
  }
}



timestamp_t timing__get_timestamp(){
  int32_t timestamp = 0;
  struct timeval now = {0, 0};
  gettimeofday(&now, NULL);

  timestamp = (now.tv_usec - epoch.tv_usec)/10;
  timestamp += (now.tv_sec - epoch.tv_sec)*TIMING__TICKS_PER_SECOND;

  return timestamp;
}
