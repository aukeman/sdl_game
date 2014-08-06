#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <constants.h>

typedef int test_fxn_t();

typedef struct {
  
  test_fxn_t* test_fxn;
  const char* title;

} test_case_t;

extern const char* test_suite_name;
extern test_case_t test_suite[];

#define TEST_SUITE_START(name) const char* test_suite_name = #name; test_case_t test_suite[] = {

#define TEST_CASE(tc) { tc, #tc },
#define TEST_SUITE_END() {NULL, NULL} };

#endif
