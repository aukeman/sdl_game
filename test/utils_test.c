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

TEST_SUITE_START(Utils Test)
  TEST_CASE(sqrt_test)
  TEST_CASE(sqrt_0_test)
  TEST_CASE(sqrt_round_up_test)
  TEST_CASE(sqrt_round_down_test)
  TEST_CASE(sqrt_neg_1_test)
  TEST_CASE(sqrt_neg_100_test)
TEST_SUITE_END()
