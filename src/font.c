#include <font.h>
#include <video.h>
#include <geometry.h>
#include <types.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

typedef struct geo__rect_t ascii_to_rect_t[256];

struct font__handle_t {

  struct video__texture_handle_t* texture;

  ascii_to_rect_t ascii_to_rect;

} ;

static struct 
video__blit_params_t blit_params = { NULL, FALSE, { 0, 0, 0, 0 }, FALSE };

int _process_string( const struct font__handle_t*, struct geo__rect_t*, bool_t, const char*, va_list ap );

int font__create( const char* font_config_file, struct font__handle_t** handle_ptr){
  int result = SUCCESS;
  FILE* fin = NULL;

  *handle_ptr = (struct font__handle_t*)malloc(sizeof(**handle_ptr));

  memset(*handle_ptr, '\0', sizeof(**handle_ptr));

  fin = fopen(font_config_file, "r");
  if ( !fin ) {
    result = FONT__CONFIG_FILE_NOT_FOUND;
    free(*handle_ptr);
    *handle_ptr = NULL;
  }
  else {
    
    char buffer[1024];

    int rc = fscanf(fin, "%1023s%*c", buffer);

    if ( rc < 1 ){
      result = FONT__BAD_CONFIG_FILE;
      free(*handle_ptr);
      *handle_ptr = NULL;
    }
    else if ( SUCCESS != video__setup_texture(buffer, &(*handle_ptr)->texture) ){
      result = FONT__IMAGE_NOT_FOUND;
      free(*handle_ptr);
      *handle_ptr = NULL;
    }
    else{

      uint8_t ascii;
      uint32_t x, y, width, height;

      do{
	rc = fscanf(fin, "%c %u %u %u %u%*c", 
		    &ascii, &x, &y, &width, &height);
	
	if ( rc == 5 ){
	  (*handle_ptr)->ascii_to_rect[ascii].x = x;
	  (*handle_ptr)->ascii_to_rect[ascii].y = y;
	  (*handle_ptr)->ascii_to_rect[ascii].width = width;
	  (*handle_ptr)->ascii_to_rect[ascii].height = height;
	}
      } while ( rc != EOF );
    }
  
    fclose(fin);
  }

  if ( *handle_ptr &&
       ((*handle_ptr)->ascii_to_rect[' '].width == 0 ||
	(*handle_ptr)->ascii_to_rect[' '].height == 0 )){

    font__free( *handle_ptr );
    *handle_ptr = NULL;

    result = FONT__NO_SPACE_CHAR_DIMENSIONS;
  }

  if ( *handle_ptr &&
       (*handle_ptr)->ascii_to_rect['\t'].width == 0 ){
    (*handle_ptr)->ascii_to_rect['\t'].width = 
      (*handle_ptr)->ascii_to_rect[' '].width*4;
  }

  if ( *handle_ptr &&
       (*handle_ptr)->ascii_to_rect['\n'].height == 0 ){
    (*handle_ptr)->ascii_to_rect['\n'].height = 
      (*handle_ptr)->ascii_to_rect[' '].height;
  }

  return result;
}

int font__free( struct font__handle_t* handle ){

  video__teardown_texture(handle->texture);
  free(handle);

  return SUCCESS;
}

int font__draw_string( const struct font__handle_t* handle, int x, int y, const char* fmt, ... ){

  int result = 0;
  va_list ap;

  struct geo__rect_t dest;
  geo__init_rect(&dest, x, y, 0, 0);

  va_start(ap, fmt);
  result = _process_string(handle, &dest, TRUE, fmt, ap);
  va_end(ap);
  
  return result;
}

int font__dimensions( const struct font__handle_t* handle, int* width, int* height, const char* fmt, ... ){

  int result = 0;
  struct geo__rect_t dest;
  va_list ap;

  geo__init_rect(&dest, 0, 0, 0, 0);

  *width = 0;
  *height = 0;

  va_start(ap, fmt);
  result = _process_string(handle, &dest, FALSE, fmt, ap);
  va_end(ap);
  
  if ( result == SUCCESS ){
    *width = dest.x;
    *height = dest.y;
  }

  return result;
}

int _process_string( const struct font__handle_t* handle, struct geo__rect_t* dest, bool_t render, const char* fmt, va_list ap ){

  static char buffer[1024];
  int original_x = 0;
  const char* iter = NULL;

  size_t formatted_string_length = vsprintf(buffer, fmt, ap);

  if ( 1023 < formatted_string_length ){
    return FONT__STRING_TOO_LONG;
  }

  original_x = dest->x;
  
  blit_params.texture_handle = handle->texture;

  video__begin_blits(&blit_params);

  iter = buffer;
  while ( *iter != '\0' ){
    const struct geo__rect_t* src = &handle->ascii_to_rect[(unsigned char)*iter];

    switch (*iter){
    case ' ':
    case '\t':
      if ( render && (src->x != 0 || src->y != 0) ){

	dest->width = src->width;
	dest->height = src->height;

	video__blit(src, dest);
      }
      dest->x += src->width;
      break;

    case '\n':

      if ( render && (src->x != 0 || src->y != 0) ){

	dest->width = src->width;
	dest->height = src->height;

	video__blit(src, dest);
      }

      dest->x = original_x;
      dest->y += src->height;
      break;

    default:
      if ( 0 < src->width && 0 < src->height ){
	dest->width = src->width;
	dest->height = src->height;

	if ( render ){
	  video__blit(src, dest);
	}
      }

      dest->x += src->width;
    }

    ++iter;
  }

  video__end_blits();

  if ( iter != buffer ){
    dest->y += handle->ascii_to_rect['\n'].height;
  }

  return SUCCESS;
}

