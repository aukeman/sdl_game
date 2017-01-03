#include <video.h>
#include <events.h>
#include <constants.h>
#include <timing.h>
#include <font.h>
#include <control.h>
#include <player.h>
#include <level.h>
#include <background.h>
#include <stopwatch.h>
#include <joystick.h>
#include <utils.h>
#include <camera.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

void on_quit( events__type_e type, 
	      const events__event_parameter_t* parameter, 
	      void* context ) {

  if ( type == EVENTS__TYPE_QUIT ||
       (type == EVENTS__TYPE_KEYUP &&
	parameter->key.value == EVENTS__KEY_ESCAPE) ) {
    *((int*)context) = FALSE;
  }
}


int main( int argc, char** argv ) {

  int keep_looping = TRUE;
  struct level_t* level = NULL;
  struct font__handle_t* font = NULL;
  struct player_prototype_t default_player;
  struct player_t player;
  struct camera_t camera;
  struct stopwatch_t process_events_sw, draw_bg_sw, draw_players_sw, update_players_sw, draw_stats_sw, flip_page_sw, frame_sw;
  struct geo__rect_t viewport = {0, 0, 0, 0}, world_bounds = {0, 0, 0, 0};
  

  video__setup(1600, 1200, 400, 300, (1 < argc) && !strcmp("-f", argv[1]) );
  js__setup();
  timing__setup();
  control__setup("data/controls.dat");

  events__add_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );
  events__add_callback( EVENTS__TYPE_KEYUP, on_quit, &keep_looping );

  font__create("resources/font/test_font.dat", &font);

  if ( level__create("resources/level/level.dat", &level) != SUCCESS ){
    fprintf(stderr, "could not create level\n");
  }

  memset(&default_player, '0', sizeof(default_player));
  default_player.draw_fxn = &player__basic_draw;
  default_player.update_fxn = &player__basic_update;

  if ( player__load_config("data/player.dat", &default_player) != SUCCESS )
  {
    fprintf( stderr, "could not load player config\n" );
  }

  geo__init_point( &player.position, utils__screen2pos(225), utils__screen2pos(225) );
  geo__init_vector( &player.velocity, 0, 0 );
  player.prototype = &default_player;
  player.control = control__get_state(1);

  player.top_collision = FALSE;
  player.bottom_collision = FALSE;
  player.left_collision = FALSE;
  player.right_collision = FALSE;

  player.against_ledge = FALSE;

  player.state.value = PLAYER__STATE_NONE;
  player.state.timestamp = 0;
  player.color[0] = 0;
  player.color[1] = 0;
  player.color[2] = 255;

  stopwatch__init(&process_events_sw);
  stopwatch__init(&draw_bg_sw);
  stopwatch__init(&draw_players_sw);
  stopwatch__init(&update_players_sw);
  stopwatch__init(&draw_stats_sw);
  stopwatch__init(&flip_page_sw);
  stopwatch__init(&frame_sw);

  video__get_viewport( &viewport );
  level__get_bounds( level, &world_bounds );

  camera__setup( &camera, 
		 &player.position,
		 &viewport,
		 &world_bounds );

  video__clearscreen();

  /* timing__set_fixed_frame_rate(60); */

  while ( keep_looping ) {

    timing__declare_top_of_frame();

    stopwatch__start(&frame_sw);

    stopwatch__start(&process_events_sw);
    events__process_events();
    stopwatch__stop(&process_events_sw);

    camera__center_on( &camera,
		       &player.position );

    stopwatch__start(&draw_bg_sw);



    level__draw(level);
    stopwatch__stop(&draw_bg_sw);

    stopwatch__start(&draw_players_sw);
    camera__begin_render(&camera);
    player.prototype->draw_fxn(&player);
    camera__end_render(&camera);
    stopwatch__stop(&draw_players_sw);
    
    stopwatch__start(&update_players_sw);
    player.prototype->update_fxn( &player, 
				  level->terrain_layer.background,
				  timing__get_frame_length() );
    stopwatch__stop(&update_players_sw);
    
    level__update( level, 
		   player.position.x - video__get_screen_extents()->viewport_position_width/2,
		   player.position.y - video__get_screen_extents()->viewport_position_height/2 );

    stopwatch__start(&draw_stats_sw);
    font__draw_string(font, 0, 0,
    		      "FPS:         %5.1f\n"
    		      "Frame Count: %5d\n"
    		      "Fame Length: %5d\n"
		      "Screen Pos: %5d %5d\n"
		      "Pos: %4d %4d Vel: %3d %3d\n"
		      "top: %d bottom: %d left: %d right: %d\n"
		      "state: %d against ledge: %d",
    		      timing__get_instantaneous_fps(),
    		      timing__get_frame_count(),
    		      timing__get_frame_length(),
		      level->terrain_layer.background->scroll_position_x,
		      level->terrain_layer.background->scroll_position_y,
		      player.position.x, 
		      player.position.y, 
		      player.velocity.x, 
		      player.velocity.y,
		      player.top_collision,
		      player.bottom_collision,
		      player.left_collision,
		      player.right_collision,
		      player.state.value,
		      player.against_ledge);
    stopwatch__stop(&draw_stats_sw);

    stopwatch__start(&flip_page_sw);
    video__flip();
    stopwatch__stop(&flip_page_sw);

    stopwatch__stop(&frame_sw);
  }

  fprintf( stdout, 
	   "%d frames; %f seconds %f fps average %f fps instantaneous\n",
	   timing__get_frame_count(),
	   timing__get_top_of_frame() / (float)TIMING__SECONDS_TO_TICKS,
	   timing__get_average_fps(),
	   timing__get_instantaneous_fps());

  stopwatch__dump(&process_events_sw, "Process Events", stdout);
  stopwatch__dump(&draw_bg_sw, "Draw Background", stdout);
  stopwatch__dump(&draw_players_sw, "Draw Players", stdout);
  stopwatch__dump(&update_players_sw, "Update Players", stdout);
  stopwatch__dump(&draw_stats_sw, "Draw Stats", stdout);
  stopwatch__dump(&flip_page_sw, "Flip Page", stdout);
  stopwatch__dump(&frame_sw, "Frame", stdout);

  level__free(level);
	   
  font__free(font);
  
  timing__teardown();
  js__teardown();
  video__teardown();

  return 0;
}
