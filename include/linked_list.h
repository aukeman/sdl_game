#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <types.h>

typedef bool_t item_equality_fxn(const void* needle, const void* haystack);

struct linked_list__node_t;

struct linked_list_t{
  struct linked_list__node_t* head;
};

int linked_list__setup(struct linked_list_t* ll);
int linked_list__teardown(struct linked_list_t* ll, bool_t free_data);

int linked_list__add(void* item, struct linked_list_t* ll); 
void* linked_list__remove(void* item, item_equality_fxn item_equality, struct linked_list_t* ll); 

bool_t linked_list__empty(const struct linked_list_t* ll);

void* linked_list__begin(struct linked_list_t* ll, struct linked_list__node_t** iter);
void* linked_list__next(struct linked_list__node_t** iter);

#endif
