#ifndef _MQTT_CONFIGURATION_HANDLER_H_
#define _MQTT_CONFIGURATION_HANDLER_H_

#include <Arduino.h>

#include "../Constants.h"
#include "../network/mqtt/MQTT_ESP.h"

class MqttConfigurationHandler {
private:
  char** ppc_topicsToSuscribe;
  char** ppc_topicsToPublish;

  Colors* p_color;
  Animation* p_keyboardAnimation;
  Animation* p_bedWallAnimation;
  Animation* p_bedSideAnimation;
  MQTT_ESP* p_mqtt;
public:
  MqttConfigurationHandler(
    Colors* p_color,
    Animation* p_keyboardAnimation,
    Animation* p_bedWallAnimation,
    Animation* p_bedSideAnimation,
    MQTT_ESP* p_mqtt
  ) :
  p_color(p_color),
  p_keyboardAnimation(p_keyboardAnimation),
  p_bedWallAnimation(p_bedWallAnimation),
  p_bedSideAnimation(p_bedSideAnimation),
  p_mqtt(p_mqtt)
  {};

  MQTT_CALLBACK_SIGNATURE;

  char** MqttConfigurationHandler::getConfAsJSON();
  char* MqttConfigurationHandler::getDeviceInfoAsJSON();
};
#endif //_MQTT_CONFIGURATION_HANDLER_H_
