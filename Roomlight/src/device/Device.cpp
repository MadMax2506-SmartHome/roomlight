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
  str_json+= "{";
  str_json+= "\"deviceId\": \"" + id + "\",";
  str_json+= "\"name\": \"" + String(DEVICE_NAME) + "\"";
  str_json+= "}";

  return stringToChar(str_json);
}

char* Device::getConfiguration() {
  return p_animation->getConfAsJSON();
}

char* Device::getData() {
  String str_json;
  str_json+= "{";
  str_json+= "\"info\": " + String(getInfo()) + ",";
  str_json+= "\"config\": " + String(getConfiguration());
  str_json+= "}";

  return stringToChar(str_json);
}