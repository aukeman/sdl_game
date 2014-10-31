#include <background.h>

#include <video.h>


void background__basic_draw( size_t idx_x, 
			     size_t idx_y, 
			     const struct background_t* background )
{
  static geo__rect_t src = { 0, 0, 
			     BACKGROUND_TILE_WIDTH, BACKGROUND_TILE_HEIGHT };

  static geo__rect_t dest = { 0, 0, 
			      BACKGROUND_TILE_WIDTH, BACKGROUND_TILE_HEIGHT };

  dest.x = idx_x * BACKGROUND_TILE_WIDTH;
  dest.y = idx_y * BACKGROUND_TILE_HEIGHT;

  video__blit( background->prototype->texture, &src, &dest );
}
