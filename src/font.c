#include <font.h>
#include <video.h>
#include <geometry.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
