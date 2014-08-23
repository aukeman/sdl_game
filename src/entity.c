#include <entity.h>
#include <constants.h>

#include <math.h>

int entity__setup( struct entity_t* entity ){
  memset(entity, '\0', sizeof(*entity));

  linked_list__setup( &entity->update_fxns );
  linked_list__setup( &entity->draw_fxns );

  return SUCCESS;
}

int entity__teardown( struct entity_t* entity ){
  
  linked_list__teardown( &entity->update_fxns, FALSE );
  linked_list__teardown( &entity->draw_fxns, FALSE );

  memset(entity, '\0', sizeof(*entity));

  return SUCCESS;
}

int entity__add_draw_fxn( struct entity_t* self, entity__draw_fxn* draw_fxn ){
  return linked_list__add( draw_fxn, &self->draw_fxns );
}
int entity__add_update_fxn( struct entity_t* self, entity__update_fxn* update_fxn ){
  return linked_list__add( update_fxn, &self->update_fxns );
}

int entity__remove_draw_fxn( struct entity_t* self, entity__draw_fxn* draw_fxn ){
  return (linked_list__remove( draw_fxn, NULL, &self->draw_fxns ) != NULL ?
	  SUCCESS : 
	  UNKNOWN_FAILURE);
}
int entity__remove_update_fxn( struct entity_t* self, entity__update_fxn* update_fxn ){
  return (linked_list__remove( update_fxn, NULL, &self->update_fxns ) != NULL ?
	  SUCCESS :
	  UNKNOWN_FAILURE);
}

void entity__draw( struct entity_t* self, const geo__point_t* position, void* context ){

  entity__draw_fxn* draw_fxn = 
    (entity__draw_fxn*)
    linked_list__begin( &self->draw_fxns );

  while ( draw_fxn ){
    draw_fxn( position, context );

    draw_fxn = 
      (entity__draw_fxn*)
      linked_list__next( &self->draw_fxns );
  }
}

void entity__update( struct entity_t* self, 
		     milliseconds_t length_of_frame,
		     geo__point_t* position,
		     geo__vector_t* velocity,
		     void* context ){

  entity__update_fxn* update_fxn = 
    (entity__update_fxn*)
    linked_list__begin( &self->update_fxns );

  while ( update_fxn ){
    update_fxn( length_of_frame, &self->bounding_box, position, velocity, context );

    update_fxn = 
      (entity__update_fxn*)
      linked_list__next( &self->update_fxns );
  }

  position->x += (int)rintf((length_of_frame/1000.0f)*velocity->x);
  position->y += (int)rintf((length_of_frame/1000.0f)*velocity->y);
}

