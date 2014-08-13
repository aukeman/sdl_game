#include <font.h>
#include <test_utils.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <SDL/SDL.h>
#include <geometry.h>

typedef geo__rect_t ascii_to_rect_t[256];
struct font__handle_t {
  struct video__texture_handle_t* texture;
  ascii_to_rect_t ascii_to_rect;
} ;


const char* write_test_font_file(const char* contents, ...);
const char* write_test_image_file();


void no_font_file(){

  struct font__handle_t* font;

  int rc = font__create( "", &font );

  TEST_ASSERT_INT( rc, FONT__CONFIG_FILE_NOT_FOUND );
  TEST_ASSERT_PTR( font, NULL );
}

void no_image_file(){

  struct font__handle_t* font;

  const char* tmp = write_test_font_file("dummy_filename\n"
					 "a  0 0 8 8\n"
					 "b  8 0 8 8\n"
					 "c 16 0 8 8\n");

  TEST_ASSERT_TRUE( tmp != NULL );

  int rc = font__create( tmp, &font );

  unlink(tmp);
  
  TEST_ASSERT_INT( rc, FONT__IMAGE_NOT_FOUND );
  TEST_ASSERT_PTR( font, NULL );
}

void load_font_file(){

  video__setup(32, 32, FALSE);

  struct font__handle_t* font;

  const char* image_file = write_test_image_file();

  TEST_ASSERT_TRUE( image_file != NULL );

  const char* font_file = write_test_font_file("%s\n"
					       "   0 0 8 8\n"
					       "a  0 0 8 8\n"
					       "b  8 0 8 8\n"
					       "c 16 0 8 8\n"
					       "A  0 8 8 8\n"
					       "B  8 8 8 8\n"
					       "C  16 8 8 8\n"
					       "1  0 16 8 8\n"
					       "2  8 16 8 8\n"
					       "3  16 16 8 8\n",
					       image_file);

  TEST_ASSERT_TRUE( font_file != NULL );

  int rc = font__create( font_file, &font );

  unlink(image_file);
  unlink(font_file);

  ascii_to_rect_t ascii_to_rect;
  memcpy(ascii_to_rect, font->ascii_to_rect, sizeof(ascii_to_rect));
  
  font__free(font);
  video__teardown();

  TEST_ASSERT_INT( rc, SUCCESS );
  TEST_ASSERT_TRUE( font != NULL );

  int idx = 0;
  for ( idx = 0; idx < 256; ++idx ){
    switch(idx){
    case ' ': 
    case 'a': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case 'b': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case 'c': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case 'A': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case 'B': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case 'C': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case '1': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case '2': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case '3': 
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 16);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    case '\t':
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 8*4);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 0);
      break;
    case '\n':
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 8);
      break;
    default:
      TEST_ASSERT_INT(ascii_to_rect[idx].x, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].y, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].width, 0);
      TEST_ASSERT_INT(ascii_to_rect[idx].height, 0);
      break;
    }
  }

}

void test_long_string(){

  video__setup(32, 32, FALSE);

  struct font__handle_t* font;

  const char* image_file = write_test_image_file();

  TEST_ASSERT_TRUE( image_file != NULL );

  const char* font_file = write_test_font_file("%s\n"
					       "   0 0 8 8\n"
					       "a  0 0 8 8\n"
					       "b  8 0 8 8\n"
					       "c 16 0 8 8\n"
					       "A  0 8 8 8\n"
					       "B  8 8 8 8\n"
					       "C  16 8 8 8\n"
					       "1  0 16 8 8\n"
					       "2  8 16 8 8\n"
					       "3  16 16 8 8\n",
					       image_file);

  TEST_ASSERT_TRUE( font_file != NULL );

  int rc = font__create( font_file, &font );

  unlink(image_file);
  unlink(font_file);

  char buffer[2048];
  memset(buffer, '\0', sizeof(buffer));
  memset(buffer, 'a', 1023);
  
  int w_1023, h_1023, w_1024, h_1024, w_2047, h_2047;

  int rc_1023 = font__dimensions(font, &w_1023, &h_1023, buffer);

  memset(buffer, 'b', 1024);
  
  int rc_1024 = font__dimensions(font, &w_1024, &h_1024, buffer);
  
  memset(buffer, 'c', 2047);
  
  int rc_2047 = font__dimensions(font, &w_2047, &h_2047, buffer);
  
  font__free(font);
  video__teardown();

  TEST_ASSERT_INT(rc_1023, SUCCESS);
  TEST_ASSERT_INT(w_1023, 1023*8);
  TEST_ASSERT_INT(h_1023, 1*8);
  
  TEST_ASSERT_INT(rc_1024, FONT__STRING_TOO_LONG);
  TEST_ASSERT_INT(w_1024, 0);
  TEST_ASSERT_INT(h_1024, 0);
  
  TEST_ASSERT_INT(rc_2047, FONT__STRING_TOO_LONG);
  TEST_ASSERT_INT(w_2047, 0);
  TEST_ASSERT_INT(h_2047, 0);
}

void font_dimensions(){

  video__setup(32, 32, FALSE);

  struct font__handle_t* font;

  const char* image_file = write_test_image_file();

  TEST_ASSERT_TRUE( image_file != NULL );

  const char* font_file = write_test_font_file("%s\n"
					       "   0 0 8 8\n"
					       "a  0 0 8 8\n"
					       "b  8 0 8 8\n"
					       "c 16 0 8 8\n"
					       "A  0 8 8 8\n"
					       "B  8 8 8 8\n"
					       "C  16 8 8 8\n"
					       "1  0 16 8 8\n"
					       "2  8 16 8 8\n"
					       "3  16 16 8 8\n",
					       image_file);

  TEST_ASSERT_TRUE( font_file != NULL );

  int rc = font__create( font_file, &font );

  unlink(image_file);
  unlink(font_file);


}


TEST_SUITE_START(Font Tests)
  TEST_CASE(no_font_file)
  TEST_CASE(no_image_file)
  TEST_CASE(load_font_file)
  TEST_CASE(test_long_string)
TEST_SUITE_END()

const char* write_test_font_file(const char* contents, ...){

  static char filename_buffer[1024];
  snprintf( filename_buffer, 1023, "%s", tmpnam(NULL) );

  FILE* fout = fopen(filename_buffer, "w");

  size_t len = strlen(contents);

  if ( !fout ){
    return NULL;
  }

  va_list ap;
  va_start(ap, contents);
  size_t characters_written = vfprintf(fout, contents, ap);
  va_end(ap);

  fclose(fout);

  if (characters_written < len){
    unlink(filename_buffer);
    return NULL;
  }
  else{
    return filename_buffer;
  }
}

const char* write_test_image_file(){

  static char filename_buffer[1024];
  snprintf( filename_buffer, 1023, "%s", tmpnam(NULL) );

  static const unsigned char test_png_image_data[] = {
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x20,
    0x08, 0x06, 0x00, 0x00, 0x00, 0xda, 0x22, 0x70, 0x25, 0x00, 0x00, 0x00,
    0x01, 0x73, 0x52, 0x47, 0x42, 0x00, 0xae, 0xce, 0x1c, 0xe9, 0x00, 0x00,
    0x00, 0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff,
    0xa0, 0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73,
    0x00, 0x00, 0x0b, 0x13, 0x00, 0x00, 0x0b, 0x13, 0x01, 0x00, 0x9a, 0x9c,
    0x18, 0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xde, 0x08,
    0x0c, 0x02, 0x25, 0x11, 0x1f, 0x81, 0x07, 0xfd, 0x00, 0x00, 0x00, 0x19,
    0x74, 0x45, 0x58, 0x74, 0x43, 0x6f, 0x6d, 0x6d, 0x65, 0x6e, 0x74, 0x00,
    0x43, 0x72, 0x65, 0x61, 0x74, 0x65, 0x64, 0x20, 0x77, 0x69, 0x74, 0x68,
    0x20, 0x47, 0x49, 0x4d, 0x50, 0x57, 0x81, 0x0e, 0x17, 0x00, 0x00, 0x02,
    0xa4, 0x49, 0x44, 0x41, 0x54, 0x78, 0xda, 0xed, 0x5b, 0xdb, 0xae, 0xc4,
    0x20, 0x08, 0x5c, 0x8d, 0xff, 0xff, 0xcb, 0x9e, 0x87, 0x93, 0x26, 0x8d,
    0x2b, 0x30, 0x30, 0x5a, 0xdb, 0xad, 0xbe, 0x6d, 0xf1, 0x8a, 0x20, 0x33,
    0xe8, 0xa6, 0x5a, 0x6b, 0xfd, 0xec, 0xf2, 0xda, 0x92, 0xb7, 0x0a, 0xb6,
    0x01, 0x98, 0x25, 0xa5, 0x34, 0x64, 0xb0, 0x68, 0x3f, 0x29, 0x25, 0xb5,
    0xed, 0x21, 0x97, 0xea, 0xb0, 0xf3, 0xb7, 0xda, 0x8f, 0xd2, 0xcf, 0xac,
    0xa2, 0xe9, 0xe7, 0x31, 0x27, 0x80, 0x14, 0xa9, 0x52, 0x4a, 0x9f, 0x5a,
    0xeb, 0x67, 0x47, 0x32, 0x5b, 0x7f, 0xb5, 0xd6, 0x2f, 0x23, 0x28, 0xad,
    0x05, 0x6b, 0x8a, 0x96, 0xe4, 0x9a, 0xcc, 0xeb, 0x41, 0xb3, 0x36, 0xd2,
    0x9a, 0xe3, 0xa8, 0x31, 0x22, 0xfd, 0x4b, 0xfa, 0x39, 0xfa, 0xb2, 0xf4,
    0xc3, 0xe8, 0x2f, 0xb3, 0x16, 0x64, 0x79, 0x20, 0x22, 0x67, 0x2d, 0xdb,
    0x0a, 0x11, 0xb3, 0x37, 0x9e, 0xd9, 0xfc, 0x76, 0x7e, 0x92, 0xae, 0xd8,
    0x13, 0xf0, 0xd0, 0x51, 0x5b, 0xaf, 0x9c, 0x3f, 0xae, 0x8a, 0x65, 0xec,
    0xe6, 0xac, 0x9e, 0xff, 0x93, 0x43, 0x68, 0xe9, 0x79, 0xb5, 0xe7, 0x08,
    0x6d, 0xdb, 0x5b, 0x72, 0xc9, 0x83, 0x19, 0x63, 0xb8, 0xe2, 0x78, 0x47,
    0x4f, 0xa2, 0xab, 0xe7, 0x80, 0xec, 0x9f, 0x36, 0xa7, 0xb4, 0xf3, 0x00,
    0x9b, 0x06, 0xee, 0xf2, 0xe2, 0x02, 0xb3, 0x80, 0x5d, 0xe2, 0x20, 0xd0,
    0xc3, 0xb2, 0x7a, 0x75, 0x58, 0x16, 0xa0, 0xc9, 0xa7, 0x83, 0xc0, 0x15,
    0x71, 0xf1, 0x8e, 0x46, 0x70, 0xd6, 0x71, 0x6f, 0x83, 0x2d, 0x1d, 0x69,
    0xed, 0x11, 0xc7, 0x95, 0xe4, 0x70, 0x26, 0x50, 0xcb, 0xb2, 0x21, 0x99,
    0x3a, 0x04, 0xc4, 0xf5, 0xea, 0x9d, 0xbf, 0xa1, 0xf2, 0x1e, 0x55, 0x8d,
    0xf6, 0x8f, 0xac, 0x11, 0x35, 0xf2, 0xe8, 0xe6, 0x7b, 0x41, 0x9f, 0x0b,
    0x03, 0xa0, 0x1d, 0x5a, 0x96, 0xad, 0x2d, 0x82, 0xe5, 0xc7, 0x08, 0x8b,
    0x40, 0xea, 0x48, 0x9e, 0x83, 0xa2, 0x68, 0x66, 0x1d, 0xc7, 0xf8, 0xb3,
    0xa8, 0x6a, 0xd4, 0x09, 0xf3, 0x08, 0xab, 0x42, 0x93, 0x31, 0x57, 0x84,
    0x12, 0xc9, 0xcb, 0xd8, 0x18, 0x6e, 0xc9, 0xb4, 0x53, 0xc6, 0x72, 0x20,
    0x8f, 0xde, 0x34, 0x43, 0x8e, 0x38, 0x61, 0x61, 0xbd, 0x14, 0xf1, 0x1e,
    0x84, 0xa7, 0x7b, 0x72, 0x05, 0x9e, 0x64, 0x50, 0x8b, 0x71, 0xbc, 0xa7,
    0x40, 0xbb, 0xb1, 0x48, 0x8c, 0x8f, 0xe4, 0x2a, 0x3c, 0x40, 0x2e, 0x0a,
    0x34, 0x97, 0xe6, 0x01, 0xee, 0x0e, 0x06, 0xdf, 0x04, 0x56, 0x77, 0x1e,
    0x60, 0x6f, 0xfe, 0xb7, 0x01, 0x68, 0x48, 0xd7, 0x83, 0xc4, 0xd9, 0x30,
    0x12, 0xe9, 0x5f, 0x42, 0xfd, 0x68, 0xff, 0x67, 0x0c, 0x23, 0x8d, 0xcf,
    0xbc, 0x49, 0x40, 0x58, 0x04, 0x2a, 0xf7, 0x82, 0x3d, 0xeb, 0x77, 0xb9,
    0x93, 0x07, 0x8c, 0x98, 0x47, 0x84, 0xaa, 0xb5, 0xb2, 0x5e, 0x5d, 0x0f,
    0x8f, 0xf7, 0xb6, 0x47, 0x59, 0x94, 0xd4, 0x96, 0xd1, 0x5b, 0xfe, 0x95,
    0xcd, 0x67, 0x5e, 0x1b, 0xa1, 0xe3, 0xb2, 0x3c, 0xfe, 0x8e, 0x97, 0x45,
    0x05, 0x45, 0x92, 0x4f, 0xb8, 0x6a, 0xd5, 0xb8, 0x36, 0x4b, 0x75, 0x67,
    0x3e, 0x2b, 0x43, 0x99, 0xd2, 0x0c, 0x27, 0xcb, 0xed, 0xe0, 0x1a, 0xcd,
    0x61, 0xa8, 0x22, 0x92, 0xaa, 0x64, 0x73, 0x11, 0x33, 0x3c, 0xc4, 0x5a,
    0xbf, 0xb7, 0xbd, 0x96, 0x27, 0x98, 0x95, 0x28, 0xd2, 0xfa, 0x2d, 0x2c,
    0x8f, 0x9c, 0x31, 0xd1, 0xc8, 0xb3, 0x33, 0x64, 0xde, 0x08, 0xdf, 0x8f,
    0xae, 0x9f, 0x69, 0x3f, 0x42, 0xf7, 0xd1, 0x6c, 0xee, 0x7e, 0x0f, 0xf0,
    0x72, 0x4a, 0xb9, 0xf3, 0x00, 0x13, 0x4e, 0xb1, 0x47, 0x19, 0xec, 0xff,
    0x9c, 0xab, 0x4a, 0x67, 0xac, 0x74, 0xaa, 0x46, 0x83, 0x24, 0x40, 0xe6,
    0x39, 0x02, 0x47, 0xa6, 0x51, 0xad, 0x74, 0x74, 0xb4, 0x3d, 0xf2, 0x6a,
    0x7a, 0xa6, 0x1c, 0x49, 0xb3, 0xf7, 0xea, 0x64, 0xa9, 0xb2, 0x87, 0xf2,
    0xb4, 0x97, 0x21, 0xe7, 0x3a, 0x07, 0xb8, 0xb1, 0x6e, 0xd4, 0xd8, 0xd8,
    0x6e, 0xb5, 0x47, 0x5e, 0xcd, 0x46, 0xc7, 0xb0, 0x5e, 0xe6, 0x22, 0xe3,
    0xb3, 0xf2, 0x28, 0x00, 0xcd, 0xe8, 0x45, 0x0b, 0x5a, 0x67, 0xe6, 0xd5,
    0xef, 0xca, 0xb8, 0x8d, 0x66, 0x02, 0x57, 0x72, 0xfa, 0xc8, 0x1a, 0xf3,
    0x55, 0xe0, 0x65, 0x75, 0xff, 0xcc, 0x63, 0x0e, 0xd4, 0x83, 0x2d, 0x96,
    0x11, 0xbd, 0x0d, 0x65, 0x0d, 0x4c, 0x1b, 0x3b, 0x5f, 0xb1, 0x79, 0xcc,
    0x5d, 0xfe, 0xc8, 0x3c, 0x03, 0xa2, 0xdc, 0x95, 0xff, 0x8d, 0xd0, 0x92,
    0x58, 0x5a, 0x08, 0x65, 0xd6, 0x5e, 0x7a, 0x99, 0x28, 0x09, 0xf4, 0x45,
    0x8e, 0xf9, 0x9e, 0x62, 0xbd, 0xf7, 0xdd, 0x1a, 0x2e, 0xf1, 0x82, 0x30,
    0x4d, 0x39, 0x08, 0x08, 0x8c, 0x00, 0x2d, 0x34, 0xc4, 0x44, 0xe6, 0x8f,
    0x9e, 0x2e, 0x92, 0xfc, 0x0f, 0x4e, 0xe4, 0x14, 0xbd, 0xe3, 0xcf, 0x98,
    0x8c, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60,
    0x82
  };
  static const unsigned int test_png_image_data_length = 841;

  FILE* fout = fopen(filename_buffer, "wb");

  if ( !fout ){
    return NULL;
  }

  if ( !fwrite(test_png_image_data, test_png_image_data_length, 1, fout) ){
    fclose(fout);
    unlink(filename_buffer);
    return NULL;
  }
  else{
    fclose(fout);
    return filename_buffer;
  }
}
