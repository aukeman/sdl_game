#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdint.h>
#include <stddef.h>

enum{
  BACKGROUND__ERROR_CODE_BASE = 0x0,
  
  BACKGROUND__CONFIG_FILE_NOT_FOUND,
  BACKGROUND__BAD_CONFIG_FILE,
  BACKGROUND__IMAGE_NOT_FOUND,

  BACKGROUND__ERROR_CODE_LAST
};


enum background__tile_collision_type_e{
  BACKGROUND__COLLISION_NONE,
  BACKGROUND__COLLISION_TOP,
  BACKGROUND__COLLISION_LEFT,
  BACKGROUND__COLLISION_RIGHT,
  BACKGROUND__COLLISION_BOTTOM,
  BACKGROUND__COLLISION_ALL
};

struct background_t;
struct background__tile_t;
struct background__tile_prototype_t;

int background__create( const char* background_config_file, struct background_t** handle_ptr );
int background__free( struct background_t* handle );

void background__draw( const struct background_t* background );
void background__update( struct background_t* background );

struct background_t {
  
  struct background__tile_t** tiles;
  struct background__tile_prototype_t* tile_prototypes;

  struct video__texture_handle_t* texture;

  uint32_t tiles_wide;
  uint32_t tiles_high;

  uint32_t tile_width; 
  uint32_t tile_height;
};

typedef void background__tile_draw_fxn( size_t idx_x, size_t idx_y, const struct background__tile_t* background );

struct background__tile_prototype_t{

  const struct background_t* background;

  enum background__tile_collision_type_e collision_type;
  
  background__tile_draw_fxn* tile_draw_fxn;

  uint32_t tile_idx_x;
  uint32_t tile_idx_y;
};

struct background__tile_t{
  const struct background__tile_prototype_t* prototype;
};

void background__tile_basic_draw( size_t idx_x, size_t idx_y, const struct background__tile_t* background );

#endif
