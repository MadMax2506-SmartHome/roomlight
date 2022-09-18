#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <Arduino.h>

#include "../../Secrets.h"
#include "../utils/Utils.h"
#include "./colors/Colors.h"
#include "./animation/Animation.h"
#include "./led_strip/Ledstrip.h"

class Device {
private:
  String id;
public:
  Ledstrip* p_strip;
  Animation* p_animation;

  Device(
    String id,
    int i_pin,
    int i_count_leds,
    int i_crc_storage_index,
    int i_start_storage_index,
    int i_end_storage_index,
    Colors* p_color
  ) {
    Device::id = id;
    // TODO: remove 250 an use it as constant
    p_strip = new Ledstrip(i_pin, i_count_leds, 250, MODUS_RGB);
    p_animation = new Animation(p_strip, p_color, i_crc_storage_index, i_start_storage_index, i_end_storage_index);
  }

  void animate();

  String getStatusTopic();
  String getConfigTopic();

  char* getInfo();
  char* getConfiguration();
  char* getData();
};

#endif //_DEVICE_H_
