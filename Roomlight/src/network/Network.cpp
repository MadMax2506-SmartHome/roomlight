#include "Network.h"

void Network::init(
  Device* p_keyboardDevice,
  Device* p_bedWallDevice,
  Device* p_bedSideDevice,
  void callback(char*, u_int8_t*, unsigned int)
) {
  //prüfen, ob WLAN verbunden ist
  if(b_isWlanConnected) {
    //WLAN verbunden
    p_ota->handle();

    if(b_isMqttConnected) p_mqtt->loop();
    else initMQTT(p_keyboardDevice, p_bedWallDevice, p_bedSideDevice, callback);
  } else {
    //WLAN nicht verbunden
    initWLAN();

    if(!b_isWlanConnected) return;
    initMQTT(p_keyboardDevice, p_bedWallDevice, p_bedSideDevice, callback);
  }
}

void Network::initWLAN() {
  if(!(b_isWlanConnected = p_wlan->connect())) return;
  p_ota->init(p_wlan->getMac(), OTA_PASSWORD);
}

void Network::initMQTT(
  Device* p_keyboardDevice,
  Device* p_bedWallDevice,
  Device* p_bedSideDevice,
  void callback(char*, u_int8_t*, unsigned int)
) {
  const int i_numberOfTopicsToSuscribe = 4;
  char** ppc_topicsToSuscribe = new char*[i_numberOfTopicsToSuscribe];
  ppc_topicsToSuscribe[0] = stringToChar(String(MQTT_GLOBAL_CONF_TOPIC));
  ppc_topicsToSuscribe[1] = stringToChar(p_keyboardDevice->getConfigTopic());
  ppc_topicsToSuscribe[2] = stringToChar(p_bedWallDevice->getConfigTopic());
  ppc_topicsToSuscribe[3] = stringToChar(p_bedSideDevice->getConfigTopic());

  if(!(b_isMqttConnected = p_mqtt->connect(ppc_topicsToSuscribe, i_numberOfTopicsToSuscribe, callback))) return;
  p_mqtt->loop();

  initSingleMqttDevice(p_keyboardDevice);
  initSingleMqttDevice(p_bedWallDevice);
  initSingleMqttDevice(p_bedSideDevice);
}

void Network::initSingleMqttDevice(Device* p_device) {
  char* pc_topic = stringToChar(p_device ->getStatusTopic());
  p_mqtt->sendMSG(pc_topic, "power-on");

  if(p_device->p_animation->getStatus()) {
     // Strip ist aktiv
     p_mqtt->sendMSG(pc_topic, "active");
  } else {
    // Strip ist im Leerlauf
    p_mqtt->sendMSG(pc_topic, "idle");
  }
}