#ifndef UTILS_H
#define UTILS_H

#include <types.h>

typedef bool_t item_equality_fxn(const void* item_a, const void* item_b);

bool_t referential_euality_check(const void* item_a, const void* item_b);

struct linked_list__node_t;

struct linked_list_t{
  struct linked_list__node_t* head;
};

int linked_list__setup(struct linked_list_t* ll);
int linked_list__teardown(struct linked_list_t* ll, bool_t free_data);

int linked_list__add(void* item, struct linked_list_t* ll); 
void* linked_list__remove(void* item, item_equality_fxn item_equality, struct linked_list_t* ll); 

void* linked_list__begin(struct linked_list_t* ll);
void* linked_list__next();

#endif
