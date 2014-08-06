#include <collision.h>
#include <geometry.h>
#include <constants.h>

#include <test_utils.h>

int test_rectangle_overlap_1(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 0, 10, 10 };

  return collision__rectangles_overlap( &a, &b );
}

TEST_SUITE_START(Collision Tests)

TEST_CASE(test_rectangle_overlap_1)

TEST_SUITE_END()
			     
