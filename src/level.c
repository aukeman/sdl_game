#include <level.h>
#include <background.h>
#include <player.h>
#include <video.h>
#include <constants.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int level__create( const char* level_config_file, struct level_t** handle_ptr ){

  int result = UNKNOWN_FAILURE;

  *handle_ptr = malloc( sizeof(**handle_ptr) );
  memset(*handle_ptr, '\0', sizeof(**handle_ptr) );

  FILE* fin = fopen(level_config_file, "r");
  if ( !fin ) {
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);
    return LEVEL__CONFIG_FILE_NOT_FOUND;
  }

  const int BUFFER_LENGTH=1023;
  char buffer[BUFFER_LENGTH+1];

  int rc = fscanf(fin, "%1023s%*c", buffer);

  if ( rc < 1 ){
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  if ( SUCCESS != 
       background__create( buffer, 
			   &(*handle_ptr)->terrain_layer.background ) ){

    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  rc = fscanf(fin, "%u*c", 
	      &(*handle_ptr)->number_of_background_layers);
  
  if ( rc < 1 ){
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  (*handle_ptr)->background_layers = 
    malloc(sizeof(struct layer_t)*(*handle_ptr)->number_of_background_layers);

  memset( (*handle_ptr)->background_layers, '\0', 
	  sizeof(struct layer_t)*(*handle_ptr)->number_of_background_layers );

  size_t idx;
  for ( idx = 0; idx < (*handle_ptr)->number_of_background_layers; ++idx ){
    
    rc = fscanf(fin, "%1023s%*c", buffer);

    if ( rc < 1 ){
      level__free(*handle_ptr);
      *handle_ptr = NULL;

      fclose(fin);

      return LEVEL__BAD_CONFIG_FILE;
    }

    if ( SUCCESS != 
	 background__create( buffer, 
			     &(*handle_ptr)->background_layers[idx].background ) ){

      level__free(*handle_ptr);
      *handle_ptr = NULL;

      fclose(fin);

      return LEVEL__BAD_CONFIG_FILE;
    }
  }

  result = SUCCESS;

  return result;
}

int level__free( struct level_t* handle ){

  int result = UNKNOWN_FAILURE;

  if ( handle ){

    size_t bg_idx;
    for ( bg_idx = 0; bg_idx < handle->number_of_background_layers; ++bg_idx ){
      background__free( handle->background_layers[bg_idx].background );
    }

    background__free( handle->terrain_layer.background );

    free(handle);

    result = SUCCESS;
  }

  return result;
}

int level__update( struct level_t* level,
		   int32_t scroll_position_x,
		   int32_t scroll_position_y ){

  background__scroll_to( level->terrain_layer.background,
			 scroll_position_x, 
			 scroll_position_y );

  return SUCCESS;
}

int level__draw( struct level_t* level ){

  size_t idx;
  for ( idx = 0; idx < level->number_of_background_layers; ++idx ){
    background__draw( level->background_layers[idx].background );
  }

  background__draw( level->terrain_layer.background );
}
