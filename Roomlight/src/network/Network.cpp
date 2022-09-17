#include "Network.h"

void Network::init(Colors* p_color, Device* p_keyboardDevice, Device* p_bedWallDevice, Device* p_bedSideDevice) {
  //prüfen, ob WLAN verbunden ist
  if(b_isWlanConnected) {
    //WLAN verbunden
    p_ota->handle();

    if(b_isMqttConnected) p_mqtt->loop();
    else initMQTT(p_color, p_keyboardDevice, p_bedWallDevice, p_bedSideDevice);
  } else {
    //WLAN nicht verbunden
    initWLAN();

    if(!b_isWlanConnected) return;
    initMQTT(p_color, p_keyboardDevice, p_bedWallDevice, p_bedSideDevice);
  }
}

void Network::initWLAN() {
  if(!(b_isWlanConnected = p_wlan->connect())) return;
  p_ota->init(p_wlan->getMac(), OTA_PASSWORD);
}

void Network::initMQTT(Colors* p_color, Device* p_keyboardDevice, Device* p_bedWallDevice, Device* p_bedSideDevice) {
  MqttCallbackHandler* mqttCallbackHandler = new MqttCallbackHandler(
    p_mqtt,
    p_color,
    p_keyboardDevice,
    p_bedWallDevice,
    p_bedSideDevice
  );

  const int i_numberOfTopicsToSuscribe = 4;
  char** ppc_topicsToSuscribe = new char*[i_numberOfTopicsToSuscribe];
  ppc_topicsToSuscribe[0] = (char *) String(MQTT_GLOBAL_CONF_TOPIC).c_str();
  ppc_topicsToSuscribe[1] = p_keyboardDevice->getConfigTopicAsPointer();
  ppc_topicsToSuscribe[2] = p_bedWallDevice->getConfigTopicAsPointer();
  ppc_topicsToSuscribe[3] = p_bedSideDevice->getConfigTopicAsPointer();

  b_isMqttConnected = p_mqtt->connect(
    ppc_topicsToSuscribe,
    i_numberOfTopicsToSuscribe,  
    [mqttCallbackHandler](char* pc_topic, u_int8_t* pi_payload, unsigned int i_length){
      return mqttCallbackHandler->onMqttPayload(pc_topic, pi_payload, i_length);
    }
  );
  if(!b_isMqttConnected) return;
  p_mqtt->loop();

  initSingleMqttDevice(p_keyboardDevice, "TODO");
  initSingleMqttDevice(p_bedWallDevice, "TODO");
  initSingleMqttDevice(p_bedSideDevice, "TODO");
}

void Network::initSingleMqttDevice(Device* p_device, char* pc_topic) {
  p_mqtt->sendMSG(pc_topic, "power-on");

  if(p_device->p_animation->getStatus()) {
     // Strip ist aktiv
     p_mqtt->sendMSG(pc_topic, "active");
  } else {
    // Strip ist im Leerlauf
    p_mqtt->sendMSG(pc_topic, "idle");
  }
}