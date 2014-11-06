#include <background.h>
#include <constants.h>
#include <video.h>
#include <utils.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

static struct 
video__blit_params_t blit_params = { NULL, FALSE, {0, 0, 0, 0}, TRUE };

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

    uint32_t texture_width = video__get_texture_width((*handle_ptr)->texture);
    uint32_t texture_height = video__get_texture_height((*handle_ptr)->texture);

    prototype->texture_src.x1 = 
      (float)(prototype->tile_idx_x * (*handle_ptr)->tile_width) / 
      (float)texture_width;

    prototype->texture_src.y1 = 
      (float)(prototype->tile_idx_y * (*handle_ptr)->tile_height) / 
      (float)texture_height;

    prototype->texture_src.x2 = 
      (float)((prototype->tile_idx_x+1) * (*handle_ptr)->tile_width) / 
      (float)texture_width;

    prototype->texture_src.y2 = 
      (float)((prototype->tile_idx_y+1) * (*handle_ptr)->tile_height) / 
      (float)texture_height;

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

void background__draw( int32_t pos_x, 
		       int32_t pos_y, 
		       const struct background_t* background ){

  int32_t screen_pos_x = utils__pos2screen(pos_x);
  int32_t screen_pos_y = utils__pos2screen(pos_y);

  int32_t min_col_idx = screen_pos_x / background->tile_width;
  int32_t min_row_idx = screen_pos_y / background->tile_height;

  int32_t max_col_idx = min_col_idx + (video__get_screen_extents()->viewport_width / background->tile_width) + 1;
  int32_t max_row_idx = min_row_idx + (video__get_screen_extents()->viewport_height / background->tile_height) + 1;

  if ( background->tiles_wide < max_col_idx ){
    max_col_idx = background->tiles_wide;
  }
  
  if ( background->tiles_high < max_row_idx ){
    max_row_idx = background->tiles_high;
  }

  int32_t col_idx, row_idx;

  video__translate( -screen_pos_x, -screen_pos_y );

  blit_params.texture_handle = background->texture;
  video__begin_blits(&blit_params);

  for ( col_idx = min_col_idx; col_idx < max_col_idx; ++col_idx ){
    for ( row_idx = min_row_idx; row_idx < max_row_idx; ++row_idx ){

      if ( background->tiles[col_idx][row_idx].prototype ){
	background->tiles[col_idx][row_idx].prototype->
	  tile_draw_fxn( col_idx,
			 row_idx,
			 &background->tiles[col_idx][row_idx] );
      }
    }
  }
  video__end_blits();
  video__translate( screen_pos_x, screen_pos_y );
}

void background__tile_basic_draw( size_t idx_x, 
				  size_t idx_y, 
				  const struct background__tile_t* tile ){

  video__blit_verts( tile->prototype->texture_src.x1, 
		     tile->prototype->texture_src.y1, 
		     tile->prototype->texture_src.x2, 
		     tile->prototype->texture_src.y2,
		     idx_x * tile->prototype->background->tile_width,
		     idx_y * tile->prototype->background->tile_height,
		     (idx_x+1) * tile->prototype->background->tile_width,
		     (idx_y+1) * tile->prototype->background->tile_height );
		     
}

