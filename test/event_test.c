#include <events.h>
#include <test_utils.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdarg.h>
#include <geometry.h>

int setup(){
  
  events__setup();

  return SUCCESS;
}

int teardown(){

  events__teardown();

  return SUCCESS;
}

void cb_increment(events__type_e event, const events__event_parameter_t* event_parameter, void* context){
  *((int*)context) += 1;
}

void cb_decrement(events__type_e event, const events__event_parameter_t* event_parameter, void* context){
  *((int*)context) -= 1;
}

void cb_double(events__type_e event, const events__event_parameter_t* event_parameter, void* context){
  *((int*)context) *= 2;
}

void cb_halve(events__type_e event, const events__event_parameter_t* event_parameter, void* context){
  *((int*)context) /= 2;
}

void no_registered_callbacks(){

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_NONE, NULL), 
		   EVENTS__INVALID_EVENT_TYPE );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_QUIT, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_KEYDOWN, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_KEYUP, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_JOYSTICK_AXIS, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_JOYSTICK_BUTTON, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_LAST, NULL), 
		   EVENTS__INVALID_EVENT_TYPE );
}

void register_invoke_remove_callbacks(){

  int ctx_1 = 1;
  int ctx_2 = 2;
  int ctx_3 = 3;
  int ctx_4 = 4;
  int ctx_5 = 5;
  int ctx_6 = 6;
  int ctx_7 = 7;

  /* REGISTER */
  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_NONE, cb_double, &ctx_1), 
		  EVENTS__INVALID_EVENT_TYPE );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_QUIT, cb_double, &ctx_2), 
		  SUCCESS );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_KEYDOWN, cb_double, &ctx_3), 
		  SUCCESS );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_KEYUP, cb_double, &ctx_4), 
		  SUCCESS );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_JOYSTICK_AXIS, cb_double, &ctx_5), 
		  SUCCESS );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_JOYSTICK_BUTTON, cb_double, &ctx_6), 
		  SUCCESS );

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_LAST, cb_double, &ctx_7), 
		  EVENTS__INVALID_EVENT_TYPE );

  /* INVOKE */
  TEST_ASSERT_INT(_invoke_callback(EVENTS__TYPE_QUIT, NULL), SUCCESS); 
  TEST_ASSERT_INT(_invoke_callback(EVENTS__TYPE_KEYDOWN, NULL), SUCCESS); 
  TEST_ASSERT_INT(_invoke_callback(EVENTS__TYPE_KEYUP, NULL), SUCCESS); 
  TEST_ASSERT_INT(_invoke_callback(EVENTS__TYPE_JOYSTICK_AXIS, NULL), SUCCESS); 
  TEST_ASSERT_INT(_invoke_callback(EVENTS__TYPE_JOYSTICK_BUTTON, NULL), SUCCESS); 

  TEST_ASSERT_INT(ctx_1, 1);
  TEST_ASSERT_INT(ctx_2, 4);
  TEST_ASSERT_INT(ctx_3, 6);
  TEST_ASSERT_INT(ctx_4, 8);
  TEST_ASSERT_INT(ctx_5, 10);
  TEST_ASSERT_INT(ctx_6, 12);
  TEST_ASSERT_INT(ctx_7, 7);

  /* REMOVE */
  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_NONE, cb_double), 
		  EVENTS__INVALID_EVENT_TYPE );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_QUIT, cb_double), 
		  SUCCESS );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_KEYDOWN, cb_double), 
		  SUCCESS );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_KEYUP, cb_double), 
		  SUCCESS );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_JOYSTICK_AXIS, cb_double), 
		  SUCCESS );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_JOYSTICK_BUTTON, cb_double), 
		  SUCCESS );

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_LAST, cb_double), 
		  EVENTS__INVALID_EVENT_TYPE );

  
  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_NONE, NULL), 
		   EVENTS__INVALID_EVENT_TYPE );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_QUIT, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_KEYDOWN, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_KEYUP, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_JOYSTICK_AXIS, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_JOYSTICK_BUTTON, NULL), 
		   EVENTS__CALLBACK_NOT_REGISTERED );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_LAST, NULL), 
		   EVENTS__INVALID_EVENT_TYPE );
  

  TEST_ASSERT_INT(ctx_1, 1);
  TEST_ASSERT_INT(ctx_2, 4);
  TEST_ASSERT_INT(ctx_3, 6);
  TEST_ASSERT_INT(ctx_4, 8);
  TEST_ASSERT_INT(ctx_5, 10);
  TEST_ASSERT_INT(ctx_6, 12);
  TEST_ASSERT_INT(ctx_7, 7);

}

void deregister_head_callbacks(){
  
  int ctx_1 = 1;
  int ctx_2 = 2;

  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_QUIT, cb_double, &ctx_1), 
		  SUCCESS );
  TEST_ASSERT_INT(events__add_callback(EVENTS__TYPE_QUIT, cb_increment, &ctx_2), 
		  SUCCESS );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_QUIT, NULL), 
		   SUCCESS );

  TEST_ASSERT_INT(ctx_1, 2);
  TEST_ASSERT_INT(ctx_2, 3);

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_QUIT, cb_double), 
		  SUCCESS );
  
  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_QUIT, NULL), 
		   SUCCESS );

  TEST_ASSERT_INT(ctx_1, 2);
  TEST_ASSERT_INT(ctx_2, 4);

  TEST_ASSERT_INT(events__remove_callback(EVENTS__TYPE_QUIT, cb_increment), 
		  SUCCESS );

  TEST_ASSERT_INT( _invoke_callback(EVENTS__TYPE_QUIT, NULL), 
		   SUCCESS );

  TEST_ASSERT_INT(ctx_1, 2);
  TEST_ASSERT_INT(ctx_2, 4);


}


TEST_SUITE_WITH_SETUP_START(Event Tests, setup, teardown)
  TEST_CASE(no_registered_callbacks)
  TEST_CASE(register_invoke_remove_callbacks)
  TEST_CASE(deregister_head_callbacks)
TEST_SUITE_END()

