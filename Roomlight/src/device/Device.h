#ifndef _DEVICE_H_
#define _DEVICE_H_

#include "../../Secrets.h"
#include "../Constants.h"
#include "./colors/Colors.h"
#include "./animation/Animation.h"
#include "./led_strip/Ledstrip.h"

class Device {
private:
  String id;
public:
  Ledstrip* p_strip;
  Animation* p_animation;

  Device(String, int, int, int, int, int, Colors*);

  void animate();

  String getStatusTopic();
  char* getStatusTopicAsPointer();

  String getConfigTopic();
  char* getConfigTopicAsPointer();

  char* Device::getInfo();
  char* Device::getConfiguration();
  char* Device::getData();
};

#endif //_DEVICE_H_
