#include <font.h>
#include <video.h>
#include <geometry.h>
#include <types.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

typedef geo__rect_t ascii_to_rect_t[256];

struct font__handle_t {

  struct video__texture_handle_t* texture;

  ascii_to_rect_t ascii_to_rect;

} ;

int font__create( const char* font_config_file, struct font__handle_t** handle_ptr){
  int result = SUCCESS;

  *handle_ptr = (struct font__handle_t*)malloc(sizeof(**handle_ptr));

  memset(*handle_ptr, '\0', sizeof(**handle_ptr));

  FILE* fin = fopen(font_config_file, "r");

  if ( !fin ) {
    result = FONT__CONFIG_FILE_NOT_FOUND;
    free(*handle_ptr);
    *handle_ptr = NULL;
  }
  else {
    
    const int BUFFER_LENGTH=1023;
    char buffer[BUFFER_LENGTH+1];

    int rc = fscanf(fin, "%s%*c", buffer);

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

  return result;
}

int font__free( struct font__handle_t* handle ){

  video__teardown_texture(handle->texture);
  free(handle);
}


int font__draw_string( const struct font__handle_t* handle, int x, int y, const char* fmt, ... ){

  geo__rect_t dest = {x, y, 0, 0};

  va_list ap;
  va_start(ap, fmt);
  int result = _process_string(handle, &dest, TRUE, fmt, ap);
  va_end(ap);
  
  return result;
}

int font__dimensions( const struct font__handle_t* handle, int* width, int* height, const char* fmt, ... ){

  geo__rect_t dest = {0, 0, 0, 0};

  *width = 0;
  *height = 0;

  va_list ap;
  va_start(ap, fmt);
  int result = _process_string(handle, &dest, FALSE, fmt, ap);
  va_end(ap);
  
  if ( result == SUCCESS ){
    *width = dest.width;
    *height = dest.height;
  }

  return result;
}

int _process_string( const struct font__handle_t* handle, geo__rect_t* dest, bool_t render, const char* fmt, va_list ap ){

  static char buffer[1024];

  size_t formatted_string_length = vsnprintf(buffer, 1023, fmt, ap);

  if ( 1023 < formatted_string_length ){
    return FONT__STRING_TOO_LONG;
  }

  int original_x = dest->x;

  const char* iter = buffer;
  while ( *iter != '\0' ){
    const geo__rect_t* src = &handle->ascii_to_rect[*buffer];

    switch (*iter){
    case ' ':
      dest->x += src->width;
      break;
    case '\t':
      dest->x += (handle->ascii_to_rect[' '].width*4);
      break;
    case '\n':
      dest->x = original_x;
      dest->y += handle->ascii_to_rect[' '].height;
      break;
    default:
      if ( 0 < src->width && 0 < src->height ){
	dest->width = src->width;
	dest->height = src->height;

	if ( render ){
	  video__blit(handle->texture, src, dest);
	}

	dest->x += src->width;
      }
    }

    ++iter;
  }

  return SUCCESS;
}

