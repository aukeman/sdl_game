#include <font.h>
#include <test_utils.h>
#include <stdio.h>
#include <unistd.h>

void no_font_file(){

  struct font__handle_t* font;

  int rc = font__create( "", &font );

  TEST_ASSERT_INT( rc, FONT__CONFIG_FILE_NOT_FOUND );
  TEST_ASSERT_PTR( font, NULL );
}

void no_image_file(){

  struct font__handle_t* font;

  const char* name = tmpnam(NULL);

  FILE* fout = fopen(name, "w");
  
  TEST_ASSERT_TRUE( fout != NULL );

  fprintf(fout, "dummy_filename\n");
  fprintf(fout, "a  0 0 8 8\n");
  fprintf(fout, "b  8 0 8 8\n");
  fprintf(fout, "c 16 0 8 8\n");

  fclose(fout);

  int rc = font__create( name, &font );

  unlink(name);
  
  TEST_ASSERT_INT( rc, FONT__IMAGE_NOT_FOUND );
  TEST_ASSERT_PTR( font, NULL );
}


TEST_SUITE_START(Font Tests)
  TEST_CASE(no_font_file)
  TEST_CASE(no_image_file)
TEST_SUITE_END()
