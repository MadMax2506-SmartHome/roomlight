#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <Arduino.h>

#include "./mqtt/MQTT_ESP.h"
#include "./refresh_over_the_air/OTA_ESP.h"
#include "./wlan/WlanESP.h"

#include "../../Secrets.h"
#include "../utils/Utils.h"
#include "../device/Device.h"
#include "../mqttCallbackHandler/MqttCallbackHandler.h"

class Network {
private:
  WlanESP* p_wlan;
  OTA_ESP* p_ota;
  MQTT_ESP* p_mqtt;

  void init(Device*, Device*, Device*, void (char*, u_int8_t*, unsigned int));
  void initMQTT(Device*, Device*, Device*, void (char*, u_int8_t*, unsigned int));
  void initSingleMqttDevice(Device*);
public:
  Network(WlanESP* p_wlan, OTA_ESP* p_ota, MQTT_ESP* p_mqtt) :
  p_wlan(p_wlan),
  p_ota(p_ota),
  p_mqtt(p_mqtt)
  {};

  void loop(Device*, Device*, Device*, void (char*, u_int8_t*, unsigned int));
};

#endif //_NETWORK_H_
