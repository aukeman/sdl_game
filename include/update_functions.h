#ifndef UPDATE_FUNCTIONS_H
#define UPDATE_FUNCTIONS_H

void update__apply_gravity( milliseconds_t length_of_frame,
			    const geo__rect_t* bounding_box,
			    const geo__point_t* position,
			    geo__vector_t* velocity,
			    void* context ){

  static const float g = 100.0f;

  velocity->y += (int)((g*length_of_frame)*0.001f);
}

void update__apply_drag(  milliseconds_t length_of_frame,
			  const geo__rect_t* bounding_box,
			  const geo__point_t* position,
			  geo__vector_t* velocity,
			  void* context ){

  static const float drag_const = 0.5f;

  velocity->x *= 1 - drag_const;
}

#endif
