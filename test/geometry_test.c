#include <geometry.h>
#include <test_utils.h>

void distance_squared_test_x_axis(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 10, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_test_y_axis(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 0, 10 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_both_axes(){

  geo__point_t a = { 10, 10 };
  geo__point_t b = { 20, 20 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 200 );
}

void distance_squared_test_negative_x_axis(){

  geo__point_t a = { -10, 0 };
  geo__point_t b = { 0, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_test_negative_y_axis(){

  geo__point_t a = { 0, -10 };
  geo__point_t b = { 0, 0 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 100 );
}

void distance_squared_both_negative_axes(){

  geo__point_t a = { -20, -20 };
  geo__point_t b = { -10, -10 };

  TEST_ASSERT_INT( geo__distance_squared(&a, &b), 200 );
}

void distance_test_x_axis(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 10, 0 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 10 );
}

void distance_test_y_axis(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 0, 10 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 10 );
}

void distance_test_x_axis_round_up(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 9.75, 0 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 10 );
}

void distance_test_both_axes(){

  geo__point_t a = { 0, 0 };
  geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}

void distance_test_round_up(){

  geo__point_t a = { 1, 0 };
  geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}

void distance_test_round_down(){

  geo__point_t a = { -1, 0 };
  geo__point_t b = { 5, 12 };

  TEST_ASSERT_INT( geo__distance(&a, &b), 13 );
}


TEST_SUITE_START(Geometry Tests)
  TEST_CASE(distance_squared_test_x_axis)
  TEST_CASE(distance_squared_test_y_axis)
  TEST_CASE(distance_squared_both_axes)
  TEST_CASE(distance_squared_test_negative_x_axis)
  TEST_CASE(distance_squared_test_negative_y_axis)
  TEST_CASE(distance_squared_both_negative_axes)
  TEST_CASE(distance_test_x_axis)
  TEST_CASE(distance_test_y_axis)
  TEST_CASE(distance_test_both_axes)
  TEST_CASE(distance_test_round_up)
  TEST_CASE(distance_test_round_down)
TEST_SUITE_END()
