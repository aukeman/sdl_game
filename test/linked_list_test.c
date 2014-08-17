#include <linked_list.h>
#include <test_utils.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

bool_t integer_equality( const void* a, const void* b ){
  return *(int*)a == *(int*)b;
}

struct linked_list_t ll;

int setup(){
  return linked_list__setup(&ll);
}

int teardown(){
  return linked_list__teardown(&ll, FALSE);
}

void empty_list(){

  int one = 1;

  TEST_ASSERT_NULL( linked_list__begin(&ll) );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_NULL( linked_list__remove(&one, integer_equality, &ll) );

}

void single_element(){

  int one = 1;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_NULL( linked_list__next() );  
}

void two_elements(){

  int one = 1;
  int two = 2;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &two );
  TEST_ASSERT_NULL( linked_list__next() );
}

void three_elements(){

  int one = 1;
  int two = 2;
  int three = 3;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &two );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );

}

void remove_from_head(){

  int one = 1;
  int two = 2;
  int three = 3;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__remove(&one, NULL, &ll), &one );
  
  TEST_ASSERT_PTR( linked_list__begin(&ll), &two );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &three );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_PTR( linked_list__remove(&three, NULL, &ll), &three );

  TEST_ASSERT_NULL( linked_list__begin(&ll) );
}

void remove_from_tail(){

  int one = 1;
  int two = 2;
  int three = 3;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__remove(&three, NULL, &ll), &three );
  
  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &two );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_PTR( linked_list__remove(&one, NULL, &ll), &one );

  TEST_ASSERT_NULL( linked_list__begin(&ll) );
}

void remove_from_middle(){

  int one = 1;
  int two = 2;
  int three = 3;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &two );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );

  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );
  
  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );
}

void remove_from_head_and_add(){

  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__remove(&one, NULL, &ll), &one );
  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );
  
  TEST_ASSERT_SUCCESS( linked_list__add(&four, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &three );
  TEST_ASSERT_PTR( linked_list__next(), &four );
  TEST_ASSERT_NULL( linked_list__next() );
}

void remove_from_tail_and_add(){

  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__remove(&three, NULL, &ll), &three );
  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );
  
  TEST_ASSERT_SUCCESS( linked_list__add(&four, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &four );
  TEST_ASSERT_NULL( linked_list__next() );
}

void remove_from_middle_and_add(){

  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_PTR( linked_list__remove(&two, NULL, &ll), &two );
  
  TEST_ASSERT_SUCCESS( linked_list__add(&four, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_PTR( linked_list__next(), &four );
  TEST_ASSERT_NULL( linked_list__next() );
}

void failed_remove(){

  int one = 1;
  int two = 2;
  int three = 3;
  int four = 4;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_NULL( linked_list__remove(&four, NULL, &ll) );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &two );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );
}

void free_on_teardown(){
  
  void* a = malloc(4);
  void* b = malloc(4);
  void* c = malloc(4);
  void* d = malloc(4);

  TEST_ASSERT_SUCCESS( linked_list__add(a, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(b, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(c, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(d, &ll) );
  
  TEST_ASSERT_SUCCESS( linked_list__teardown(&ll, TRUE) );
}

void custom_equality_check(){

  int one = 1;
  int two = 2;
  int three = 3;

  int other_two = 2;

  TEST_ASSERT_SUCCESS( linked_list__add(&one, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&two, &ll) );
  TEST_ASSERT_SUCCESS( linked_list__add(&three, &ll) );

  TEST_ASSERT_NULL( linked_list__remove(&other_two, NULL, &ll) );
  TEST_ASSERT_PTR( linked_list__remove(&other_two, integer_equality, &ll), &two );

  TEST_ASSERT_PTR( linked_list__begin(&ll), &one );
  TEST_ASSERT_PTR( linked_list__next(), &three );
  TEST_ASSERT_NULL( linked_list__next() );
}


TEST_SUITE_WITH_SETUP_START(Linked List Tests, setup, teardown)
  TEST_CASE(empty_list)
  TEST_CASE(single_element)
  TEST_CASE(two_elements)
  TEST_CASE(three_elements)
  TEST_CASE(remove_from_head)
  TEST_CASE(remove_from_tail)
  TEST_CASE(remove_from_middle)
  TEST_CASE(remove_from_head_and_add)
  TEST_CASE(remove_from_tail_and_add)
  TEST_CASE(remove_from_middle_and_add)
  TEST_CASE(failed_remove)
  TEST_CASE(free_on_teardown)
  TEST_CASE(custom_equality_check)
TEST_SUITE_END()

