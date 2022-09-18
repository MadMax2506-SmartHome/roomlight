#ifndef _OTA_ESP_H_
#define _OTA_ESP_H_

#include <Arduino.h>
#include <ArduinoOTA.h>

class OTA_ESP {
public:
  OTA_ESP();
  OTA_ESP(int i_port);

  void init();
  void init(char*);
  void init(char*, char*);
  
  void handle();
};
#endif //_OTA_ESP_H_
