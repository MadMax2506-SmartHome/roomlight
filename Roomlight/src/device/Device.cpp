#include "Device.h"

void Device::animate() { p_animation->animate(); }

String Device::getStatusTopic() {
  return (String(MQTT_STATUS_TOPIC) + "/" + id);
}
String Device::getConfigTopic() {
  return String(MQTT_CONF_TOPIC) + "/" + id;
}

char* Device::getInfo() {
  String str_json;
  DynamicJsonDocument configJSON(1024);

  configJSON["deviceId"] = id;
  configJSON["name"] = DEVICE_NAME;
  serializeJson(configJSON, str_json);

  Serial.println(stringToChar(str_json));
  return stringToChar(str_json);
}

char* Device::getConfiguration() {
  Serial.println(p_animation->getConfAsJSON());
  return p_animation->getConfAsJSON();
}

char* Device::getData() {
  String str_json;
  DynamicJsonDocument configJSON(1024);

  configJSON["info"] = getData();
  configJSON["config"] = getConfiguration();

  Serial.println(stringToChar(str_json));
  return stringToChar(str_json);
}