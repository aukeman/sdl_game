#include <collision.h>
#include <geometry.h>
#include <constants.h>

#include <test_utils.h>

int test_rectangle_overlap_colocated(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 0, 10, 10 };

  return (TRUE == collision__rectangles_overlap( &a, &b ));
}

int test_rectangle_overlap_contains(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 3, 3, 7, 7 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int test_rectangle_overlap_top(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, -5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int test_rectangle_overlap_bottom(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int test_rectangle_overlap_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 0, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int test_rectangle_overlap_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, 0, 10, 10 };

  TEST_ASSERT(TRUE != collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

TEST_SUITE_START(Collision Tests)

TEST_CASE(test_rectangle_overlap_colocated)
TEST_CASE(test_rectangle_overlap_contains)

TEST_CASE(test_rectangle_overlap_top)
TEST_CASE(test_rectangle_overlap_bottom)
TEST_CASE(test_rectangle_overlap_left)
TEST_CASE(test_rectangle_overlap_right)

TEST_SUITE_END()
			     
