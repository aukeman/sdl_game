#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <constants.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

typedef void test_fxn_t();
typedef int setup_fxn_t();
typedef int teardown_fxn_t();

typedef struct {
  
  test_fxn_t* test_fxn;
  const char* title;

} test_case_t;

extern const char* test_suite_name;

extern setup_fxn_t* setup_fxn;
extern setup_fxn_t* teardown_fxn;

extern test_case_t test_suite[];

extern int g_test_result;

#define TEST_SUITE_WITH_SETUP_START(name, setup, teardown) \
setup_fxn_t* setup_fxn = setup;                            \
teardown_fxn_t* teardown_fxn = teardown;                   \
const char* test_suite_name = #name;                       \
test_case_t test_suite[] = {

#define TEST_SUITE_START(name) TEST_SUITE_WITH_SETUP_START(name, NULL, NULL)

#define __ASSERTION_LOCATION fprintf(stderr, "\nFILE %s LINE %d failed assertion: ", __FILE__, __LINE__);

#define __ASSERTION_FAIL g_test_result = FALSE; return

#define __ASSERTION_EQUALITY(comparison_result, actual, expected, fmt )	\
if ( !(comparison_result) ) {						\
  __ASSERTION_LOCATION;                  \
  fprintf(stderr, "\"%s\" expected: %"#fmt" actual: %"#fmt"\n", #actual" == "#expected, (expected), (actual));                     \
  __ASSERTION_FAIL; }

#define __ASSERTION_INEQUALITY(comparison_result, actual, expected, fmt )	\
if ( !(comparison_result) ) {						\
  __ASSERTION_LOCATION;                  \
  fprintf(stderr, "\"%s\" expected: %"#fmt" actual: %"#fmt"\n", #actual" != "#expected, (expected), (actual));                     \
  __ASSERTION_FAIL; }

#define TEST_CASE(tc) { tc, #tc },
#define TEST_SUITE_END() {NULL, NULL} };

#define TEST_ASSERT_TRUE(a)      \
if ( !(a) ) {                    \
  __ASSERTION_LOCATION;          \
  fprintf(stderr, "\"%s\"", #a); \
  __ASSERTION_FAIL; }

#define TEST_ASSERT_FALSE(a) \
if ( (a) ) {                          \
  __ASSERTION_LOCATION;               \
  fprintf(stderr, " NOT \"%s\"", #a); \
  __ASSERTION_FAIL; }

#define TEST_ASSERT_INT(actual,expected) \
  __ASSERTION_EQUALITY( (actual) == (expected), actual, expected, d )

#define TEST_ASSERT_FLOAT_EPSILON(actual,expected, epsilon) \
  __ASSERTION_EQUALITY( fabs((actual)-(expected)) <= (epsilon), actual, expected, f )

#define TEST_ASSERT_FLOAT(actual,expected) \
  TEST_ASSERT_FLOAT_EPSILON(actual, expected, 0.000001f)

#define TEST_ASSERT_PTR(  actual,expected) \
  __ASSERTION_EQUALITY( (actual) == (expected), (void*)actual, (void*)expected, p )

#define TEST_ASSERT_NULL( actual ) \
  TEST_ASSERT_PTR(actual, (void*)NULL)

#define TEST_ASSERT_NOT_NULL( actual ) \
  __ASSERTION_INEQUALITY( (actual) != NULL, (void*)actual, (void*)NULL, p )

#define TEST_ASSERT_SUCCESS( actual ) \
  TEST_ASSERT_INT(actual, SUCCESS)

#define TEST_ASSERT_FAIL( actual ) \
  __ASSERTION_INEQUALITY( (actual) != SUCCESS, actual, SUCCESS, d )


#endif

