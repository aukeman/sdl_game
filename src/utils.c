#include <utils.h>
#include <constants.h>
#include <stdlib.h>


bool_t referential_equality_check(const void* item_a, const void* item_b){
  return item_a == item_b;
}

struct linked_list__node_t{
  void* data;
  struct linked_list__node_t* next;
};

struct linked_list__node_t* _iterator;

int linked_list__setup(struct linked_list_t* ll){
  ll->head = NULL;

  return SUCCESS;
}

int linked_list__teardown(struct  linked_list_t* ll, bool_t free_data){

  struct linked_list__node_t* current_node = ll->head;
  while ( current_node ){

      struct linked_list__node_t* node_to_free = current_node;
      current_node = current_node->next;

      if ( free_data ){
	free(node_to_free->data);
      }

      free(node_to_free);
    }

  ll->head = NULL;

  return SUCCESS;
}

int linked_list__add(void* item, struct linked_list_t* ll){

  struct linked_list__node_t** current_node_ptr = &(ll->head);

  while ( *current_node_ptr != NULL ){
    current_node_ptr = &((*current_node_ptr)->next);
  }

  *current_node_ptr = 
    (struct linked_list__node_t*)malloc(sizeof(struct linked_list__node_t));

  (*current_node_ptr)->data = item;
  (*current_node_ptr)->next = NULL;

  return SUCCESS;
}

void* linked_list__remove(void* item, item_equality_fxn item_equality, struct linked_list_t* ll){

  struct linked_list__node_t** current_node_ptr = &(ll->head);
  struct linked_list__node_t* next_node = NULL;
  
  void* found_item = NULL;

  while ( *current_node_ptr != NULL &&
	  !item_equality(item, (*current_node_ptr)->data) ){
    current_node_ptr = &((*current_node_ptr)->next);
  }
  
  if ( *current_node_ptr ){
    found_item = (*current_node_ptr)->data;
    next_node = (*current_node_ptr)->next;
    
    free(*current_node_ptr);
    *current_node_ptr = next_node;
  }

  return found_item;
}

int linked_list__begin_iteration(const struct linked_list_t* ll){
  _iterator = ll->head;

  return SUCCESS;
}

void* linked_list__next_iteration(){

  void* result = NULL;

  if ( _iterator ){
    result = _iterator->data;
    _iterator = _iterator->next;
  }

  return result;
}
