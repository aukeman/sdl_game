#ifndef PLAYER_H
#define PLAYER_H

#include <geometry.h>
#include <control.h> 

#include <stdint.h>

struct player_t;
struct background_t;

typedef void player__draw_fxn( int32_t screen_pos_x, int32_t screen_pos_y, const struct player_t* player );
typedef void player__update_fxn( struct player_t* player, 
				 const struct background_t* terrain,
				 milliseconds_t frame_length );

enum player__state_e{
  PLAYER__STATE_NONE, /**/
  PLAYER__STATE_STANDING, /**/
  PLAYER__STATE_DUCKING, /**/
  PLAYER__STATE_WALKING, /**/
  PLAYER__STATE_RUNNING, /**/
  PLAYER__STATE_START_JUMPING, /**/
  PLAYER__STATE_JUMPING, /**/
  PLAYER__STATE_CEASE_JUMPING, /**/
  PLAYER__STATE_FALLING, /**/
  PLAYER__STATE_SLIDING_WALL_ON_LEFT, /**/
  PLAYER__STATE_SLIDING_WALL_ON_RIGHT, /**/
  PLAYER__STATE_START_JUMPING_OFF_WALL_ON_LEFT, /**/
  PLAYER__STATE_START_JUMPING_OFF_WALL_ON_RIGHT, /**/
  PLAYER__STATE_JUMPING_OFF_WALL, /**/
  PLAYER__STATE_HANGING_ON_LEDGE, /**/
  PLAYER__STATE_START_CLIMBING_LEDGE, /**/
  PLAYER__STATE_START_BACK_FLIP, /**/
  PLAYER__STATE_BACK_FLIP /**/
};

typedef struct
{
  int per_second_x_acceleration;
  int per_second_x_decceleration;

  int per_second_gravity_acceleration;

  int velocity_limit_walking;
  int velocity_limit_running;
  int velocity_limit_wall_sliding;
  int velocity_limit_falling;

  int jump_initial_y_velocity;
  int jump_x_velocity_percent;
  int jump_from_ledge_initial_y_velocity;
  int wall_jump_initial_y_velocity;
  int wall_jump_initial_x_velocity;
  int jump_final_y_velocity;

  int minimum_backflip_starting_velocity;

} player__config_t;

struct player_prototype_t{

  struct geo__rect_t bounding_box_standing;
  struct geo__rect_t bounding_box_ducking;

  player__config_t config;

  player__draw_fxn* draw_fxn;
  player__update_fxn* update_fxn;
  const struct video__texture_handle_t* texture;
};

struct player_state_info_t
{
  enum player__state_e value;
  timestamp_t timestamp;
};

struct player_t{

  struct geo__point_t position;
  struct geo__vector_t velocity;

  const struct player_prototype_t* prototype;
  const struct control__state_t* control;
  
  bool_t top_collision;
  bool_t bottom_collision;
  bool_t left_collision;
  bool_t right_collision;

  bool_t against_ledge;

  struct player_state_info_t state;

  uint8_t color[3];
};

void player__basic_draw( int32_t screen_pos_x, int32_t screen_pos_y, const struct player_t* player );
void player__basic_update( struct player_t* player, 
			   const struct  background_t* background,
			   milliseconds_t frame_length );




int player__load_config( const char* config_file,
			 struct player_prototype_t* prototype );

enum player__state_e player__calculate_new_state( 
				      const struct player_t* player );

void player__calculate_new_velocity( const struct player_t* player, 
				     milliseconds_t frame_length,
				     struct geo__vector_t* new_velocity );

const struct geo__rect_t* player__get_bounding_box( const struct player_t* player );

#endif
