#include <video.h>
#include <events.h>
#include <constants.h>

#include <unistd.h>

void on_quit( enum events__type_e type, uint32_t parameter, void* context ) {
  *((int*)context) = FALSE;
}

int main( int argc, char** argv ) {

  video__setup();

  int keep_looping = TRUE;

  events__set_callback( EVENTS__TYPE_QUIT, on_quit, &keep_looping );

  struct video__texture_data_t texture;
  video__setup_texture("testing.png", &texture);

  while ( keep_looping ) {
    events__process_events();

    video__clearscreen();

    video__blit(&texture, 0, 0);

    video__flip();
  }

  video__teardown();

  return 0;
}
