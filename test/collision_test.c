#include <collision.h>
#include <geometry.h>
#include <constants.h>

#include <test_utils.h>

int rectangle_overlap_colocated(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 0, 10, 10 };

  return (TRUE == collision__rectangles_overlap( &a, &b ));
}

int rectangle_overlap_contains(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 3, 3, 4, 4 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_spans_horizontal(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 3, 20, 4 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_spans_vertical(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 3, -5, 4, 20 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_top(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, -5, 10, 10 };
  geo__rect_t c = { 3, -5, 4, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_bottom(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 5, 10, 10 };
  geo__rect_t c = { 3, 5, 4, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 0, 10, 10 };
  geo__rect_t c = { -5, 3, 10, 4 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, 0, 10, 10 };
  geo__rect_t c = { 5, 3, 10, 4 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_upper_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, -5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_upper_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, -5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_lower_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_lower_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, 5, 10, 10 };

  TEST_ASSERT(TRUE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(TRUE == collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_top(){
  
  geo__rect_t a = { 0,   0, 10, 10 };
  geo__rect_t b = { 0, -10, 10, 10 };
  geo__rect_t c = { 3, -10,  4, 10 };
  geo__rect_t d = { 0, -20, 10, 10 };
  geo__rect_t e = { 3, -20,  4, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_bottom(){
  
  geo__rect_t a = { 0,   0, 10, 10 };
  geo__rect_t b = { 0,  10, 10, 10 };
  geo__rect_t c = { 3,  10,  4, 10 };
  geo__rect_t d = { 0,  20, 10, 10 };
  geo__rect_t e = { 3,  20,  4, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_left(){
  
  geo__rect_t a = {   0, 0, 10, 10 };
  geo__rect_t b = { -10, 0, 10, 10 };
  geo__rect_t c = { -10, 3, 10,  4 };
  geo__rect_t d = { -20, 0, 10, 10 };
  geo__rect_t e = { -20, 3, 10,  4 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_right(){
  
  geo__rect_t a = {  0, 0, 10, 10 };
  geo__rect_t b = { 10, 0, 10, 10 };
  geo__rect_t c = { 10, 3, 10,  4 };
  geo__rect_t d = { 20, 0, 10, 10 };
  geo__rect_t e = { 20, 3, 10,  4 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_upper_left_corner(){
  
  geo__rect_t a = {   0,   0, 10, 10 };
  geo__rect_t b = { -10, -10, 10, 10 };
  geo__rect_t c = { -20, -10, 10, 10 };
  geo__rect_t d = { -10, -20, 10, 10 };
  geo__rect_t e = { -20, -20, 10, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_upper_right_corner(){
  
  geo__rect_t a = {  0,   0, 10, 10 };
  geo__rect_t b = { 10, -10, 10, 10 };
  geo__rect_t c = { 20, -10, 10, 10 };
  geo__rect_t d = { 10, -20, 10, 10 };
  geo__rect_t e = { 20, -20, 10, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_lower_left_corner(){
  
  geo__rect_t a = {   0,   0, 10, 10 };
  geo__rect_t b = { -10, 10, 10, 10 };
  geo__rect_t c = { -20, 10, 10, 10 };
  geo__rect_t d = { -10, 20, 10, 10 };
  geo__rect_t e = { -20, 20, 10, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_lower_right_corner(){
  
  geo__rect_t a = {  0,   0, 10, 10 };
  geo__rect_t b = { 10, 10, 10, 10 };
  geo__rect_t c = { 20, 10, 10, 10 };
  geo__rect_t d = { 10, 20, 10, 10 };
  geo__rect_t e = { 20, 20, 10, 10 };

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT(FALSE == collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT(FALSE == collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int horizontal_line_intersects_rectangle(){

  geo__rect_t r = { 0, 0, 10, 10 };
  geo__line_t l = { -5, 5, 10, 10 };

  geo__point_t p;

  TEST_ASSERT(TRUE == collision__line_intersects_rectangle( &l, &r, &p ));
  TEST_ASSERT(p.x == 0);
  TEST_ASSERT(p.y == 5);
  
  TEST_OK();
}

TEST_SUITE_START(Collision Tests)

TEST_CASE(rectangle_overlap_colocated)
TEST_CASE(rectangle_overlap_contains)
TEST_CASE(rectangle_overlap_spans_horizontal)
TEST_CASE(rectangle_overlap_spans_vertical)

TEST_CASE(rectangle_overlap_top)
TEST_CASE(rectangle_overlap_bottom)
TEST_CASE(rectangle_overlap_left)
TEST_CASE(rectangle_overlap_right)

TEST_CASE(rectangle_overlap_upper_left)
TEST_CASE(rectangle_overlap_upper_right)
TEST_CASE(rectangle_overlap_lower_left)
TEST_CASE(rectangle_overlap_lower_right)

TEST_CASE(rectangle_doesnt_overlap_top)
TEST_CASE(rectangle_doesnt_overlap_bottom)
TEST_CASE(rectangle_doesnt_overlap_left)
TEST_CASE(rectangle_doesnt_overlap_right)

TEST_CASE(rectangle_doesnt_overlap_upper_left_corner)
TEST_CASE(rectangle_doesnt_overlap_upper_right_corner)
TEST_CASE(rectangle_doesnt_overlap_lower_left_corner)
TEST_CASE(rectangle_doesnt_overlap_lower_right_corner)

TEST_CASE(horizontal_line_intersects_rectangle)

TEST_SUITE_END()
			     
