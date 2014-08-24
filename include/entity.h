#ifndef ENTITY_H
#define ENTTIY_H

#include <geometry.h>
#include <linked_list.h>
#include <types.h>
#include <string.h>

typedef void entity__draw_fxn( const geo__point_t* position, void* context );
typedef void entity__update_fxn( milliseconds_t length_of_frame,
				 const geo__rect_t* bounding_box,
				 const geo__point_t* position,
				 geo__vector_t* velocity,
				 void* context );

struct video__texture_handle_t;

struct entity_t{

  geo__rect_t bounding_box;

  const struct video__texture_handle_t* texture;

  struct linked_list_t update_fxns;
  struct linked_list_t draw_fxns;
};

int entity__setup( struct entity_t* entity );
int entity__teardown( struct entity_t* entity );

int entity__add_draw_fxn( struct entity_t* self, entity__draw_fxn* draw_fxn );
int entity__add_update_fxn( struct entity_t* self, entity__update_fxn* update_fxn );

int entity__remove_draw_fxn( struct entity_t* self, entity__draw_fxn* draw_fxn );
int entity__remove_update_fxn( struct entity_t* self, entity__update_fxn* update_fxn );

void entity__draw( struct entity_t* self, const geo__point_t* position, void* context );
void entity__update( struct entity_t* self, 
		     milliseconds_t length_of_frame,
		     geo__point_t* position,
		     geo__vector_t* velocity,
		     void* context );


#endif
