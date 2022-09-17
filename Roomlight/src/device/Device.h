#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "../../Secrets.h"
#include "./colors/Colors.h"
#include "./animation/Animation.h"
#include "./led_strip/Ledstrip.h"

class Device {
private:
public:
  Ledstrip* p_strip;
  Animation* p_animation;

  Device(String, int, int, int, int, int, Colors*);

  void animate();
};

#endif //_DEVICE_H_
