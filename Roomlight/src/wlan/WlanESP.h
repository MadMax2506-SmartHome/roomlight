#ifndef _WLANESP_H_
#define _WLANESP_H_

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

class WlanESP {
private:
  int i_maxTrys;
  boolean b_isAvailable;

  char* pc_ssid;
  char* pc_password;

  ESP8266WiFiMulti p_WiFiMulti;
public:
  WlanESP(char* pc_ssid, char* pc_password, int i_maxTrys=10):
  i_maxTrys(i_maxTrys),
  pc_ssid(pc_ssid),
  pc_password(pc_password)
  {
    pinMode(LED_BUILTIN, OUTPUT);
  }

  boolean connect();

  char* getMac();
  char* getIp();
};
#endif //_WLANESP_H_
