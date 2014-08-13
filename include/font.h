#ifndef FONT_H
#define FONT_H

#include <constants.h>

struct font__handle_t;

enum{
  FONT__ERROR_CODE_BASE = 0x0,
  
  FONT__CONFIG_FILE_NOT_FOUND,
  FONT__BAD_CONFIG_FILE,
  FONT__IMAGE_NOT_FOUND,
  FONT__STRING_TOO_LONG,
  FONT__NO_SPACE_CHAR_DIMENSIONS,

  FONT__ERROR_CODE_LAST
};

int font__create( const char* font_config_file, struct font__handle_t** handle_ptr);

int font__free( struct font__handle_t* handle );

int font__draw_string( const struct font__handle_t* handle, int x, int y, const char* fmt, ... );

int font__dimensions( const struct font__handle_t* handle, int* width, int* height, const char* fmt, ... );
#endif
