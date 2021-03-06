#include <test_utils.h>
#include <control.h>
#include <joystick.h>
#include <events.h>
#include <linked_list.h>
#include <constants.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

extern struct linked_list_t keyboard_mappings[512];
extern struct linked_list_t js_axis_mappings[JS__MAX_JOYSTICKS][JS__MAX_AXES];
extern struct linked_list_t js_button_mappings[JS__MAX_JOYSTICKS][JS__MAX_BUTTONS];

const char* write_config_file(const char* contents, ...);

const char* setup(int* setup_result, const char* contents, ...);
void teardown(const char* mapping_filename);

void no_mapping_file(){

  int idx1, idx2;

  TEST_ASSERT_INT(control__setup(""), CONTROL__MAPPING_FILE_NOT_FOUND);

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

  int setup_result, idx1, idx2;
  const char* mapping_file = setup(&setup_result, "");
  TEST_ASSERT_INT(setup_result, CONTROL__NO_DEVICES_TO_MAP);

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

void map_directions_to_axes(){
  
  int setup_result, idx1, idx2;
  const char* mapping_file = setup(&setup_result, 
				   "0\n"
				   "up joystick 3 axis 4 0.0 1.0\n"
				   "down joystick 3 axis 4 0.0 -1.0\n"
				   "left joystick 3 axis 5 0.0 1.0\n"
				   "right joystick 3 axis 5 0.0 -1.0\n");
  TEST_ASSERT_SUCCESS(setup_result);

  for ( idx1 = 0; idx1 < 512; ++idx1 ){
    linked_list__empty( &keyboard_mappings[idx1] );
  }

  for ( idx1 = 0; idx1 < JS__MAX_JOYSTICKS; ++idx1 ){

    for ( idx2 = 0; idx2 < JS__MAX_AXES; ++idx2 ){
      
      if ( idx1 == 3 && idx2 == 4 ){

	struct linked_list__node_t* iter;
	const struct control_mapping_t* mapping = 
	  (const struct control_mapping_t*)
	  linked_list__begin(&js_axis_mappings[idx1][idx2], &iter);

	TEST_ASSERT_NOT_NULL( mapping );

	TEST_ASSERT_INT( mapping->type, ANALOG );
	TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
	TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
	TEST_ASSERT_NOT_NULL( control__get_state(0) );
	TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->up );

	mapping = 
	  (const struct control_mapping_t*)linked_list__next(&iter);

	TEST_ASSERT_NOT_NULL( mapping );

	TEST_ASSERT_INT( mapping->type, ANALOG );
	TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
	TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
	TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->down );

	mapping = 
	  (const struct control_mapping_t*)linked_list__next(&iter);

	TEST_ASSERT_NULL( mapping );
      }
      else if ( idx1 == 3 && idx2 == 5 ){

	struct linked_list__node_t* iter;
	const struct control_mapping_t* mapping = 
	  (const struct control_mapping_t*)
	  linked_list__begin(&js_axis_mappings[idx1][idx2], &iter);

	TEST_ASSERT_NOT_NULL( mapping );

	TEST_ASSERT_INT( mapping->type, ANALOG );
	TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
	TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
	TEST_ASSERT_NOT_NULL( control__get_state(0) );
	TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->left );

	mapping = 
	  (const struct control_mapping_t*)linked_list__next(&iter);

	TEST_ASSERT_NOT_NULL( mapping );

	TEST_ASSERT_INT( mapping->type, ANALOG );
	TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
	TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
	TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->right );

	mapping = 
	  (const struct control_mapping_t*)linked_list__next(&iter);

	TEST_ASSERT_NULL( mapping );
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


void map_directions_to_buttons(){
  
  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "1\n"
				   "up joystick 4 button 10\n"
				   "down joystick 4 button 11\n"
				   "left joystick 4 button 12\n"
				   "right joystick 4 button 13\n");

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[4][10], &iter);


  TEST_ASSERT_SUCCESS(setup_result);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(1) );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->up );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[4][11], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->down );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[4][12], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->left );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[4][13], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->right );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void map_directions_to_keys(){
  
  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "1\n"
				   "up keyboard 273\n"
				   "down keyboard 274\n"
				   "left keyboard 276\n"
				   "right keyboard 275\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_UP], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(1) );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->up );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_DOWN], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->down );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_LEFT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->left );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_RIGHT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->right );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void map_jump_and_fire_to_axes(){
  
  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "2\n"
				   "jump joystick 5 axis 0 -1.0 1.0\n"
				   "fire joystick 5 axis 1 -1.0 1.0\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[5][0], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, -1.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(2) );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[5][1], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, -1.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}


void map_jump_and_fire_to_buttons(){
  
  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "2\n"
				   "jump joystick 5 button 3\n"
				   "fire joystick 5 button 10\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[5][3], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(2) );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[5][10], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void map_jump_and_fire_to_keys(){
  
  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "2\n"
				   "jump keyboard 306\n"
				   "fire keyboard 32\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[306], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(2) );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[32], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(2)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void map_to_both_joystick_and_keyboard(){

  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "0\n"
				   "up    joystick 0 axis    1  0.0  1.0\n"
				   "down  joystick 0 axis    1  0.0 -1.0\n"
				   "left  joystick 0 axis    0  0.0  1.0\n"
				   "right joystick 0 axis    0  0.0 -1.0\n"
				   "jump  joystick 0 button  0\n"
				   "fire  joystick 0 button  1\n"
				   "up    keyboard           273\n"
				   "down  keyboard           274\n"
				   "left  keyboard           276\n"
				   "right keyboard           275\n"
				   "jump  keyboard           306\n"
				   "fire  keyboard           32\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[0][1], &iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
  TEST_ASSERT_NOT_NULL( control__get_state(0) );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->up );

  mapping = (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->down );

  mapping = (const struct control_mapping_t*)linked_list__next(&iter);
  
  TEST_ASSERT_NULL( mapping );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[0][0], &iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->left );

  mapping = 
    (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->right );

  mapping = 
    (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NULL( mapping );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[0][0], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[0][1], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_UP], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->up );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_DOWN], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->down );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_LEFT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->left );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_RIGHT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->right );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[306], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[32], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void map_player_1_to_joystick_and_player_2_to_keyboard(){

  int setup_result = 0;
  struct linked_list__node_t* iter = NULL;
  const struct control_mapping_t* mapping = NULL;
  const char* mapping_file = setup(&setup_result, 
				   "0\n"
				   "up    joystick 0 axis    1  0.0  1.0\n"
				   "down  joystick 0 axis    1  0.0 -1.0\n"
				   "left  joystick 0 axis    0  0.0  1.0\n"
				   "right joystick 0 axis    0  0.0 -1.0\n"
				   "jump  joystick 0 button  0\n"
				   "fire  joystick 0 button  1\n"
				   "1\n"
				   "up    keyboard           273\n"
				   "down  keyboard           274\n"
				   "left  keyboard           276\n"
				   "right keyboard           275\n"
				   "jump  keyboard           306\n"
				   "fire  keyboard           32\n");
  TEST_ASSERT_SUCCESS(setup_result);

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[0][1], &iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
  TEST_ASSERT_NOT_NULL( control__get_state(0) );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->up );

  mapping = (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->down );

  mapping = (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NULL( mapping );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_axis_mappings[0][0], &iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->left );

  mapping = 
    (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NOT_NULL( mapping );

  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, -1.0f );
	
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(0)->right );

  mapping = 
    (const struct control_mapping_t*)linked_list__next(&iter);

  TEST_ASSERT_NULL( mapping );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[0][0], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&js_button_mappings[0][1], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(0)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_UP], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_NOT_NULL( control__get_state(1) );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->up );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_DOWN], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->down );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_LEFT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->left );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[EVENTS__KEY_RIGHT], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, ANALOG );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.analog, &control__get_state(1)->right );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[306], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(1)->jump );

  TEST_ASSERT_NULL( linked_list__next(&iter) );


  mapping = 
    (const struct control_mapping_t*)
    linked_list__begin(&keyboard_mappings[32], &iter);

  TEST_ASSERT_NOT_NULL( mapping );
  TEST_ASSERT_INT( mapping->type, BINARY );
  TEST_ASSERT_FLOAT( mapping->min_input, 0.0f );
  TEST_ASSERT_FLOAT( mapping->max_input, 0.0f );
  TEST_ASSERT_PTR( mapping->control_type.binary, &control__get_state(1)->fire );

  TEST_ASSERT_NULL( linked_list__next(&iter) );

  teardown(mapping_file);
}

void malformed_control(){
  
  int setup_result;
  const char* mapping_file = setup(&setup_result, 
				   "0\n"
				   "up\n");
  TEST_ASSERT_INT(setup_result, CONTROL__BAD_MAPPING_FILE);

  unlink(mapping_file);
}

void buffer_overflow(){
  
  int setup_result;
  const char* mapping_file = setup(&setup_result, 
				   "0\n"
				   "upppppppppppppppx pppppppppppppppppx\n");
  TEST_ASSERT_INT(setup_result, CONTROL__BAD_MAPPING_FILE);

  unlink(mapping_file);
}


TEST_SUITE_START(Control Tests)
 TEST_CASE(no_mapping_file)
 TEST_CASE(no_mappings)
 TEST_CASE(map_directions_to_axes)
 TEST_CASE(map_directions_to_buttons)
 TEST_CASE(map_directions_to_keys)
 TEST_CASE(map_jump_and_fire_to_axes)
 TEST_CASE(map_jump_and_fire_to_buttons)
 TEST_CASE(map_jump_and_fire_to_keys)
 TEST_CASE(map_to_both_joystick_and_keyboard)
 TEST_CASE(map_player_1_to_joystick_and_player_2_to_keyboard)
 TEST_CASE(malformed_control)
 TEST_CASE(buffer_overflow)
TEST_SUITE_END()

const char* setup(int* setup_result, const char* contents, ...){

  FILE* fout = NULL;
  size_t len = 0, characters_written = 0;
  static char filename_buffer[1024];
  va_list ap;

  sprintf( filename_buffer, "%s", tmpnam(NULL) );
  fout = fopen(filename_buffer, "w");
  len = strlen(contents);
  
  *setup_result = UNKNOWN_FAILURE;

  if ( !fout ){
    return NULL;
  }

  va_start(ap, contents);
  characters_written = vfprintf(fout, contents, ap);
  va_end(ap);

  fclose(fout);

  if (characters_written < len){
    unlink(filename_buffer);
    return NULL;
  }
  else{

    *setup_result = control__setup(filename_buffer);
    return filename_buffer;
  }
}

void teardown(const char* mapping_file){
  TEST_ASSERT_SUCCESS(control__teardown());
  unlink(mapping_file);
}
