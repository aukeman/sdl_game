#ifndef LEVEL_H
#define LEVEL_H

#include <stddef.h>
#include <stdint.h>

struct background_t;
struct player_t;
struct geo__rect_t;
struct camera_t;

enum {
  LEVEL__ERROR_CODE_BASE = 0x0,
  
  LEVEL__CONFIG_FILE_NOT_FOUND,
  LEVEL__BAD_CONFIG_FILE,

  LEVEL__ERROR_CODE_LAST
};

struct layer_t{
  struct background_t* background;
  
  float scroll_factor_x;
  float scroll_factor_y;
};

struct level_t {
  struct layer_t terrain_layer;
  struct layer_t* background_layers;
  struct layer_t* foreground_layers;
  size_t number_of_background_layers;
  size_t number_of_foreground_layers;
};

int level__create( const char* level_config_file, struct level_t** handle_ptr );
int level__free( struct level_t* handle );

int level__draw( const struct level_t*, const struct camera_t* );

int level__get_bounds( const struct level_t*, struct geo__rect_t* );

#endif
