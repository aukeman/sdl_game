#include <utils.h>
#include <test_utils.h>
#include <limits.h>

void sqrt_test(){
  TEST_ASSERT_INT( utils__sqrt(100), 10 );
}

void sqrt_0_test(){
  TEST_ASSERT_INT( utils__sqrt(0), 0 );
}

void sqrt_round_up_test(){
  TEST_ASSERT_INT( utils__sqrt(99), 10 );
}

void sqrt_round_down_test(){
  TEST_ASSERT_INT( utils__sqrt(101), 10 );
}

void sqrt_neg_1_test(){
  TEST_ASSERT_INT( utils__sqrt(-1), 0 );
}

void sqrt_neg_100_test(){
  TEST_ASSERT_INT( utils__sqrt(-100), 0 );
}

void round_up_positive_test(){
  TEST_ASSERT_INT( utils__round(0.5f), 1 );
}

void round_down_positive_test(){
  TEST_ASSERT_INT( utils__round(0.49f), 0 );
}

void round_0_test(){
  TEST_ASSERT_INT( utils__round(0.0f), 0 );
}

void round_up_negative_test(){
  TEST_ASSERT_INT( utils__round(-0.49f), 0 );
}

void round_down_negative_test(){
  TEST_ASSERT_INT( utils__round(-0.5f), -1 );
}

TEST_SUITE_START(Utils Test)
  TEST_CASE(sqrt_test)
  TEST_CASE(sqrt_0_test)
  TEST_CASE(sqrt_round_up_test)
  TEST_CASE(sqrt_round_down_test)
  TEST_CASE(sqrt_neg_1_test)
  TEST_CASE(sqrt_neg_100_test)
  TEST_CASE(round_up_positive_test)
  TEST_CASE(round_down_positive_test)
  TEST_CASE(round_0_test)
  TEST_CASE(round_up_negative_test)
  TEST_CASE(round_down_negative_test)
TEST_SUITE_END()
