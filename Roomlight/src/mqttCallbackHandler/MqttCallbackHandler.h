#ifndef _MQTT_CALLBACK_HANDLER_H_
#define _MQTT_CALLBACK_HANDLER_H_

#include <Arduino.h>

#include "../utils/Utils.h"
#include "../network/Network.h"
#include "../device/Device.h"

class MqttCallbackHandler {
private:
  MQTT_ESP* p_mqtt;
  Colors* p_color;
  Device* p_keyboardDevice;
  Device* p_bedWallDevice;
  Device* p_bedSideDevice;

  void handleMqttPayload(String, String, String);
  void listDevice(Device*);
  void handleDeviceConfigurations(Device*, String, String);
public:
  MqttCallbackHandler(
    MQTT_ESP* p_mqtt,
    Colors* p_color,
    Device* p_keyboardDevice,
    Device* p_bedWallDevice,
    Device* p_bedSideDevice
  ) :
  p_mqtt(p_mqtt),
  p_color(p_color),
  p_keyboardDevice(p_keyboardDevice),
  p_bedWallDevice(p_bedWallDevice),
  p_bedSideDevice(p_bedSideDevice)
  {};

  void onMqttPayload(char*, u_int8_t*, unsigned int);
};
#endif //_MQTT_CALLBACK_HANDLER_H_
