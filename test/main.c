#include <test_utils.h>
#include <constants.h>
#include <stdio.h>

int g_test_result = TRUE;

int main(int argc, char** argv){

  int result = 0;

  test_case_t* iter = test_suite;

  FILE* fout = stdout;

  fprintf(fout, "%s\n", test_suite_name);

  int test_idx = 0;
  while ( iter->test_fxn != NULL ){

    fprintf(fout, " %3d: %-48s", test_idx, iter->title);
    fflush(stdout);

    g_test_result = TRUE;

    iter->test_fxn();

    if ( g_test_result ){
      fprintf( fout, "...ok\n" );
    }
    else{
      fprintf( fout, "...FAILED\n" );
      result = 1;
    }

    ++test_idx;
    ++iter;
  }
  
  return result;
}
