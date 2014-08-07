#include <collision.h>
#include <geometry.h>
#include <constants.h>

#include <test_utils.h>

int rectangle_overlap_colocated(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 0, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));

  TEST_OK();
}

int rectangle_overlap_contains(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 3, 3, 4, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_spans_horizontal(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 3, 20, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_spans_vertical(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 3, -5, 4, 20 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_top(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, -5, 10, 10 };
  geo__rect_t c = { 3, -5, 4, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_bottom(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 0, 5, 10, 10 };
  geo__rect_t c = { 3, 5, 4, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 0, 10, 10 };
  geo__rect_t c = { -5, 3, 10, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, 0, 10, 10 };
  geo__rect_t c = { 5, 3, 10, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_OK();
}

int rectangle_overlap_upper_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, -5, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_upper_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, -5, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_lower_left(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { -5, 5, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_overlap_lower_right(){
  
  geo__rect_t a = { 0, 0, 10, 10 };
  geo__rect_t b = { 5, 5, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_top(){
  
  geo__rect_t a = { 0,   0, 10, 10 };
  geo__rect_t b = { 0, -10, 10, 10 };
  geo__rect_t c = { 3, -10,  4, 10 };
  geo__rect_t d = { 0, -20, 10, 10 };
  geo__rect_t e = { 3, -20,  4, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_bottom(){
  
  geo__rect_t a = { 0,   0, 10, 10 };
  geo__rect_t b = { 0,  10, 10, 10 };
  geo__rect_t c = { 3,  10,  4, 10 };
  geo__rect_t d = { 0,  20, 10, 10 };
  geo__rect_t e = { 3,  20,  4, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_left(){
  
  geo__rect_t a = {   0, 0, 10, 10 };
  geo__rect_t b = { -10, 0, 10, 10 };
  geo__rect_t c = { -10, 3, 10,  4 };
  geo__rect_t d = { -20, 0, 10, 10 };
  geo__rect_t e = { -20, 3, 10,  4 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_right(){
  
  geo__rect_t a = {  0, 0, 10, 10 };
  geo__rect_t b = { 10, 0, 10, 10 };
  geo__rect_t c = { 10, 3, 10,  4 };
  geo__rect_t d = { 20, 0, 10, 10 };
  geo__rect_t e = { 20, 3, 10,  4 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_upper_left_corner(){
  
  geo__rect_t a = {   0,   0, 10, 10 };
  geo__rect_t b = { -10, -10, 10, 10 };
  geo__rect_t c = { -20, -10, 10, 10 };
  geo__rect_t d = { -10, -20, 10, 10 };
  geo__rect_t e = { -20, -20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_upper_right_corner(){
  
  geo__rect_t a = {  0,   0, 10, 10 };
  geo__rect_t b = { 10, -10, 10, 10 };
  geo__rect_t c = { 20, -10, 10, 10 };
  geo__rect_t d = { 10, -20, 10, 10 };
  geo__rect_t e = { 20, -20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_lower_left_corner(){
  
  geo__rect_t a = {   0,   0, 10, 10 };
  geo__rect_t b = { -10, 10, 10, 10 };
  geo__rect_t c = { -20, 10, 10, 10 };
  geo__rect_t d = { -10, 20, 10, 10 };
  geo__rect_t e = { -20, 20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int rectangle_doesnt_overlap_lower_right_corner(){
  
  geo__rect_t a = {  0,   0, 10, 10 };
  geo__rect_t b = { 10, 10, 10, 10 };
  geo__rect_t c = { 20, 10, 10, 10 };
  geo__rect_t d = { 10, 20, 10, 10 };
  geo__rect_t e = { 20, 20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  TEST_OK();
}

int perpendicular_lines_intersect(){

  geo__line_t v = { 5, -5,  5, 5 };
  geo__line_t h = { 0,  0, 10, 0 };

  geo__point_t p = {0, 0};

  TEST_ASSERT_TRUE(collision__line_intersects_line(&v, &h, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&v, &h, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&h, &v, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&h, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  TEST_OK();
}

int perpendicular_lines_dont_intersect(){

  geo__line_t v =  {  5, -5,    5,  5 };
  geo__line_t h1 = { -5,  0,    5,  0 };
  geo__line_t h2 = {  0, -5,   10, -5 };
  geo__line_t h3 = {  0,  5,   10,  5 };

  geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h1, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h1, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h2, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h2, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h3, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h3, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h1, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h1, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h2, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h2, &v, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h3, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h3, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 5 );

  TEST_OK();
}

int diagonal_lines_intersect(){

  geo__line_t l1 = { 0,   0, 10, -10 };
  geo__line_t l2 = { 0, -10, 10,  0  };
  geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&l1, &l2, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&l1, &l2, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&l2, &l1, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&l2, &l1, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -5 );

  TEST_OK();
}

int diagonal_lines_intersect_2(){

  geo__line_t a  = { 0,  0, 20, 10 };
  geo__line_t b =  { 0, 10, 20,  0 };

  geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, 5 );
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, 5 );
  
  TEST_OK();
}

int diagonal_lines_intersect_3(){

  geo__line_t a  = { 0,   0, 20, 10 };
  geo__line_t b =  { 0, -10, 20, 30 };

  geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, 7 );
  TEST_ASSERT_INT( p.y, 3 );
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x,  7 );
  TEST_ASSERT_INT( p.y,  3 );
  
  TEST_OK();
}

int diagonal_lines_dont_intersect(){

  geo__line_t l  = { 0,  0, 10, 10 };
 
  geo__line_t la = {  0, 10,  5, 5 };
  geo__line_t lb = { -5,  5,  0, 0 };
  geo__line_t lc = { 10, 10, 20, 0 };

  geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &la, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &la, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &lb, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &lb, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &lc, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&l, &lc, &p));
  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, 10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&la, &l, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&la, &l, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 5 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&lb, &l, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&lb, &l, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&lc, &l, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&lc, &l, &p));
  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, 10 );

  TEST_OK();
}

int diagonal_lines_dont_intersect_2(){

  geo__line_t a = { 0,  0,   20, 10 };
  geo__line_t b = { 0, 10,   20, 40 };

  geo__point_t p = { 0, 0 };

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, -5 );
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, -5 );
  
  TEST_OK();
 
}


int horizontal_line_intersects_rectangle(){

  geo__rect_t r = { 0, 0, 10, 10 };
  geo__line_t l = { -5, 5, 10, 10 };

  geo__point_t p;

  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l, &r, &p ));
  TEST_ASSERT_TRUE(p.x == 0);
  TEST_ASSERT_TRUE(p.y == 5);
  
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

TEST_CASE(perpendicular_lines_intersect)
TEST_CASE(perpendicular_lines_dont_intersect)
TEST_CASE(diagonal_lines_intersect)
TEST_CASE(diagonal_lines_intersect_2)
TEST_CASE(diagonal_lines_intersect_3)
TEST_CASE(diagonal_lines_dont_intersect)
TEST_CASE(diagonal_lines_dont_intersect_2)

TEST_CASE(horizontal_line_intersects_rectangle)

TEST_SUITE_END()
			     