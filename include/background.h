#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <stdint.h>
#include <stddef.h>

#define BACKGROUND_TILE_WIDTH 32
#define BACKGROUND_TILE_HEIGHT 32

enum background__collision_type_e{
  BACKGROUND__COLLISION_NONE,
  BACKGROUND__COLLISION_TOP,
  BACKGROUND__COLLISION_LEFT,
  BACKGROUND__COLLISION_RIGHT,
  BACKGROUND__COLLISION_BOTTOM,
  BACKGROUND__COLLISION_ALL
};

struct background_t;

typedef void background__draw_fxn( size_t idx_x, size_t idx_y, const struct background_t* background );

struct background_prototype_t{
  background__draw_fxn* draw_fxn;
  const struct video__texture_handle_t* texture;

  enum background__collision_type_e collision_type;
};

struct background_t{
  const struct background_prototype_t* prototype;
  uint8_t render_flags;
};

void background__basic_draw( size_t idx_x, size_t idx_y, const struct background_t* background );

#endif
