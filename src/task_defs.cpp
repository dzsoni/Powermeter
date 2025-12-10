#include "task_defs.h"

extern struct_hardwares* sh;


uint8_t brightness=RGB_BRIGHTNESS/8 ;

void t1Cb_secTimer()
{
  (brightness<RGB_BRIGHTNESS/16)?brightness++:brightness=0; //increase brightness
  neopixelWrite(RGB_BUILTIN,0,brightness,0); // Green
}

