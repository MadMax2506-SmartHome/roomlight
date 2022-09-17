#ifndef _NETWORK_H_
#define _NETWORK_H_

#include "./mqtt/MQTT_ESP.h"
#include "./refresh_over_the_air/OTA_ESP.h"
#include "./wlan/WlanESP.h"

#include "../../Secrets.h"
#include "../animation/Animation.h"
#include "../colors/Colors.h"
#include "../mqttCallbackHandler/MqttCallbackHandler.h"

class Network {
private:
  boolean b_isWlanConnected;
  boolean b_isMqttConnected;

  WlanESP* p_wlan;
  OTA_ESP* p_ota;
  MQTT_ESP* p_mqtt;

  void initWLAN();
  void initMQTT(Colors*, Animation*, Animation*, Animation*);
  void initSingleMqttDevice(Animation*, char*);
public:
  Network(WlanESP* p_wlan, OTA_ESP* p_ota, MQTT_ESP* p_mqtt) :
  b_isWlanConnected(false),
  b_isMqttConnected(false),
  p_wlan(p_wlan),
  p_ota(p_ota),
  p_mqtt(p_mqtt)
  {};

  void init(Colors*, Animation*, Animation*, Animation*);
};

#endif //_NETWORK_H_
