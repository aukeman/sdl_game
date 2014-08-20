#include <test_utils.h>
#include <control.h>
#include <joystick.h>
#include <linked_list.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern struct linked_list_t keyboard_mappings[512];
extern struct linked_list_t js_axis_mappings[JS__MAX_JOYSTICKS][JS__MAX_AXES];
extern struct linked_list_t js_button_mappings[JS__MAX_JOYSTICKS][JS__MAX_BUTTONS];

const char* write_config_file(const char* contents, ...);

const char* setup(const char* contents, ...);
void teardown(const char* mapping_filename);

enum control_type_e {
  NO_CONTROL_MAPPING,
  ANALOG,
  BINARY
};

struct control_mapping_t{

  enum control_type_e type;

  float min_input;
  float max_input;

  union{
    struct control__analog_t* analog;
    struct control__binary_t* binary;
  };
};

void no_mapping_file(){
  TEST_ASSERT_INT(control__setup(""), CONTROL__MAPPING_FILE_NOT_FOUND);

  int idx1, idx2;
  for ( idx1 = 0; idx1 < 512; ++idx1 ){
    linked_list__empty( &keyboard_mappings[idx1] );
  }

  for ( idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1 ){

    for ( idx2 = 0; idx2 < JS__MAX_AXES; ++idx2 ){
      TEST_ASSERT_TRUE( linked_list__empty( &js_axis_mappings[idx1][idx2] ) );
    }

    for ( idx2 = 0; idx2 < JS__MAX_BUTTONS; ++idx2 ){
      TEST_ASSERT_TRUE( linked_list__empty( &js_button_mappings[idx1][idx2] ) );
    }
  }

  TEST_ASSERT_SUCCESS(control__teardown());
}

void no_mappings(){

  const char* mapping_file = setup("");

  int idx1, idx2;
  for ( idx1 = 0; idx1 < 512; ++idx1 ){
    linked_list__empty( &keyboard_mappings[idx1] );
  }

  for ( idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1 ){

    for ( idx2 = 0; idx2 < JS__MAX_AXES; ++idx2 ){
      TEST_ASSERT_TRUE( linked_list__empty( &js_axis_mappings[idx1][idx2] ) );
    }

    for ( idx2 = 0; idx2 < JS__MAX_BUTTONS; ++idx2 ){
      TEST_ASSERT_TRUE( linked_list__empty( &js_button_mappings[idx1][idx2] ) );
    }
  }

  teardown(mapping_file);
}

void map_up_to_axis(){
  
  const char* mapping_file = setup("0\n"
				   "up joystick 3 axis 4 0.0 1.0");


  int idx1, idx2;
  for ( idx1 = 0; idx1 < 512; ++idx1 ){
    linked_list__empty( &keyboard_mappings[idx1] );
  }

  for ( idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1 ){

    for ( idx2 = 0; idx2 < JS__MAX_AXES; ++idx2 ){
      
      if ( idx1 == 3 && idx2 == 4 ){

	const struct control_mapping_t* mapping = 
	  (const struct control_mapping_t*)
	  linked_list__begin(&js_axis_mappings[idx1][idx2]);

	TEST_ASSERT_NOT_NULL( mapping );

	TEST_ASSERT_INT( mapping->type, ANALOG );
	TEST_ASSERT_FLOAT( mapping->min_input, 0.0f, 0 );
	TEST_ASSERT_FLOAT( mapping->max_input, 1.0f, 0 );
	
	TEST_ASSERT_NOT_NULL( control__get_state(0) );
	TEST_ASSERT_PTR( mapping->analog, &control__get_state(0)->up );

      }
      else{
	TEST_ASSERT_TRUE( linked_list__empty( &js_axis_mappings[idx1][idx2] ) );
      }
    }

    for ( idx2 = 0; idx2 < JS__MAX_BUTTONS; ++idx2 ){
      TEST_ASSERT_TRUE( linked_list__empty( &js_button_mappings[idx1][idx2] ) );
    }
  }

  teardown(mapping_file);
}



TEST_SUITE_START(Control Tests)
 TEST_CASE(no_mapping_file)
 TEST_CASE(no_mappings)
TEST_CASE(map_up_to_axis)
TEST_SUITE_END()

const char* setup(const char* contents, ...){

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

    TEST_ASSERT_SUCCESS(control__setup(filename_buffer));
    return filename_buffer;
  }
}

void teardown(const char* mapping_file){
  TEST_ASSERT_SUCCESS(control__teardown());
  unlink(mapping_file);
}
