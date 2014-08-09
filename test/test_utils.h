#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <constants.h>
#include <stdio.h>

typedef void test_fxn_t();

typedef struct {
  
  test_fxn_t* test_fxn;
  const char* title;

} test_case_t;

extern const char* test_suite_name;
extern test_case_t test_suite[];

extern int g_test_result;

#define TEST_SUITE_START(name) const char* test_suite_name = #name; test_case_t test_suite[] = {

#define __ASSERTION_LOCATION fprintf(stderr, "\nFILE %s LINE %d failed assertion: ", __FILE__, __LINE__);

#define __ASSERTION_FAIL g_test_result = FALSE; return

#define __ASSERTION_EQUALITY(comparison_result, actual, expected, fmt )	\
if ( !(comparison_result) ) {						\
  __ASSERTION_LOCATION;                  \
  fprintf(stderr, "\"%s\" expected: %"#fmt" actual: %"#fmt"\n", #actual" == "#expected, (expected), (actual));                     \
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
  

#endif
