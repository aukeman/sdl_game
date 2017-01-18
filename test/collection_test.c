#include <collection_of.h>
#include <test_utils.h>

DECLARE_COLLECTION_OF( int, 10 );

void init_collection_test(){
  COLLECTION_OF(int, 10) test_collection;
  INIT_COLLECTION_OF(int, 10, &test_collection);

  TEST_ASSERT_PTR( test_collection.sentinel, test_collection.values );

  TEST_ASSERT_INT( test_collection.values[0], 0 );
  TEST_ASSERT_INT( test_collection.values[1], 0 );
  TEST_ASSERT_INT( test_collection.values[2], 0 );
  TEST_ASSERT_INT( test_collection.values[3], 0 );
  TEST_ASSERT_INT( test_collection.values[4], 0 );
  TEST_ASSERT_INT( test_collection.values[5], 0 );
  TEST_ASSERT_INT( test_collection.values[6], 0 );
  TEST_ASSERT_INT( test_collection.values[7], 0 );
  TEST_ASSERT_INT( test_collection.values[8], 0 );
  TEST_ASSERT_INT( test_collection.values[9], 0 );
}

void add_to_empty_collection_test(){
  bool_t result = FALSE;

  COLLECTION_OF(int, 10) test_collection;
  INIT_COLLECTION_OF(int, 10, &test_collection);

  result = ADD_TO_COLLECTION(&test_collection, 3);
  
  TEST_ASSERT_INT(result, TRUE);
  TEST_ASSERT_PTR(test_collection.sentinel, test_collection.values + 1);


  TEST_ASSERT_INT( test_collection.values[0], 3 );
  TEST_ASSERT_INT( test_collection.values[1], 0 );
  TEST_ASSERT_INT( test_collection.values[2], 0 );
  TEST_ASSERT_INT( test_collection.values[3], 0 );
  TEST_ASSERT_INT( test_collection.values[4], 0 );
  TEST_ASSERT_INT( test_collection.values[5], 0 );
  TEST_ASSERT_INT( test_collection.values[6], 0 );
  TEST_ASSERT_INT( test_collection.values[7], 0 );
  TEST_ASSERT_INT( test_collection.values[8], 0 );
  TEST_ASSERT_INT( test_collection.values[9], 0 );
}

void add_to_nonempty_collection_test(){
  bool_t result = FALSE;

  COLLECTION_OF(int, 10) test_collection;
  INIT_COLLECTION_OF(int, 10, &test_collection);
  ADD_TO_COLLECTION(&test_collection, 9);

  result = ADD_TO_COLLECTION(&test_collection, 3);
  
  TEST_ASSERT_INT(result, TRUE);
  TEST_ASSERT_PTR(test_collection.sentinel, test_collection.values + 2);


  TEST_ASSERT_INT( test_collection.values[0], 9 );
  TEST_ASSERT_INT( test_collection.values[1], 3 );
  TEST_ASSERT_INT( test_collection.values[2], 0 );
  TEST_ASSERT_INT( test_collection.values[3], 0 );
  TEST_ASSERT_INT( test_collection.values[4], 0 );
  TEST_ASSERT_INT( test_collection.values[5], 0 );
  TEST_ASSERT_INT( test_collection.values[6], 0 );
  TEST_ASSERT_INT( test_collection.values[7], 0 );
  TEST_ASSERT_INT( test_collection.values[8], 0 );
  TEST_ASSERT_INT( test_collection.values[9], 0 );
}

void add_to_full_collection_test(){
  bool_t result = FALSE;

  COLLECTION_OF(int, 10) test_collection;
  INIT_COLLECTION_OF(int, 10, &test_collection);
  ADD_TO_COLLECTION(&test_collection, 9);
  ADD_TO_COLLECTION(&test_collection, 8);
  ADD_TO_COLLECTION(&test_collection, 7);
  ADD_TO_COLLECTION(&test_collection, 6);
  ADD_TO_COLLECTION(&test_collection, 5);
  ADD_TO_COLLECTION(&test_collection, 4);
  ADD_TO_COLLECTION(&test_collection, 3);
  ADD_TO_COLLECTION(&test_collection, 2);
  ADD_TO_COLLECTION(&test_collection, 1);
  ADD_TO_COLLECTION(&test_collection, 0);

  result = ADD_TO_COLLECTION(&test_collection, 3);
  
  TEST_ASSERT_INT(result, FALSE);
  TEST_ASSERT_PTR(test_collection.sentinel, test_collection.values + 10);


  TEST_ASSERT_INT( test_collection.values[0], 9 );
  TEST_ASSERT_INT( test_collection.values[1], 8 );
  TEST_ASSERT_INT( test_collection.values[2], 7 );
  TEST_ASSERT_INT( test_collection.values[3], 6 );
  TEST_ASSERT_INT( test_collection.values[4], 5 );
  TEST_ASSERT_INT( test_collection.values[5], 4 );
  TEST_ASSERT_INT( test_collection.values[6], 3 );
  TEST_ASSERT_INT( test_collection.values[7], 2 );
  TEST_ASSERT_INT( test_collection.values[8], 1 );
  TEST_ASSERT_INT( test_collection.values[9], 0 );
}

TEST_SUITE_START(Collection Test)
  TEST_CASE(init_collection_test)
  TEST_CASE(add_to_empty_collection_test)
  TEST_CASE(add_to_nonempty_collection_test)
  TEST_CASE(add_to_full_collection_test)
TEST_SUITE_END()
