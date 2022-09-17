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
  MQTT_ESP(
    byte* pbyte_ip,
    int i_port,
    WiFiClient p_espClient,
    int i_maxConnectionTrys = 2,
    boolean b_retained = false
  ) :
  i_port(i_port),
  i_countTopicsToSubscribe(0),
  i_maxConnectionTrys(i_maxConnectionTrys),
  b_retained(b_retained),
  b_isMqttInit(false),
  b_isMqttAvailable(false),
  ppc_topicsToSubscribe(NULL),
  pbyte_ip(pbyte_ip),
  p_espClient(p_espClient)
  {}

  void setRetained(boolean);

  boolean connect(MQTT_CALLBACK_SIGNATURE);
  boolean connect(char**, int, MQTT_CALLBACK_SIGNATURE);
  boolean reconnect();

  void loop();

  boolean sendMSG(char*, char*);
};
#endif //_MQTT_ESP_H_
