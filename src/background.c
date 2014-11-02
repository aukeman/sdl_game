#include <background.h>
#include <constants.h>
#include <video.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int background__create( const char* background_config_file, 
			struct background_t** handle_ptr ){

  *handle_ptr = malloc(sizeof(**handle_ptr));
  memset( *handle_ptr, '\0', sizeof(**handle_ptr));

  FILE* fin = fopen(background_config_file, "r");

  if ( !fin ) {
    background__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);
    return BACKGROUND__CONFIG_FILE_NOT_FOUND;
  }

  const int BUFFER_LENGTH=1023;
  char buffer[BUFFER_LENGTH+1];

  int rc = fscanf(fin, "%1023s%*c", buffer);
  
  if ( rc < 1 ){
    background__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }
  else if ( SUCCESS != video__setup_texture(buffer, &(*handle_ptr)->texture) ){
    background__free(*handle_ptr);
    *handle_ptr = NULL;
    
    fclose(fin);
    return BACKGROUND__IMAGE_NOT_FOUND;
  }

  rc = fscanf(fin, "%d %d%*c", 
	      &(*handle_ptr)->tile_width, &(*handle_ptr)->tile_height);
	
  if ( rc != 2 ) {
    background__free(*handle_ptr);
    *handle_ptr = NULL;
    
    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }

  int number_of_prototypes;

  rc = fscanf(fin, "%d%*c", &number_of_prototypes);
	
  if ( rc != 1 ) {
    background__free(*handle_ptr);
    *handle_ptr = NULL;
    
    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }

  size_t number_of_prototype_bytes = 
    number_of_prototypes*sizeof(struct background__tile_prototype_t);

  (*handle_ptr)->tile_prototypes = malloc(number_of_prototype_bytes);
  memset((*handle_ptr)->tile_prototypes, '\0', number_of_prototype_bytes);

  struct background__tile_prototype_t* prototype_dictionary[255];
  memset( &prototype_dictionary, '\0', sizeof(prototype_dictionary) );

  int prototype_idx;
  for ( prototype_idx = 0; prototype_idx < number_of_prototypes; ++prototype_idx ){
    
    char prototype_label;
    struct background__tile_prototype_t* prototype = 
      &(*handle_ptr)->tile_prototypes[prototype_idx];

    rc = fscanf(fin, "%c %d %d %d%*c", 
		&prototype_label,
		&(prototype->tile_idx_x),
		&(prototype->tile_idx_y),
		(int*)&(prototype->collision_type));

    if ( rc != 4 ) {
      background__free(*handle_ptr);
      *handle_ptr = NULL;
    
      fclose(fin);
      return BACKGROUND__BAD_CONFIG_FILE;
    }

    prototype->tile_draw_fxn = &background__tile_basic_draw;
    prototype->background = *handle_ptr;

    prototype_dictionary[prototype_label] = prototype;
  }

  rc = fscanf(fin, "%d %d%*c", 
	      &(*handle_ptr)->tiles_wide,
	      &(*handle_ptr)->tiles_high);

  if ( rc != 2 ) {
    background__free(*handle_ptr);
    *handle_ptr = NULL;
    
    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }

  size_t number_of_tile_col_bytes = ((*handle_ptr)->tiles_wide*
				     sizeof(struct background__tile_t*));

  size_t number_of_tile_row_bytes = ((*handle_ptr)->tiles_high*
				     sizeof(struct background__tile_t));

  (*handle_ptr)->tiles = malloc(number_of_tile_col_bytes);
  memset((*handle_ptr)->tiles, '\0', number_of_tile_col_bytes);
  
  size_t row_idx, col_idx;
  for ( col_idx = 0; col_idx < (*handle_ptr)->tiles_wide; ++col_idx ){
    (*handle_ptr)->tiles[col_idx] = malloc(number_of_tile_row_bytes);
    memset((*handle_ptr)->tiles[col_idx], '\0', number_of_tile_row_bytes);
  }

  for ( row_idx = 0; row_idx < (*handle_ptr)->tiles_high; ++row_idx ){
    for ( col_idx = 0; col_idx < (*handle_ptr)->tiles_wide; ++col_idx ){
      int tile_label = fgetc(fin);

      if ( tile_label != ' ' && prototype_dictionary[tile_label] == NULL ){
	background__free(*handle_ptr);
	*handle_ptr = NULL;
      
	fclose(fin);
	return BACKGROUND__BAD_CONFIG_FILE;
      }

      (*handle_ptr)->tiles[col_idx][row_idx].prototype = 
	prototype_dictionary[tile_label];
    }

    int tile_label = fgetc(fin);
    if ( tile_label != '\n' ){ /* TODO: check for \r\n */
      background__free( *handle_ptr );
      *handle_ptr = NULL;
      
      fclose(fin);
      return BACKGROUND__BAD_CONFIG_FILE;
    }
  }

  fclose(fin);
  return SUCCESS;
}

int background__free( struct background_t* handle ){

  if ( handle->texture != NULL ){
    video__teardown_texture(handle->texture);
  }

  size_t col_idx;
  for ( col_idx = 0; col_idx < handle->tiles_wide; ++col_idx){
    free(handle->tiles[col_idx]);
  }

  free(handle->tiles);
  free(handle->tile_prototypes);
  free(handle);

  return SUCCESS;
}

void background__draw( const struct background_t* background ){

  int col_idx, row_idx;

  video__begin_blits(NULL);
  for ( col_idx = 0; col_idx < background->tiles_wide; ++col_idx ){
    for ( row_idx = 0; row_idx < background->tiles_high; ++row_idx ){
      if ( background->tiles[col_idx][row_idx].prototype ){
	background->tiles[col_idx][row_idx].prototype->
	  tile_draw_fxn( col_idx,
			 row_idx,
			 &background->tiles[col_idx][row_idx] );
      }
    }
  }
  video__end_blits();
}

void background__tile_basic_draw( size_t idx_x, 
				  size_t idx_y, 
				  const struct background__tile_t* tile ){
  geo__rect_t src = { 0, 0, 
		      tile->prototype->background->tile_width, 
		      tile->prototype->background->tile_height };

  geo__rect_t dest = { 0, 0, 
		      tile->prototype->background->tile_width, 
		      tile->prototype->background->tile_height };

  src.x = tile->prototype->tile_idx_x * tile->prototype->background->tile_width;
  src.y = tile->prototype->tile_idx_y * tile->prototype->background->tile_height;

  dest.x = idx_x * tile->prototype->background->tile_width;
  dest.y = idx_y * tile->prototype->background->tile_height;

  video__blit( tile->prototype->background->texture, &src, &dest );
}

