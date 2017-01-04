#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <types.h>

#include <stdint.h>
#include <stddef.h>

struct geo__rect_t;
struct geo__vector_t;

enum{
  BACKGROUND__ERROR_CODE_BASE = 0x0,
  
  BACKGROUND__CONFIG_FILE_NOT_FOUND,
  BACKGROUND__BAD_CONFIG_FILE,
  BACKGROUND__IMAGE_NOT_FOUND,

  BACKGROUND__ERROR_CODE_LAST
};


enum background__tile_collision_type_e{
  BACKGROUND__COLLISION_NONE = 0x0,
  BACKGROUND__COLLISION_TOP = 0x1,
  BACKGROUND__COLLISION_LEFT = 0x2,
  BACKGROUND__COLLISION_RIGHT = 0x4,
  BACKGROUND__COLLISION_BOTTOM = 0x8,
  BACKGROUND__COLLISION_ALL = 0x0F
};

struct background_t;
struct background__tile_t;
struct background__tile_prototype_t;

int background__create( const char* background_config_file, struct background_t** handle_ptr );
int background__free( struct background_t* handle );

void background__draw( const struct background_t* background, const struct geo__rect_t* );

struct background_t {
  
  struct background__tile_t** tiles;
  struct background__tile_prototype_t* tile_prototypes;

  struct video__texture_handle_t* texture;

  bool_t transparency_allowed;

  uint32_t tiles_wide;
  uint32_t tiles_high;

  uint32_t tile_width; 
  uint32_t tile_height;

  uint32_t tile_screen_width; 
  uint32_t tile_screen_height;

  int32_t scroll_position_x;
  int32_t scroll_position_y;
};

typedef void background__tile_draw_fxn( size_t idx_x, size_t idx_y, const struct background__tile_t* background );

struct background__tile_prototype_t{

  const struct background_t* background;

  enum background__tile_collision_type_e collision_type;
  
  background__tile_draw_fxn* tile_draw_fxn;

  uint32_t tile_idx_x;
  uint32_t tile_idx_y;

  struct{
    float x1;
    float y1;
    float x2;
    float y2;
  } texture_src;
};

struct background__tile_t{

  const struct background__tile_prototype_t* prototype;
};

void background__scroll_to( struct background_t* background,
			    int32_t position_x, 
			    int32_t position_y );

void background__tile_basic_draw( size_t idx_x, 
				  size_t idx_y, 
				  const struct background__tile_t* tile );

bool_t background__collision_test( const struct background_t* background,
				   const struct geo__rect_t* position,
				   struct geo__vector_t* velocity,
				   bool_t* top_collision,
				   bool_t* bottom_collision,
				   bool_t* left_collision,
				   bool_t* right_collision );

#endif
