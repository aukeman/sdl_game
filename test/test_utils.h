#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <constants.h>
#include <stdio.h>

typedef int test_fxn_t();

typedef struct {
  
  test_fxn_t* test_fxn;
  const char* title;

} test_case_t;

extern const char* test_suite_name;
extern test_case_t test_suite[];

#define TEST_SUITE_START(name) const char* test_suite_name = #name; test_case_t test_suite[] = {

#define __ASSERTION_LOCATION fprintf(stderr, "\nFILE %s LINE %d failed assertion: ", __FILE__, __LINE__);

#define TEST_CASE(tc) { tc, #tc },
#define TEST_SUITE_END() {NULL, NULL} };

#define TEST_OK() return TRUE
#define TEST_ASSERT_TRUE(a) if ( !(a) ) { __ASSERTION_LOCATION; fprintf(stderr, "\"%s\"", #a); return FALSE; }

#define TEST_ASSERT_FALSE(a) if ( (a) ) { __ASSERTION_LOCATION; fprintf(stderr, " NOT \"%s\"", #a); return FALSE; }

#define TEST_ASSERT_INT(actual,expected) if ((actual) != (expected)) { __ASSERTION_LOCATION; fprintf(stderr, "\"%s\" expected: %d actual: %d\n", #actual" == "#expected, (expected), (actual)); return FALSE; }

#endif
