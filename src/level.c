#include <level.h>
#include <background.h>
#include <player.h>
#include <video.h>
#include <constants.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool_t _create_layers( FILE*,
		       struct layer_t**,
		       size_t* number_of_layers_ptr );

int level__create( const char* level_config_file, struct level_t** handle_ptr ){

  FILE* fin = NULL;
  char buffer[1024];

  *handle_ptr = malloc( sizeof(**handle_ptr) );
  memset(*handle_ptr, '\0', sizeof(**handle_ptr) );

  fin = fopen(level_config_file, "r");
  if ( !fin ) {
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);
    return LEVEL__CONFIG_FILE_NOT_FOUND;
  }

  if ( fscanf(fin, "%1023s%*c", buffer) < 1 ){
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  (*handle_ptr)->terrain_layer.scroll_factor_x = 1;
  (*handle_ptr)->terrain_layer.scroll_factor_y = 1;

  if ( SUCCESS != 
       background__create( buffer, 
			   &(*handle_ptr)->terrain_layer.background ) ){

    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  if ( !_create_layers( fin,
			&(*handle_ptr)->background_layers,
			&(*handle_ptr)->number_of_background_layers ) ||
       !_create_layers( fin,
			&(*handle_ptr)->foreground_layers,
			&(*handle_ptr)->number_of_foreground_layers ) ){
    level__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);

    return LEVEL__BAD_CONFIG_FILE;
  }

  return SUCCESS;
}

bool_t _create_layers( FILE* fin,
		       struct layer_t** layers_ptr,
		       size_t* number_of_layers_ptr ){

  char buffer[1024];
  size_t idx = 0;
  int rc = fscanf(fin, "%u*c", number_of_layers_ptr);
  
  if ( rc < 1 ){
    return FALSE;
  }

  *layers_ptr = 
    malloc(sizeof(struct layer_t)*(*number_of_layers_ptr));

  memset( *layers_ptr, '\0', 
	  sizeof(struct layer_t)*(*number_of_layers_ptr) );

  for ( idx = 0; idx < (*number_of_layers_ptr); ++idx ){
    
    rc = fscanf(fin, "%1023s %d %d*c", 
		buffer,
		&(*layers_ptr)[idx].scroll_factor_x,
		&(*layers_ptr)[idx].scroll_factor_y);

    if ( rc < 3 ){
      return FALSE;
    }

    if ( SUCCESS != 
	 background__create( buffer, 
			     &(*layers_ptr)[idx].background ) ){

      return FALSE;
    }
  }

  return TRUE;
}

int level__free( struct level_t* handle ){

  int result = UNKNOWN_FAILURE;

  if ( handle ){

    size_t idx;
    for ( idx = 0; idx < handle->number_of_background_layers; ++idx ){
      background__free( handle->background_layers[idx].background );
    }

    free( handle->background_layers );

    for ( idx = 0; idx < handle->number_of_foreground_layers; ++idx ){
      background__free( handle->foreground_layers[idx].background );
    }

    free( handle->foreground_layers );

    background__free( handle->terrain_layer.background );

    free(handle);

    result = SUCCESS;
  }

  return result;
}

int level__update( struct level_t* level,
		   int32_t scroll_position_x,
		   int32_t scroll_position_y ){

  int32_t terrain_scroll_position_x = 0, terrain_scroll_position_y = 0, idx = 0;

  background__scroll_to( level->terrain_layer.background,
			 scroll_position_x, 
			 scroll_position_y );

  terrain_scroll_position_x = level->terrain_layer.background->scroll_position_x;
  terrain_scroll_position_y = level->terrain_layer.background->scroll_position_y;

  for ( idx = 0; idx < level->number_of_background_layers; ++idx ){

    struct layer_t* background_layer = &level->background_layers[idx];

    int32_t background_scroll_position_x = 0;
    int32_t background_scroll_position_y = 0;

    if ( 0 < background_layer->scroll_factor_x ){
      background_scroll_position_x = 
	(terrain_scroll_position_x/background_layer->scroll_factor_x);
    }

    if ( 0 < background_layer->scroll_factor_y ){
      background_scroll_position_y = 
	(terrain_scroll_position_y/background_layer->scroll_factor_y);
    }

    background__scroll_to( background_layer->background, 
			   background_scroll_position_x,
			   background_scroll_position_y );
  }


  for ( idx = 0; idx < level->number_of_foreground_layers; ++idx ){

    struct layer_t* foreground_layer = &level->foreground_layers[idx];

    int32_t foreground_scroll_position_x = 
      (terrain_scroll_position_x*foreground_layer->scroll_factor_x);

    int32_t foreground_scroll_position_y = 
      (terrain_scroll_position_y*foreground_layer->scroll_factor_y);

    background__scroll_to( foreground_layer->background, 
			   foreground_scroll_position_x,
			   foreground_scroll_position_y );
  }

  return SUCCESS;
}

int level__draw( struct level_t* level ){

  size_t idx;
  for ( idx = 0; idx < level->number_of_background_layers; ++idx ){
    background__draw( level->background_layers[idx].background );
  }

  background__draw( level->terrain_layer.background );

  for ( idx = 0; idx < level->number_of_foreground_layers; ++idx ){
    background__draw( level->foreground_layers[idx].background );
  }

  return SUCCESS;
}
