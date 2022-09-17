#include "Device.h"

Device::Device(
  String id,
  int i_pin,
  int i_count_leds,
  int i_crc_storage_index,
  int i_start_storage_index,
  int i_end_storage_index,
  Colors* p_color
) {
  // TODO: remove 250 an use it as constant
  p_strip = new Ledstrip(i_pin, i_count_leds, 250, MODUS_RGB);
  p_animation = new Animation(p_strip, p_color, i_crc_storage_index, i_start_storage_index, i_end_storage_index);
}

void Device::animate() {
  p_animation->animate();
}