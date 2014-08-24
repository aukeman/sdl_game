#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>
#include <control.h>
#include <player.h>
#include <background.h>

#include <unistd.h>
#include <stdio.h>

void on_quit( events__type_e type, 
	      const events__event_parameter_t* parameter, 
	      void* context ) {

  if ( type == EVENTS__TYPE_QUIT ||
       (type == EVENTS__TYPE_KEYUP &&
	parameter->key.value == EVENTS__KEY_ESCAPE) ) {
    *((int*)context) = FALSE;
  }
}

void draw_player( const geo__point_t* position, void* context ){
  
  static geo__rect_t dest = { 0, 0, 32, 32 };

  dest.x = position->x;
  dest.y = position->y;

  const struct player_t* player = (const struct player_t*)context;

  video__rect( &dest, player->color[0], player->color[1], player->color[2], 255 );
}

void update_player( milliseconds_t length_of_frame,
		    const geo__rect_t* bounding_box,
		    const geo__point_t* position,
		    geo__vector_t* velocity,
		    void* context ){
  
  struct player_t* player = (struct player_t*)context;

  const struct control__state_t* control = control__get_state(player->player_idx);

  static const int pps = 100;

  velocity->x = (control->right.value*pps) - (control->left.value*pps);
  velocity->y = (control->down.value*pps) - (control->up.value*pps);
}

void draw_background( const geo__point_t* position, void* context ){
  
  const struct background_t* background = (const struct background_t*)context;

  geo__rect_t dest = { position->x, 
		       position->y, 
		       background->prototype->bounding_box.width, 
		       background->prototype->bounding_box.height };

  video__blit( background->prototype->texture, 
	       &background->prototype->bounding_box,
	       &dest );
}



int main( int argc, char** argv ) {

  video__setup(800, 600, 400, 300, FALSE);
  js__setup();
  timing__setup();
  control__setup("data/controls.dat");

  int keep_looping = TRUE;

  events__add_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );
  events__add_callback( EVENTS__TYPE_KEYUP, on_quit, &keep_looping );

  struct video__texture_handle_t* texture;
  video__setup_texture("resources/img/testing.png", &texture);
  
  geo__rect_t source = {0, 0, 32, 32};
  geo__rect_t dest = {0, 0, 32, 32};

  struct font__handle_t* font = NULL;
  font__create("resources/font/test_font.dat", &font);

  struct entity_t background_entity;
  entity__setup(&background_entity);
  background_entity.texture = texture;
  entity__add_draw_fxn(&background_entity, draw_background);

  const int number_of_columns = 400/32+1;
  const int number_of_rows = 300/32+1;
  struct background_t background[number_of_columns][number_of_rows];

  int col_idx, row_idx;
  for ( col_idx = 0; col_idx < number_of_columns; ++col_idx ){
    for ( row_idx = 0; row_idx < number_of_columns; ++row_idx ){
      background[col_idx][row_idx].position.x = col_idx*32;
      background[col_idx][row_idx].position.y = row_idx*32;
      background[col_idx][row_idx].prototype = &background_entity;
    }
  }

  struct entity_t player_entity;
  entity__setup(&player_entity);

  entity__add_update_fxn(&player_entity, update_player);
  entity__add_draw_fxn(&player_entity, draw_player);

  struct player_t players[2] = {
    { 0, { 175, 225 }, { 0, 0 }, { 255, 0,   0 } },
    { 1, { 225, 225 }, { 0, 0 }, {   0, 0, 255 } }
  };

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    events__process_events();

    video__clearscreen();

    for ( col_idx = 0; col_idx < number_of_columns; ++col_idx ){
      for ( row_idx = 0; row_idx < number_of_columns; ++row_idx ){
	entity__draw( &background_entity,
		      &background[col_idx][row_idx].position,
		      &background[col_idx][row_idx] );
      }
    }


    for ( dest.x = 0; dest.x < 400; dest.x += 32 ){
      for ( dest.y = 0; dest.y < 300; dest.y += 32 ){
    	video__blit(texture, &source, &dest);
      }
    }

    int player_idx = 0;
    for ( player_idx = 0; player_idx < 2; ++player_idx ){
      entity__update( &player_entity, 
		      timing__get_frame_length(),
		      &players[player_idx].position, 
		      &players[player_idx].velocity, 
		      &players[player_idx] );

      entity__draw( &player_entity, 
		    &players[player_idx].position, 
		    &players[player_idx] );
    }
		    

    geo__line_t line = { 30, 30, 350, 45 };

    video__line( &line, 0, 255, 0, 192 );

    
    font__draw_string(font, 0, 0, 
		      "FPS:         %5.1f\n"
		      "Frame Count: %5d\n"
		      "Fame Length: %5d",
		      timing__get_instantaneous_fps(),
		      timing__get_frame_count(),
		      timing__get_frame_length());

    video__flip();
  }

  fprintf( stdout, 
	   "%d frames; %f seconds %f fps average %f fps instantaneous\n",
	   timing__get_frame_count(),
	   timing__get_top_of_frame() / 1000.0f,
	   timing__get_average_fps(),
	   timing__get_instantaneous_fps());

  entity__teardown(&player_entity);

	   
  font__free(font);

  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
