#include <collision.h>
#include <geometry.h>
#include <constants.h>

#include <test_utils.h>

#include <limits.h>

void rectangle_overlap_colocated(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 0, 0, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));

  
}

void rectangle_overlap_contains(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 3, 3, 4, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  
}

void rectangle_overlap_spans_horizontal(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -5, 3, 20, 4 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  
}

void rectangle_overlap_spans_vertical(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 3, -5, 4, 20 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  
}

void rectangle_overlap_top(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 0, -5, 10, 10 };
  struct geo__rect_t c = { 3, -5, 4, 10 };

  struct geo__rect_t d = { 0, -10, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &d, &a ));
}

void rectangle_overlap_bottom(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 0, 5, 10, 10 };
  struct geo__rect_t c = { 3, 5, 4, 10 };

  struct geo__rect_t d = { 0, 10, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &d, &a ));

  
}

void rectangle_overlap_left(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -5, 0, 10, 10 };
  struct geo__rect_t c = { -5, 3, 10, 4 };

  struct geo__rect_t d = { -10, 0, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &d, &a ));

  
}

void rectangle_overlap_right(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 5, 0, 10, 10 };
  struct geo__rect_t c = { 5, 3, 10, 4 };

  struct geo__rect_t d = { 10, 0, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &d, &a ));

  
}

void rectangle_overlap_upper_left(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -5, -5, 10, 10 };

  struct geo__rect_t c = { -5, -10, 5, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  
}

void rectangle_overlap_upper_right(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 5, -5, 10, 10 };

  struct geo__rect_t c = { 10, -10, 3, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  
}

void rectangle_overlap_lower_left(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -5, 5, 10, 10 };

  struct geo__rect_t c = { -10, 10, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  
}

void rectangle_overlap_lower_right(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 5, 5, 10, 10 };

  struct geo__rect_t c = { 10, 10, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  
}

void rectangle_doesnt_overlap_touching(){
  
  struct geo__rect_t a = { 0, 0, 10, 10 };

  struct geo__rect_t b = { -10, -10, 10, 10 };
  struct geo__rect_t c = { -10,   0, 10, 10 };
  struct geo__rect_t d = { -10,  10, 10, 10 };
  struct geo__rect_t e = {   0,  10, 10, 10 };
  struct geo__rect_t f = {  10,  10, 10, 10 };
  struct geo__rect_t g = {  10,   0, 10, 10 };
  struct geo__rect_t h = {  10, -10, 10, 10 };
  struct geo__rect_t i = {   0, -10, 10, 10 };

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &e, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &f ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &f, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &g ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &g, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &h ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &h, &a ));

  TEST_ASSERT_TRUE(collision__rectangles_overlap( &a, &i ));
  TEST_ASSERT_TRUE(collision__rectangles_overlap( &i, &a ));

  
}

void rectangle_doesnt_overlap_top(){
  
  struct geo__rect_t a = { 0,   0, 10, 10 };

  struct geo__rect_t b = { 0, -10, 10, 9 };
  struct geo__rect_t c = { 3, -10,  4, 5 };
  struct geo__rect_t d = { 0, -20, 10, 3 };
  struct geo__rect_t e = { 3, -20,  4, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_bottom(){
  
  struct geo__rect_t a = { 0,   0, 10, 10 };
  struct geo__rect_t b = { 0,  11, 10, 10 };
  struct geo__rect_t c = { 3,  11,  4, 10 };
  struct geo__rect_t d = { 0,  20, 10, 10 };
  struct geo__rect_t e = { 3,  20,  4, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_left(){
  
  struct geo__rect_t a = {   0, 0, 10, 10 };
  struct geo__rect_t b = { -10, 0, 9, 10 };
  struct geo__rect_t c = { -10, 3, 9,  4 };
  struct geo__rect_t d = { -20, 0, 10, 10 };
  struct geo__rect_t e = { -20, 3, 10,  4 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_right(){
  
  struct geo__rect_t a = {  0, 0, 10, 10 };
  struct geo__rect_t b = { 11, 0, 10, 10 };
  struct geo__rect_t c = { 11, 3, 10,  4 };
  struct geo__rect_t d = { 20, 0, 10, 10 };
  struct geo__rect_t e = { 20, 3, 10,  4 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_upper_left_corner(){
  
  struct geo__rect_t a = {   0,   0, 10, 10 };
  struct geo__rect_t b = { -10, -10, 9, 9 };
  struct geo__rect_t c = { -20, -10, 10, 10 };
  struct geo__rect_t d = { -10, -20, 9, 10 };
  struct geo__rect_t e = { -20, -20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_upper_right_corner(){
  
  struct geo__rect_t a = {  0,   0, 10, 10 };
  struct geo__rect_t b = { 11, -11, 10, 10 };
  struct geo__rect_t c = { 20, -10, 10, 10 };
  struct geo__rect_t d = { 10, -20, 10, 10 };
  struct geo__rect_t e = { 20, -20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_lower_left_corner(){
  
  struct geo__rect_t a = {   0,   0, 10, 10 };
  struct geo__rect_t b = { -10, 10, 9, 9 };
  struct geo__rect_t c = { -20, 10, 10, 10 };
  struct geo__rect_t d = { -10, 20, 10, 10 };
  struct geo__rect_t e = { -20, 20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void rectangle_doesnt_overlap_lower_right_corner(){
  
  struct geo__rect_t a = {  0,  0, 10, 10 };
  struct geo__rect_t b = { 11, 11, 10, 10 };
  struct geo__rect_t c = { 20, 10, 10, 10 };
  struct geo__rect_t d = { 10, 20, 10, 10 };
  struct geo__rect_t e = { 20, 20, 10, 10 };

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &b ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &b, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &c ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &c, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &d ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &d, &a ));

  TEST_ASSERT_FALSE(collision__rectangles_overlap( &a, &e ));
  TEST_ASSERT_FALSE(collision__rectangles_overlap( &e, &a ));

  
}

void perpendicular_lines_intersect(){

  struct geo__line_t v = { 5, -5,  5, 5 };
  struct geo__line_t h = { 0,  0, 10, 0 };

  struct geo__point_t p = {0, 0};

  TEST_ASSERT_TRUE(collision__line_intersects_line(&v, &h, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&v, &h, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&h, &v, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&h, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  
}

void perpendicular_lines_dont_intersect(){

  struct geo__line_t v =  {  5, -5,    5,  5 };
  struct geo__line_t h1 = { -5,  0,    4,  0 };
  struct geo__line_t h2 = {  0, -6,   10, -6 };
  struct geo__line_t h3 = {  0,  6,   10,  6 };

  struct geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h1, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h1, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h2, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h2, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -6 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h3, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&v, &h3, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 6 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h1, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h1, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h2, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h2, &v, &p));
  TEST_ASSERT_INT( p.x,  5 );
  TEST_ASSERT_INT( p.y, -6 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h3, &v, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&h3, &v, &p));
  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, 6 );

  
}

void diagonal_lines_intersect(){

  struct geo__line_t l1 = { 0,   0, 10, -10 };
  struct geo__line_t l2 = { 0, -10, 10,  0  };
  struct geo__point_t p = {0, 0};

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

  
}

void diagonal_lines_intersect_2(){

  struct geo__line_t a  = { 0,  0, 20, 10 };
  struct geo__line_t b =  { 0, 10, 20,  0 };

  struct geo__point_t p = {0, 0};

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
  
  
}

void diagonal_lines_intersect_3(){

  struct geo__line_t a  = { 0,   0, 20, 10 };
  struct geo__line_t b =  { 0, -10, 20, 30 };

  struct geo__point_t p = {0, 0};

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
  
  
}

void diagonal_lines_dont_intersect(){

  struct geo__line_t l  = { 0,  0, 10, 10 };
 
  struct geo__line_t la = {  0, 10,  4, 6 };
  struct geo__line_t lb = { -5,  5,  -1, 1 };
  struct geo__line_t lc = { 11, 9, 20, 0 };

  struct geo__point_t p = {0, 0};

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

  
}

void diagonal_lines_dont_intersect_2(){

  struct geo__line_t a = { 0,  0,   20, 10 };
  struct geo__line_t b = { 0, 10,   20, 40 };

  struct geo__point_t p = { 0, 0 };

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
}

void vertical_parallel_lines_dont_intersect(){

  struct geo__line_t a = { -10, 100,  -10, -100 };
  struct geo__line_t b = {  30,  10,  30,    12 };
  struct geo__line_t c = {  -30,  -1000,  -30,    -1200 };

  struct geo__point_t p;

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );
}

void horizontal_parallel_lines_dont_intersect(){

  struct geo__line_t a = { -100,    10,  100,  10 };
  struct geo__line_t b = {  10,     30,  12,    30 };
  struct geo__line_t c = { -1000,  -30, -1200, -30 };

  struct geo__point_t p;

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );
}

void diagonal_parallel_lines_dont_intersect(){

  struct geo__line_t a = { -100,    10,  -120,  15 };
  struct geo__line_t b = { -100,    30,  -120,  35 };
  struct geo__line_t c = { -1000,  -30, -1020, -25 };

  struct geo__point_t p;

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &b, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&a, &c, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );

  p.x = 99; p.y = 99;
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, NULL));
  TEST_ASSERT_FALSE(collision__line_intersects_line(&c, &a, &p));
  TEST_ASSERT_INT( p.x, 0 );
  TEST_ASSERT_INT( p.y, 0 );
}

void vertical_overlapping_lines_intersect(){

  struct geo__line_t a = { -10, -100,  -10, 100 };

  struct geo__line_t b = { -10,   10,  -10,  20 };

  struct geo__line_t c = { -10, -100,  -10, -90 };
  struct geo__line_t d = { -10,  -90,  -10, -100 };

  struct geo__line_t e = { -10,  110,  -10,  90 };
  struct geo__line_t f = { -10,   90,  -10, 110 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&a, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, -100 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, 10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&c, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&c, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, -100 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&d, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&d, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, -90 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&e, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&e, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, 100 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&f, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&f, &a, &p));
  TEST_ASSERT_INT( p.x, -10 );
  TEST_ASSERT_INT( p.y, 90 );

}
void horizontal_overlapping_lines_intersect(){

  struct geo__line_t a = { -100, -10,  100, -10 };

  struct geo__line_t b = { 10,  -10,  20,  -10 };

  struct geo__line_t c = { -100, -10,  -90, -10 };
  struct geo__line_t d = { -90,  -10,  -100, -10 };

  struct geo__line_t e = { 110, -10,  90,  -10 };
  struct geo__line_t f = { 90,  -10,  110, -10 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&b, &a, &p));
  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, -10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&c, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&c, &a, &p));
  TEST_ASSERT_INT( p.x, -100 );
  TEST_ASSERT_INT( p.y, -10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&d, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&d, &a, &p));
  TEST_ASSERT_INT( p.x, -90 );
  TEST_ASSERT_INT( p.y, -10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&e, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&e, &a, &p));
  TEST_ASSERT_INT( p.x, 100 );
  TEST_ASSERT_INT( p.y, -10 );

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line(&f, &a, NULL));
  TEST_ASSERT_TRUE(collision__line_intersects_line(&f, &a, &p));
  TEST_ASSERT_INT( p.x, 90 );
  TEST_ASSERT_INT( p.y, -10 );
}
void diagonal_overlapping_lines_intersect(){

  struct geo__line_t a = {10, 10,   100, 100 };

  struct geo__line_t b = {  0,   0,  100, 100 };
  struct geo__line_t c = {110, 110,    0,   0 };
  struct geo__line_t d = { 20,  20,   30,  30 };
  struct geo__line_t e = { 30, 30,    20,  20 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line( &a, &a, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_line( &a, &a, &p ));
  TEST_ASSERT_INT(p.x, 10);
  TEST_ASSERT_INT(p.y, 10);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line( &b, &a, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_line( &b, &a, &p ));
  TEST_ASSERT_INT(p.x, 10);
  TEST_ASSERT_INT(p.y, 10);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line( &c, &a, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_line( &c, &a, &p ));
  TEST_ASSERT_INT(p.x, 100);
  TEST_ASSERT_INT(p.y, 100);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line( &d, &a, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_line( &d, &a, &p ));
  TEST_ASSERT_INT(p.x, 20);
  TEST_ASSERT_INT(p.y, 20);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_line( &e, &a, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_line( &e, &a, &p ));
  TEST_ASSERT_INT(p.x, 30);
  TEST_ASSERT_INT(p.y, 30);
  

}

void horizontal_line_intersects_rectangle(){

  struct geo__rect_t r = { 0, 0, 10, 10 };

  struct geo__line_t l1 = { -5, 5,   15, 5 };
  struct geo__line_t l2 = { 15, 5,   -5, 5 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, 0);
  TEST_ASSERT_INT(p.y, 5);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, 10);
  TEST_ASSERT_INT(p.y, 5);
  
  
}

void vertical_line_intersects_rectangle(){

  struct geo__rect_t r = { 0, 0, 10, 10 };

  struct geo__line_t l1 = { 5, -5,   5, 15 };
  struct geo__line_t l2 = { 5, 15,   5, -5 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, 5);
  TEST_ASSERT_INT(p.y, 0);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, 5);
  TEST_ASSERT_INT(p.y, 10);
  
  
}

void diagonal_line_intersects_rectangle(){

  struct geo__rect_t r;
  struct geo__line_t l1, l2, l3;
  struct geo__point_t p;

  geo__init_rect(&r, 0, 0, 10, 10);
  geo__init_line(&l1, -5, -5, 15, 15);
  geo__init_line(&l2, 15, 15, -5, -5);
  geo__init_line(&l3, 10, 10, -5, -5);


  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, 0);
  TEST_ASSERT_INT(p.y, 0);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, 10);
  TEST_ASSERT_INT(p.y, 10);

}

void orthogonal_line_intersects_rectangle_corner(){

  struct geo__rect_t r = { 3, 4, 5, 6 };

  /* lower left */
  struct geo__line_t l1 = { 0, 4,  4, 4 }; struct geo__point_t i1 = { 3, 4 };
  struct geo__line_t l2 = { 3, 0,  3, 4 }; struct geo__point_t i2 = { 3, 4 };

  /* lower right */
  struct geo__line_t l3 = { 10, 4,  8, 4 }; struct geo__point_t i3 = { 8, 4 };
  struct geo__line_t l4 = {  8, 0,  8, 8 }; struct geo__point_t i4 = { 8, 4 };

  /* upper left */
  struct geo__line_t l5 = { 0, 10,  4, 10 }; struct geo__point_t i5 = { 3, 10 };
  struct geo__line_t l6 = { 3, 15,  3, 4 }; struct geo__point_t i6 = { 3, 10 };

  /* upper right */
  struct geo__line_t l7 = { 15, 10,  5, 10 }; struct geo__point_t i7 = { 8, 10 };
  struct geo__line_t l8 = { 8, 15,  8, 4 }; struct geo__point_t i8 = { 8, 10 };

  struct geo__point_t p = {0, 0};

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, i1.x);
  TEST_ASSERT_INT(p.y, i1.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, i2.x);
  TEST_ASSERT_INT(p.y, i2.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, &p ));
  TEST_ASSERT_INT(p.x, i3.x);
  TEST_ASSERT_INT(p.y, i3.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, &p ));
  TEST_ASSERT_INT(p.x, i4.x);
  TEST_ASSERT_INT(p.y, i4.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, &p ));
  TEST_ASSERT_INT(p.x, i5.x);
  TEST_ASSERT_INT(p.y, i5.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l6, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l6, &r, &p ));
  TEST_ASSERT_INT(p.x, i6.x);
  TEST_ASSERT_INT(p.y, i6.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l7, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l7, &r, &p ));
  TEST_ASSERT_INT(p.x, i7.x);
  TEST_ASSERT_INT(p.y, i7.y);
  
  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l8, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l8, &r, &p ));
  TEST_ASSERT_INT(p.x, i8.x);
  TEST_ASSERT_INT(p.y, i8.y);
}

void line_from_inside_intersects_rectangle_at_origin(){
  
  struct geo__rect_t r = { -50, -11, 10, 10 };

  struct geo__line_t l1 = { -45, -5,  10,  -5 };  struct geo__point_t i1 = { -45, -5 };
  struct geo__line_t l2 = { -45, -5,  -60, -5 };  struct geo__point_t i2 = { -45, -5 };
  struct geo__line_t l3 = { -45, -5,  -45, -15 }; struct geo__point_t i3 = { -45, -5 };
  struct geo__line_t l4 = { -45, -5,  -45, 15 };  struct geo__point_t i4 = { -45, -5 };

  struct geo__line_t l5 = { -45, -5,  -45, -6 };  struct geo__point_t i5 = { -45, -5 };


  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, i1.x);
  TEST_ASSERT_INT(p.y, i1.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, i2.x);
  TEST_ASSERT_INT(p.y, i2.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, &p ));
  TEST_ASSERT_INT(p.x, i3.x);
  TEST_ASSERT_INT(p.y, i3.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, &p ));
  TEST_ASSERT_INT(p.x, i4.x);
  TEST_ASSERT_INT(p.y, i4.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, &p ));
  TEST_ASSERT_INT(p.x, i5.x);
  TEST_ASSERT_INT(p.y, i5.y);
}

void line_on_side_intersects_rectangle_at_origin(){

  struct geo__rect_t r = { -100, 10, 30, 5 };

  struct geo__line_t l1 = { -100, 12,  -110, 12 }; struct geo__point_t i1 = { -100, 12 };
  struct geo__line_t l2 = { -100, 12,  -100, 10 }; struct geo__point_t i2 = { -100, 12 };
  struct geo__line_t l3 = { -100, 12,  -100, 8 };  struct geo__point_t i3 = { -100, 12 };

  struct geo__line_t l4 = { -90, 15,  -100, 12 }; struct geo__point_t i4  = { -90, 15 };
  struct geo__line_t l5 = { -90, 15,  -80,  15 }; struct geo__point_t i5  = { -90, 15 };
  struct geo__line_t l6 = { -90, 15,  -100, 15 };  struct geo__point_t i6 = { -90, 15 };

  struct geo__line_t l7 = { -70, 12,  -60, 12 }; struct geo__point_t i7  = { -70, 12 };
  struct geo__line_t l8 = { -70, 12,  -70,  15 }; struct geo__point_t i8 = { -70, 12 };
  struct geo__line_t l9 = { -70, 12,  -70, 8 };  struct geo__point_t i9  = { -70, 12 };

  struct geo__line_t l10 = { -90, 10,  -90, 5 }; struct geo__point_t i10 = { -90, 10 };
  struct geo__line_t l11 = { -90, 10,  -80, 10 }; struct geo__point_t i11 = { -90, 10 };
  struct geo__line_t l12 = { -90, 10,  -100, 10 }; struct geo__point_t i12 = { -90, 10 };

  struct geo__point_t p;

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l1, &r, &p ));
  TEST_ASSERT_INT(p.x, i1.x);
  TEST_ASSERT_INT(p.y, i1.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l2, &r, &p ));
  TEST_ASSERT_INT(p.x, i2.x);
  TEST_ASSERT_INT(p.y, i2.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l3, &r, &p ));
  TEST_ASSERT_INT(p.x, i3.x);
  TEST_ASSERT_INT(p.y, i3.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l4, &r, &p ));
  TEST_ASSERT_INT(p.x, i4.x);
  TEST_ASSERT_INT(p.y, i4.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l5, &r, &p ));
  TEST_ASSERT_INT(p.x, i5.x);
  TEST_ASSERT_INT(p.y, i5.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l6, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l6, &r, &p ));
  TEST_ASSERT_INT(p.x, i6.x);
  TEST_ASSERT_INT(p.y, i6.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l7, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l7, &r, &p ));
  TEST_ASSERT_INT(p.x, i7.x);
  TEST_ASSERT_INT(p.y, i7.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l8, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l8, &r, &p ));
  TEST_ASSERT_INT(p.x, i8.x);
  TEST_ASSERT_INT(p.y, i8.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l9, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l9, &r, &p ));
  TEST_ASSERT_INT(p.x, i9.x);
  TEST_ASSERT_INT(p.y, i9.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l10, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l10, &r, &p ));
  TEST_ASSERT_INT(p.x, i10.x);
  TEST_ASSERT_INT(p.y, i10.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l11, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l11, &r, &p ));
  TEST_ASSERT_INT(p.x, i11.x);
  TEST_ASSERT_INT(p.y, i11.y);

  p.x = 0; p.y = 0;
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l12, &r, NULL ));
  TEST_ASSERT_TRUE(collision__line_intersects_rectangle( &l12, &r, &p ));
  TEST_ASSERT_INT(p.x, i12.x);
  TEST_ASSERT_INT(p.y, i12.y);
}


void point_in_rectangle_middle(){
  struct geo__rect_t r = { -5, -5, 10, 5 };

  struct geo__point_t p1 = { -3, -3 };
  struct geo__point_t p2 = {  0, -2 };
  struct geo__point_t p3 = {  3, -1 };

  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p1, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p2, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p3, &r ));
}

void point_in_rectangle_edge(){
  struct geo__rect_t r = { -5, -5, 10, 5 };

  struct geo__point_t p1 = { -5, -5 };
  struct geo__point_t p2 = {  5, -5 };
  struct geo__point_t p3 = {  5, 0 };
  struct geo__point_t p4 = {  -5, 0 };

  struct geo__point_t p5 = { 0, -5 };
  struct geo__point_t p6 = {  5, -3 };
  struct geo__point_t p7 = {  0, 0 };
  struct geo__point_t p8 = {  -5, -2 };

  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p1, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p2, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p3, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p4, &r ));

  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p5, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p6, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p7, &r ));
  TEST_ASSERT_TRUE(collision__point_in_rectangle( &p8, &r ));
}

void point_outside_rectangle(){
  struct geo__rect_t r = { -5, -5, 10, 5 };

  struct geo__point_t p1 = { -6, -6 };
  struct geo__point_t p2 = {  6, -6 };
  struct geo__point_t p3 = {  6, 0 };
  struct geo__point_t p4 = {  -6, 0 };

  struct geo__point_t p5 = { 0, -6 };
  struct geo__point_t p6 = {  6, -3 };
  struct geo__point_t p7 = {  0, 1 };
  struct geo__point_t p8 = {  -6, -2 };

  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p1, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p2, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p3, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p4, &r ));

  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p5, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p6, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p7, &r ));
  TEST_ASSERT_FALSE(collision__point_in_rectangle( &p8, &r ));
}

void point_on_vertical_line(){

  struct geo__line_t l1 = { 1, 5,    1, 100 };
  struct geo__line_t l2 = { 1, 100,  1,   5 };

  struct geo__point_t end1 = { 1, 5 };
  struct geo__point_t end2 = { 1, 100 };

  struct geo__point_t middle = { 1, 50 };

  struct geo__point_t off_end1 = { 1, 0 };
  struct geo__point_t off_end2 = { 1, 110 };

  struct geo__point_t off_middle = { 0, 50 };

  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l2 ) );
}

void point_on_horizontal_line(){

  struct geo__line_t l1 = { 5,   1,    100, 1 };
  struct geo__line_t l2 = { 100, 1,      5, 1 };

  struct geo__point_t end1 = { 5, 1 };
  struct geo__point_t end2 = { 100, 1 };

  struct geo__point_t middle = { 50, 1 };

  struct geo__point_t off_end1 = { 0, 1 };
  struct geo__point_t off_end2 = { 110, 1 };

  struct geo__point_t off_middle = { 50, 0 };

  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l2 ) );
}

void point_on_diagonal_line(){

  struct geo__line_t l1 = { 5,   1,    105, 5 };
  struct geo__line_t l2 = { 105, 5,      5, 1 };

  struct geo__point_t end1 = { 5, 1 };
  struct geo__point_t end2 = { 105, 5 };

  struct geo__point_t middle = { 55, 3 };

  struct geo__point_t off_end1 = { 205, 9 };
  struct geo__point_t off_end2 = { -95, -3 };

  struct geo__point_t off_middle = { 50, 0 };

  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end1, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &end2, &l2 ) );

  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l1 ) );
  TEST_ASSERT_TRUE( collision__point_on_line( &middle, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end1, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_end2, &l2 ) );

  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l1 ) );
  TEST_ASSERT_FALSE( collision__point_on_line( &off_middle, &l2 ) );

}
 
void moving_rect_doesnt_intersect_no_distance(){

  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -100, -100, 10, 10 };

  struct geo__vector_t v = { 10, 0 };

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, NULL );

  TEST_ASSERT_FALSE( result );
}

void moving_rect_doesnt_intersect(){

  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { -100, -100, 10, 10 };

  struct geo__vector_t v = { 10, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_FALSE( result );
  TEST_ASSERT_INT( distance, INT_MAX );
}

void moving_rect_intersects_no_distance(){

  struct geo__rect_t a = { 0, 0, 10, 10 };
  struct geo__rect_t b = { 0, 0, 20, 10 };

  struct geo__vector_t v = { 10, 0 };

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, NULL );

  TEST_ASSERT_TRUE( result );
} 

void moving_rect_intersects(){

  struct geo__rect_t a = {  0, 0, 10, 10 };
  struct geo__rect_t b = { 20, 0, 10, 10 };

  struct geo__vector_t v = { 10, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_left_to_right_intersects_top(){

  struct geo__rect_t a = {  0, 0, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { 100, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_left_to_right_intersects_bottom(){

  struct geo__rect_t a = {  0, 0, 10, 10 };
  struct geo__rect_t b = { 20, 5, 10, 10 };

  struct geo__vector_t v = { 100, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}


void moving_rect_right_to_left_intersects_top(){

  struct geo__rect_t a = {  40, 0, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { -100, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_right_to_left_intersects_bottom(){

  struct geo__rect_t a = {  40, 0, 10, 10 };
  struct geo__rect_t b = { 20, 5, 10, 10 };

  struct geo__vector_t v = { -100, 0 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_top_to_bottom_intersects_left(){

  struct geo__rect_t a = {  15, 15, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { 0, -100 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_top_to_bottom_intersects_right(){

  struct geo__rect_t a = {  25, 15, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { 0, -100 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_bottom_to_top_intersects_left(){

  struct geo__rect_t a = {  15, -25, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { 0, 100 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
}

void moving_rect_bottom_to_top_intersects_right(){

  struct geo__rect_t a = {  25, -25, 10, 10 };
  struct geo__rect_t b = { 20, -5, 10, 10 };

  struct geo__vector_t v = { 0, 100 };

  int distance = 0;

  bool_t result = 
    collision__moving_rectangle_intersects_rectangle( &a, &v, &b, &distance );

  TEST_ASSERT_TRUE( result );
  TEST_ASSERT_INT( distance, 10 );
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

TEST_CASE(rectangle_doesnt_overlap_touching)

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

TEST_CASE(vertical_parallel_lines_dont_intersect)
TEST_CASE(horizontal_parallel_lines_dont_intersect)
TEST_CASE(diagonal_parallel_lines_dont_intersect)

TEST_CASE(vertical_overlapping_lines_intersect)
TEST_CASE(horizontal_overlapping_lines_intersect)
TEST_CASE(diagonal_overlapping_lines_intersect)

TEST_CASE(horizontal_line_intersects_rectangle)
TEST_CASE(vertical_line_intersects_rectangle)
TEST_CASE(diagonal_line_intersects_rectangle)
TEST_CASE(orthogonal_line_intersects_rectangle_corner)

TEST_CASE(line_from_inside_intersects_rectangle_at_origin)
TEST_CASE(line_on_side_intersects_rectangle_at_origin)

TEST_CASE(point_in_rectangle_middle)
TEST_CASE(point_in_rectangle_edge)
TEST_CASE(point_outside_rectangle)

TEST_CASE(point_on_vertical_line)
TEST_CASE(point_on_horizontal_line)
TEST_CASE(point_on_diagonal_line)

TEST_CASE(moving_rect_doesnt_intersect_no_distance)
TEST_CASE(moving_rect_doesnt_intersect)
TEST_CASE(moving_rect_intersects_no_distance)
TEST_CASE(moving_rect_intersects)
TEST_CASE(moving_rect_left_to_right_intersects_top)
TEST_CASE(moving_rect_left_to_right_intersects_bottom)
TEST_CASE(moving_rect_right_to_left_intersects_top)
TEST_CASE(moving_rect_right_to_left_intersects_bottom)
TEST_CASE(moving_rect_top_to_bottom_intersects_left)
TEST_CASE(moving_rect_top_to_bottom_intersects_right)
TEST_CASE(moving_rect_bottom_to_top_intersects_left)
TEST_CASE(moving_rect_bottom_to_top_intersects_right)

TEST_SUITE_END()
			     
