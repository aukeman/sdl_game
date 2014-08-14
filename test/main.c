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
  while ( iter->test_fxn != NULL && result < 2 ){

    fprintf(fout, " %3d: %-48s", test_idx, iter->title);
    fflush(stdout);

    g_test_result = TRUE;

    if ( setup_fxn != NULL && SUCCESS != setup_fxn() ){
      fprintf(fout, "ERROR.  cannot complete setup.  ending test.\n");
      result = 2;
    }
    else{

      iter->test_fxn();

      if ( g_test_result ){
	fprintf( fout, "...ok\n" );
      }
      else{
	fprintf( fout, "...FAILED\n" );
	result = 1;
      }

      if ( teardown_fxn != NULL && SUCCESS != teardown_fxn() ){
	fprintf(fout, "ERROR.  cannot complete teardown.  ending test.\n");
	result = 3;
      }
    }

    ++test_idx;
    ++iter;
  }
  
  return result;
}
