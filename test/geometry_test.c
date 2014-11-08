#include <geometry.h>
#include <test_utils.h>

void distance_squared_0(){

  struct geo__point_t a = { 10, 15 };
  struct geo__point_t b = { 10, 15 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 0 );
}

void distance_squared_test_x_axis(){

  struct geo__point_t a = { 0, 0 };
  struct geo__point_t b = { 10, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_test_y_axis(){

  struct geo__point_t a = { 0, 0 };
  struct geo__point_t b = { 0, 10 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_both_axes(){

  struct geo__point_t a = { 10, 10 };
  struct geo__point_t b = { 20, 20 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 200 );
}

void distance_squared_test_negative_x_axis(){

  struct geo__point_t a = { -10, 0 };
  struct geo__point_t b = { 0, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_test_negative_y_axis(){

  struct geo__point_t a = { 0, -10 };
  struct geo__point_t b = { 0, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_both_negative_axes(){

  struct geo__point_t a = { -20, -20 };
  struct geo__point_t b = { -10, -10 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 200 );
}

void distance_test_0(){

  struct geo__point_t a = { 10, 15 };
  struct geo__point_t b = { 10, 15 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 0 );
}

void distance_test_x_axis(){

  struct geo__point_t a = { 0, 0 };
  struct geo__point_t b = { 10, 0 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 10 );
}

void distance_test_y_axis(){

  struct geo__point_t a = { 0, 0 };
  struct geo__point_t b = { 0, 10 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 10 );
}

void distance_test_both_axes(){

  struct geo__point_t a = { 0, 0 };
  struct geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}

void distance_test_round_up(){

  struct geo__point_t a = { 1, 0 };
  struct geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}

void distance_test_round_down(){

  struct geo__point_t a = { -1, 0 };
  struct geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}

void length_squared_0(){

  struct geo__line_t l = { 10, 15,  10, 15 };

  TEST_ASSERT_INT( geo__length_squared(&l), 0 );
}

void length_squared_test_x_axis(){

  struct geo__line_t l = { 0, 0,  10, 0 };

  TEST_ASSERT_INT( geo__length_squared(&l), 100 );
}

void length_squared_test_y_axis(){

  struct geo__line_t l = { 0, 0,  0, 10 };

  TEST_ASSERT_INT( geo__length_squared(&l), 100 );
}

void length_squared_both_axes(){

  struct geo__line_t l = { 10, 10,  20, 20 };

  TEST_ASSERT_INT( geo__length_squared(&l), 200 );
}

void length_squared_test_negative_x_axis(){

  struct geo__line_t l = { -10, 0,  0, 0 };

  TEST_ASSERT_INT( geo__length_squared(&l), 100 );
}

void length_squared_test_negative_y_axis(){

  struct geo__line_t l = { 0, -10,  0, 0 };

  TEST_ASSERT_INT( geo__length_squared(&l), 100 );
}

void length_squared_both_negative_axes(){

  struct geo__line_t l = { -20, -20,  -10, -10 };

  TEST_ASSERT_INT( geo__length_squared(&l), 200 );
}

void length_0(){

  struct geo__line_t l = { 10, 15,  10, 15 };

  TEST_ASSERT_INT( geo__length_squared(&l), 0 );
}

void length_test_x_axis(){

  struct geo__line_t l = { 0, 0,  10, 0 };

  TEST_ASSERT_INT( geo__length(&l), 10 );
}

void length_test_y_axis(){

  struct geo__line_t l = { 0, 0,  0, 10 };

  TEST_ASSERT_INT( geo__length(&l), 10 );
}

void length_test_both_axes(){

  struct geo__line_t l = { 0, 0,  5, 12 };

  TEST_ASSERT_INT( geo__length(&l), 13 );
}

void length_test_round_up(){

  struct geo__line_t l = { 1, 0,  5, 12 };

  TEST_ASSERT_INT( geo__length(&l), 13 );
}

void length_test_round_down(){

  struct geo__line_t l = { -1, 0,  5, 12 };

  TEST_ASSERT_INT( geo__length(&l), 13 );
}

void point_on_0_length_line(){

  struct geo__line_t l = { 10, 15,  10, 15 };
  int distance = 10;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y, 15 );
}

void point_on_line_at_0_distance(){

  struct geo__line_t l = { 10, 0,  20, 0 };
  int distance = 0;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 10 );
  TEST_ASSERT_INT( p.y,  0 );
}

void point_on_x_axis_line_at_100_distance(){

  struct geo__line_t l = { 10, 0,  20, 0 };
  int distance = 100;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 110 );
  TEST_ASSERT_INT( p.y,   0 );
}

void point_on_y_axis_line_at_100_distance(){

  struct geo__line_t l = { 0, 10,  0, 20 };
  int distance = 100;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x,   0 );
  TEST_ASSERT_INT( p.y, 110 );
}

void point_on_negative_x_axis_line_at_100_distance(){

  struct geo__line_t l = { 20, 0,  10, 0 };
  int distance = 100;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, -80 );
  TEST_ASSERT_INT( p.y,   0 );
}

void point_on_negative_y_axis_line_at_100_distance(){

  struct geo__line_t l = { 0, 20,  0, 10 };
  int distance = 100;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x,   0 );
  TEST_ASSERT_INT( p.y, -80 );
}

void point_on_both_axes_line_at_longer_distance(){

  struct geo__line_t l = { 10, 10,  15, 22 };
  int distance = 26;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 20 );
  TEST_ASSERT_INT( p.y, 34 );
}

void point_on_both_axes_line_at_shorter_distance(){

  struct geo__line_t l = { 10, 10,  20, 34 };
  int distance = 13;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 15 );
  TEST_ASSERT_INT( p.y, 22 );
}

void point_on_both_axes_line_at_negative_distance(){

  struct geo__line_t l = { 10, 10,  20, 34 };
  int distance = -13;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 5 );
  TEST_ASSERT_INT( p.y, -2 );
}

void point_on_both_axes_line_round_up(){

  struct geo__line_t l = { 10, 10,  15, 22 };
  int distance = 25;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 19 );
  TEST_ASSERT_INT( p.y, 33 );
}

void point_on_both_axes_line_round_down(){

  struct geo__line_t l = { 10, 10,  15, 22 };
  int distance = 27;

  struct geo__point_t p;
  geo__point_on_line_at_distance( &l, distance, &p );

  TEST_ASSERT_INT( p.x, 20 );
  TEST_ASSERT_INT( p.y, 34 );
}

TEST_SUITE_START(Geometry Tests)
  TEST_CASE(distance_squared_0)
  TEST_CASE(distance_squared_test_x_axis)
  TEST_CASE(distance_squared_test_y_axis)
  TEST_CASE(distance_squared_both_axes)
  TEST_CASE(distance_squared_test_negative_x_axis)
  TEST_CASE(distance_squared_test_negative_y_axis)
  TEST_CASE(distance_squared_both_negative_axes)
  TEST_CASE(distance_test_0)
  TEST_CASE(distance_test_x_axis)
  TEST_CASE(distance_test_y_axis)
  TEST_CASE(distance_test_both_axes)
  TEST_CASE(distance_test_round_up)
  TEST_CASE(distance_test_round_down)

  TEST_CASE(length_squared_0)
  TEST_CASE(length_squared_test_x_axis)
  TEST_CASE(length_squared_test_y_axis)
  TEST_CASE(length_squared_both_axes)
  TEST_CASE(length_squared_test_negative_x_axis)
  TEST_CASE(length_squared_test_negative_y_axis)
  TEST_CASE(length_squared_both_negative_axes)
  TEST_CASE(length_0)
  TEST_CASE(length_test_x_axis)
  TEST_CASE(length_test_y_axis)
  TEST_CASE(length_test_both_axes)
  TEST_CASE(length_test_round_up)
  TEST_CASE(length_test_round_down)

  TEST_CASE(point_on_0_length_line)
  TEST_CASE(point_on_line_at_0_distance)
  TEST_CASE(point_on_x_axis_line_at_100_distance)
  TEST_CASE(point_on_y_axis_line_at_100_distance)
  TEST_CASE(point_on_negative_x_axis_line_at_100_distance)
  TEST_CASE(point_on_negative_y_axis_line_at_100_distance)
  TEST_CASE(point_on_both_axes_line_at_longer_distance)
  TEST_CASE(point_on_both_axes_line_at_shorter_distance)
  TEST_CASE(point_on_both_axes_line_at_negative_distance)
  TEST_CASE(point_on_both_axes_line_round_up)
  TEST_CASE(point_on_both_axes_line_round_down)

TEST_SUITE_END()
