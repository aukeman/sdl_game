#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>
#include <stdint.h>

struct background_t;
struct player_t;

enum {
  LEVEL__ERROR_CODE_BASE = 0x0,
  
  LEVEL__CONFIG_FILE_NOT_FOUND,
  LEVEL__BAD_CONFIG_FILE,

  LEVEL__ERROR_CODE_LAST
};

struct layer_t{
  struct background_t* background;
  
  int32_t scroll_factor_x;
  int32_t scroll_factor_y;
};

struct level_t {
  struct layer_t terrain_layer;
  struct layer_t* background_layers;
  size_t number_of_background_layers;
};

int level__create( const char* level_config_file, struct level_t** handle_ptr );
int level__free( struct level_t* handle );

int level__update( struct level_t* level,
		   int32_t scroll_position_x,
		   int32_t scroll_position_y );

int level__draw( struct level_t* level );


#endif
