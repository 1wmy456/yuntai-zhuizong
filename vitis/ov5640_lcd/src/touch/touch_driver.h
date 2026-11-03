#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H
#include "touch_events.h"

int touch_init(unsigned lcd_id,unsigned width,unsigned height);
void touch_service(uint64_t now);
typedef void (*TouchHandler)(unsigned event,unsigned x,unsigned y,uint64_t now);
void touch_set_handler(TouchHandler handler);
void touch_capture_control(unsigned flags);
void touch_display_state(unsigned state);
#endif
