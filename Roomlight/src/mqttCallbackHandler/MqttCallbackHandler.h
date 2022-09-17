#ifndef _MQTT_CALLBACK_HANDLER_H_
#define _MQTT_CALLBACK_HANDLER_H_

#include <Arduino.h>

#include "../Constants.h"

class MqttCallbackHandler {
private:
  MQTT_ESP* p_mqtt;
  Colors* p_color;
  Animation* p_keyboardAnimation;
  Animation* p_bedWallAnimation;
  Animation* p_bedSideAnimation;

  void handleMqttPayload(String, String, String);
  void handleAnimation(Animation*, char*, String, String);
public:
  MqttCallbackHandler(
    MQTT_ESP* p_mqtt,
    Colors* p_color,
    Animation* p_keyboardAnimation,
    Animation* p_bedWallAnimation,
    Animation* p_bedSideAnimation
  ) :
  p_mqtt(p_mqtt),
  p_color(p_color),
  p_keyboardAnimation(p_keyboardAnimation),
  p_bedWallAnimation(p_bedWallAnimation),
  p_bedSideAnimation(p_bedSideAnimation)
  {};

  void onMqttPayload(char*, u_int8_t*, unsigned int);
};
#endif //_MQTT_CALLBACK_HANDLER_H_
