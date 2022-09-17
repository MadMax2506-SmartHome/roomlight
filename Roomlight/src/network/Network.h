#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <Arduino.h>

#include "../storage/Storage.h"
#include "../led_strip/Ledstrip.h"
#include "../colors/Colors.h"

#include "./wlan/WlanESP.h"
#include "./mqtt/MQTT_ESP.h"
#include "./refresh_over_the_air/OTA_ESP.h"

#include "../Constants.h"
#include "../../Secrets.h"

#define WLAN_MAX_CONNETION_TRYS 10

#define MQTT_MAX_CONNETION_TRYS 10
#define MQTT_RETAINED false

class Network {
private:
  int i_countTopicsToSubscribe;
  int i_countTopicsToPublish;

  char** ppc_topicsToSuscribe;
  char** ppc_topicsToPublish;

  boolean b_isWlanConnected;
  boolean b_isMqttConnected;

  WlanESP* p_wlan;
  WiFiClient p_espClient;
  OTA_ESP* p_ota;
  MQTT_ESP* p_mqtt;
public:
  Network() :
  b_isWlanConnected(false),
  b_isMqttConnected(false)
  {
    p_wlan = new WlanESP(WLAN_SSID, WLAN_PASSWORD, WLAN_MAX_CONNETION_TRYS);
    p_ota = new OTA_ESP();
    p_mqtt = new MQTT_ESP(MQTT_SERVER_IP_ADDRESS, MQTT_SERVER_PORT, p_espClient, MQTT_MAX_CONNETION_TRYS, MQTT_RETAINED);
  }

  void init(boolean, boolean, boolean);
  void initWLAN();
  void initMQTT(boolean, boolean, boolean);
};
#endif //_NETWORK_H_
