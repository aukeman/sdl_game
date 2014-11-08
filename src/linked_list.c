#include <linked_list.h>
#include <constants.h>
#include <stdlib.h>


bool_t referential_equality(const void* needle, const void* haystack){
  return needle == haystack;
}

struct linked_list__node_t{
  void* data;
  struct linked_list__node_t* next;
};

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

  if ( item_equality == NULL ){
    item_equality = referential_equality;
  }

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

bool_t linked_list__empty(const struct linked_list_t* ll){
  return !ll || (ll->head == NULL);
}

void* linked_list__begin(struct linked_list_t* ll, struct linked_list__node_t** iter){

  void* result = NULL;

  if ( iter ){
    *iter = ll->head;
    result = linked_list__next(iter);
  }

  return result;
}

void* linked_list__next(struct linked_list__node_t** iter){

  void* result = NULL;

  if ( iter && *iter ){

    result = (*iter)->data;
    *iter = (*iter)->next;
  }

  return result;
}
