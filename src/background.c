#include <background.h>
#include <constants.h>
#include <video.h>
#include <utils.h>
#include <geometry.h>

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

  rc = fscanf(fin, "%d*c", 
	      &(*handle_ptr)->transparency_allowed);
  
  if ( rc < 1 ){
    background__free(*handle_ptr);
    *handle_ptr = NULL;

    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }

  rc = fscanf(fin, "%d %d%*c", 
	      &(*handle_ptr)->tile_screen_width, &(*handle_ptr)->tile_screen_height);
	
  if ( rc != 2 ) {
    background__free(*handle_ptr);
    *handle_ptr = NULL;
    
    fclose(fin);
    return BACKGROUND__BAD_CONFIG_FILE;
  }

  (*handle_ptr)->tile_width = utils__screen2pos((*handle_ptr)->tile_screen_width);
  (*handle_ptr)->tile_height = utils__screen2pos((*handle_ptr)->tile_screen_height);

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

    rc = fscanf(fin, "%c %d %d %x", 
		&prototype_label,
		&(prototype->tile_idx_x),
		&(prototype->tile_idx_y),
		(unsigned int*)&(prototype->collision_type));

    if ( rc != 4 ) {
      background__free(*handle_ptr);
      *handle_ptr = NULL;
    
      fclose(fin);
      return BACKGROUND__BAD_CONFIG_FILE;
    }

    prototype->top_surface_vector.x = (*handle_ptr)->tile_width;
    prototype->top_surface_vector.y = 0;

    prototype->top_surface_origin.x = 0;
    prototype->top_surface_origin.y = 0;

    prototype->bottom_surface_vector.x = (*handle_ptr)->tile_width;
    prototype->bottom_surface_vector.y = 0;

    prototype->bottom_surface_origin.x = 0;
    prototype->bottom_surface_origin.y = (*handle_ptr)->tile_height;

    int32_t origin_y = 0;
    int32_t vector_y = 0;

    rc = fscanf(fin, " %d %d%*c",
		&origin_y,
		&vector_y);

    if ( 2 <= rc ){
      
      if ( origin_y < 0 ){
	/* bottom surface */
	prototype->bottom_surface_origin.y = -(origin_y);
	prototype->bottom_surface_vector.y = vector_y;
      }
      else{
	/* top surface */
	prototype->top_surface_origin.y = (*handle_ptr)->tile_height - origin_y;
	prototype->top_surface_vector.y = vector_y;
      }
    }

    prototype->tile_draw_fxn = &background__tile_basic_draw;
    prototype->background = *handle_ptr;

    uint32_t texture_width = video__get_texture_width((*handle_ptr)->texture);
    uint32_t texture_height = video__get_texture_height((*handle_ptr)->texture);

    prototype->texture_src.x1 = 
      (float)(prototype->tile_idx_x * (*handle_ptr)->tile_screen_width) / 
      (float)texture_width;

    prototype->texture_src.y1 = 
      (float)(prototype->tile_idx_y * (*handle_ptr)->tile_screen_height) / 
      (float)texture_height;

    prototype->texture_src.x2 = 
      (float)((prototype->tile_idx_x+1) * (*handle_ptr)->tile_screen_width) / 
      (float)texture_width;

    prototype->texture_src.y2 = 
      (float)((prototype->tile_idx_y+1) * (*handle_ptr)->tile_screen_height) / 
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

  if ( handle ){

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
  }

  return SUCCESS;
}

void background__draw( const struct background_t* background ){

  int32_t screen_pos_x = utils__pos2screen(background->scroll_position_x);
  int32_t screen_pos_y = utils__pos2screen(background->scroll_position_y);

  int32_t min_col_idx = screen_pos_x / background->tile_screen_width;
  int32_t min_row_idx = screen_pos_y / background->tile_screen_height;

  int32_t max_col_idx = min_col_idx + (video__get_screen_extents()->viewport_screen_width / background->tile_screen_width) + 1;
  int32_t max_row_idx = min_row_idx + (video__get_screen_extents()->viewport_screen_height / background->tile_screen_height) + 1;

  if ( background->tiles_wide < max_col_idx ){
    max_col_idx = background->tiles_wide;
  }
  
  if ( background->tiles_high < max_row_idx ){
    max_row_idx = background->tiles_high;
  }

  int32_t col_idx, row_idx;

  video__translate( -screen_pos_x, -screen_pos_y );

  blit_params.texture_handle = background->texture;
  blit_params.suppress_transparency = !background->transparency_allowed;
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

void background__scroll_to( struct background_t* background,
			    int32_t position_x, 
			    int32_t position_y ){
  
  int minimum_scroll_position_x = 0;
  int minimum_scroll_position_y = 0;
  int maximum_scroll_position_x = background->tiles_wide*background->tile_width - video__get_screen_extents()->viewport_position_width;
  int maximum_scroll_position_y = background->tiles_high*background->tile_height - video__get_screen_extents()->viewport_position_height;
    
  background->scroll_position_x = position_x;
  background->scroll_position_y = position_y;

  if ( background->scroll_position_x < minimum_scroll_position_x ){
    background->scroll_position_x = minimum_scroll_position_x;
  }
  else if ( maximum_scroll_position_x < background->scroll_position_x ){
    background->scroll_position_x = maximum_scroll_position_x;
  }
  
  if ( background->scroll_position_y < minimum_scroll_position_y ){
    background->scroll_position_y = minimum_scroll_position_y;
  }
  else if ( maximum_scroll_position_y < background->scroll_position_y ){
    background->scroll_position_y = maximum_scroll_position_y;
  }
}

void background__tile_basic_draw( size_t idx_x, 
				  size_t idx_y, 
				  const struct background__tile_t* tile ){

  video__blit_verts( tile->prototype->texture_src.x1, 
		     tile->prototype->texture_src.y1, 
		     tile->prototype->texture_src.x2, 
		     tile->prototype->texture_src.y2,
		     idx_x * tile->prototype->background->tile_screen_width,
		     idx_y * tile->prototype->background->tile_screen_height,
		     (idx_x+1) * tile->prototype->background->tile_screen_width,
		     (idx_y+1) * tile->prototype->background->tile_screen_height );
		     
}

bool_t background__collision_test( const struct background_t* background,
				   const struct geo__rect_t* position,
				   bool_t climbing_stairs,
				   bool_t descending_stairs,
				   struct geo__vector_t* velocity,
				   bool_t* top_collision,
				   bool_t* bottom_collision,
				   bool_t* left_collision,
				   bool_t* right_collision,
				   struct geo__vector_t* surface_vector){

  
  if ( surface_vector->y == 0 ){

    struct geo__rect_t pos_for_test = { position->x, 
					position->y,
					position->width,
					position->height };

    surface_vector->x = 0;
    surface_vector->y = 0;

    _collision_test_for_walls( background,
			       &pos_for_test,
			       climbing_stairs,
			       &(velocity->x),
			       left_collision, 
			       right_collision,
			       surface_vector );

    pos_for_test.x += velocity->x;

    _collision_test_for_floors_and_ceilings( background,
					     &pos_for_test,
					     &(velocity->y),
					     top_collision, 
					     bottom_collision,
					     surface_vector );
  }
  else{
    
  }

  return (*top_collision || 
	  *bottom_collision || 
	  *left_collision || 
	  *right_collision);
}

bool_t _collision_test_for_walls( const struct background_t* background,
				  const struct geo__rect_t* position,
				  bool_t* climbing_stairs,
				  int* x_velocity,
				  bool_t* left_collision,
				  bool_t* right_collision,
				  struct geo__vector_t* surface_vector ){

  *left_collision = *right_collision = FALSE;

  struct geo__rect_t pos_for_test = { position->x,
				      position->y+1,
				      position->width,
				      position->height-2 };

  struct geo__point_t incline_hot_spot = { position->x + position->width/2,
					   position->y + position->height };

  bool_t moving_right = ( 0 < *x_velocity );
  bool_t moving_left = ( *x_velocity < 0 );
  bool_t standing_still = ( 0 == *x_velocity );

  if ( moving_right ) {
    pos_for_test.x += pos_for_test.width;
    pos_for_test.width = 0;
  }
  else if ( moving_left ){
    pos_for_test.width = 0;
  }

  /* get the range of background tiles to check */
  int32_t ul_idx_x = (pos_for_test.x + (moving_left)*(*x_velocity) - 1) / background->tile_width;
  int32_t ul_idx_y = (pos_for_test.y) / background->tile_height;
  
  int32_t lr_idx_x = (pos_for_test.x + pos_for_test.width + (moving_right)*(*x_velocity)) / background->tile_width;
  int32_t lr_idx_y = (pos_for_test.y + pos_for_test.height) / background->tile_height;

  struct geo__rect_t tile_position = 
    { 0, 0, 
      background->tile_width, 
      background->tile_height };

  int32_t idx_x, idx_y;
  for ( idx_x = ul_idx_x; idx_x <= lr_idx_x; ++idx_x ){
    for ( idx_y = ul_idx_y; idx_y <= lr_idx_y; ++idx_y ){
      tile_position.x = (idx_x * background->tile_width);
      tile_position.y = (idx_y * background->tile_height);

      if ( background->tiles[idx_x][idx_y].prototype ){

	int32_t distance_until_collision = 0;

	enum background__tile_collision_type_e collision_type = 
	  background->tiles[idx_x][idx_y].prototype->collision_type;

	if ( standing_still ){
	  *left_collision = 
	    (*left_collision ||
	     ((collision_type & BACKGROUND__COLLISION_RIGHT) &&
	      collision__touches_right(&pos_for_test, &tile_position)));

	  *right_collision = 
	    (*right_collision ||
	     ((collision_type & BACKGROUND__COLLISION_LEFT) &&
	      collision__touches_left(&pos_for_test, &tile_position)));
	}
	else if (((moving_right) && 
		  (collision_type & BACKGROUND__COLLISION_LEFT)) ||
		 ((moving_left) && 
		  (collision_type & BACKGROUND__COLLISION_RIGHT))){

	  struct geo__vector_t v = { *x_velocity, 0 };

	  if ( collision__moving_rectangle_intersects_rectangle( &pos_for_test,
								 &v,
								 &tile_position,
								 &distance_until_collision ) ){
	    *left_collision = (moving_left);
	    *right_collision = (moving_right);

	    *x_velocity = (*x_velocity / abs(*x_velocity)) * distance_until_collision;

	    standing_still = (0 == *x_velocity);
	  }
	}
      }
    }
  }

  return (*left_collision || *right_collision);
}

bool_t _collision_test_for_floors_and_ceilings(const struct background_t* background,
					       const struct geo__rect_t* position,
					       int* y_velocity,
					       bool_t* top_collision,
					       bool_t* bottom_collision,
					       struct geo__rect_t* surface_vector){

  struct geo__rect_t pos_for_test = { position->x+1,
				      position->y,
				      position->width-2,
				      position->height };

  struct geo__vector_t incline_hot_spot = { position->x + position->width/2,
					    position->y + position->height };

  bool_t moving_down = ( 0 < *y_velocity );
  bool_t moving_up = ( *y_velocity < 0 );
  bool_t standing_still = ( 0 == *y_velocity );

  if ( moving_down ){
    pos_for_test.y += pos_for_test.height;
    pos_for_test.height = 0;
  }
  else if ( moving_up ){
    pos_for_test.height = 0;
  }

  /* get the range of background tiles to check */
  int32_t ul_idx_x = (pos_for_test.x) / background->tile_width;
  int32_t ul_idx_y = (pos_for_test.y + moving_up*(*y_velocity) - 1) / background->tile_height;
  
  int32_t lr_idx_x = (pos_for_test.x + pos_for_test.width) / background->tile_width;
  int32_t lr_idx_y = (incline_hot_spot.y + moving_down*(*y_velocity)) / background->tile_height;

  struct geo__rect_t tile_position = 
    { 0, 0, 
      (background->tile_width), 
      (background->tile_height) };

  *top_collision = *bottom_collision = FALSE;

  int32_t idx_x, idx_y;
  for ( idx_x = ul_idx_x; idx_x <= lr_idx_x; ++idx_x ){
    for ( idx_y = ul_idx_y; idx_y <= lr_idx_y; ++idx_y ){
      tile_position.x = (idx_x * background->tile_width);
      tile_position.y = (idx_y * background->tile_height);

      if ( background->tiles[idx_x][idx_y].prototype ){

	int32_t distance_until_collision = 0;

	enum background__tile_collision_type_e collision_type = 
	  background->tiles[idx_x][idx_y].prototype->collision_type;

	if ( standing_still ){

	  *top_collision = 
	    (*top_collision ||
	     ((collision_type & BACKGROUND__COLLISION_BOTTOM) &&
	      collision__touches_bottom(&pos_for_test, &tile_position)));

	  *bottom_collision = 
	    (*bottom_collision ||
	     (surface_vector->y != 0) ||
	     ((collision_type & BACKGROUND__COLLISION_TOP) &&
	      collision__touches_top(&pos_for_test, &tile_position)));
	}
	else if (((moving_down) && 
		  (collision_type & BACKGROUND__COLLISION_TOP)) ||
		 ((moving_up) && 
		  (collision_type & BACKGROUND__COLLISION_BOTTOM))){

	  struct geo__vector_t v = { 0, *y_velocity };

	  if ( collision__moving_rectangle_intersects_rectangle( &pos_for_test,
								 &v,
								 &tile_position,
								 &distance_until_collision ) ){

	    *top_collision = (moving_up);
	    *bottom_collision = (moving_down);

	    *y_velocity = (*y_velocity / abs(*y_velocity)) * distance_until_collision;

	    standing_still = (0 == *y_velocity);
	  }
	}

      }
    }
  }
  
  return (*top_collision || *bottom_collision);
}
