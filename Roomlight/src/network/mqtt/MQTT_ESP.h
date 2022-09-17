#ifndef _MQTT_ESP_H_
#define _MQTT_ESP_H_

#include <Arduino.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

class MQTT_ESP {
private:
  int i_port;
  int i_countTopicsToSubscribe;
  int i_maxConnectionTrys;

  boolean b_retained;
  boolean b_isMqttInit;
  boolean b_isMqttAvailable;

  char** ppc_topicsToSubscribe;
  byte* pbyte_ip;

  WiFiClient p_espClient;
  PubSubClient* p_client;
public:
  MQTT_ESP(byte* pbyte_ip, int i_port, WiFiClient p_espClient, int i_maxConnectionTrys = 2, boolean b_retained = false) :
  pbyte_ip(pbyte_ip),
  i_port(i_port),
  p_espClient(p_espClient),
  i_maxConnectionTrys(i_maxConnectionTrys),
  b_retained(b_retained)
  {
    b_isMqttInit      = false;
    b_isMqttAvailable = false;

    i_countTopicsToSubscribe  = 0;
    ppc_topicsToSubscribe     = NULL;
  }

  void setRetained(boolean);

  boolean connect(void callback(char*, byte*, unsigned int));
  boolean connect(char**, int, void callback(char*, byte*, unsigned int));

  boolean reconnect();

  void loop();

  boolean sendMSG(char*, char*);
};
#endif //_MQTT_ESP_H_
