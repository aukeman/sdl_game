#ifndef VIDEO_H
#define VIDEO_H

#include <geometry.h>

#include <stdint.h>

enum {
  VIDEO__ERROR_CODE_BASE = 0x0,
  VIDEO__COULD_NOT_CREATE_SURFACE,
  VIDEO__COULD_NOT_INITIALIZE_SDL,
  VIDEO__ERROR_CODE_LAST
};

struct video__screen_extents_t {
  uint32_t width;
  uint32_t height;
  
  int fullscreen;
};

struct video__texture_data_t {
  uint32_t texture_id;
  uint32_t width;
  uint32_t height;
};

const struct video__screen_extents_t* video__get_screen_extents();

int video__setup(uint32_t width, uint32_t height, int fullscreen);
int video__teardown();


int video__clearscreen();

int video__flip();

int video__setup_texture(const char* file_directory, 
			 struct video__texture_data_t* texture_data_ptr );

int video__teardown_texture(struct video__texture_data_t* texture_data_ptr);

int video__blit(const struct video__texture_data_t* texture_data_ptr, 
		const struct geo__rect_t* src,
		const struct geo__rect_t* dest);

#endif
