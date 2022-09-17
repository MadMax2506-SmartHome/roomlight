#include "Network.h"

void Network::init(Colors* p_color, Animation* p_keyboardAnimation, Animation* p_bedWallAnimation, Animation* p_bedSideAnimation) {
  //prüfen, ob WLAN verbunden ist
  if(b_isWlanConnected) {
    //WLAN verbunden
    p_ota->handle();

    if(b_isMqttConnected) p_mqtt->loop();
    else initMQTT(p_color, p_keyboardAnimation, p_bedWallAnimation, p_bedSideAnimation);
  } else {
    //WLAN nicht verbunden
    initWLAN();

    if(!b_isWlanConnected) return;
    initMQTT(p_color, p_keyboardAnimation, p_bedWallAnimation, p_bedSideAnimation);
  }
}

void Network::initWLAN() {
  if(!(b_isWlanConnected = p_wlan->connect())) return;
  p_ota->init(p_wlan->getMac(), OTA_PASSWORD);
}

void Network::initMQTT(Colors* p_color, Animation* p_keyboardAnimation, Animation* p_bedWallAnimation, Animation* p_bedSideAnimation) {
  MqttCallbackHandler* mqttCallbackHandler = new MqttCallbackHandler(
    p_mqtt,
    p_color,
    p_keyboardAnimation,
    p_bedWallAnimation,
    p_bedSideAnimation
  );

  char** ppc_topicsToSuscribe = new char*[2];
  b_isMqttConnected = p_mqtt->connect(
    ppc_topicsToSuscribe,
    MQTT_COUNT_TOPICS_TO_SUBSCRIBE,  
    [mqttCallbackHandler](char* pc_topic, u_int8_t* pi_payload, unsigned int i_length){
      return mqttCallbackHandler->onMqttPayload(pc_topic, pi_payload, i_length);
    }
  );
  if(!b_isMqttConnected) return;
  p_mqtt->loop();

  initSingleMqttDevice(p_keyboardAnimation, "TODO");
  initSingleMqttDevice(p_bedWallAnimation, "TODO");
  initSingleMqttDevice(p_bedSideAnimation, "TODO");
}

void Network::initSingleMqttDevice(Animation* animation, char* topic) {
  p_mqtt->sendMSG(topic, "power-on");

  if(animation->getStatus()) {
     // Strip ist aktiv
     p_mqtt->sendMSG(topic, "active");
  } else {
    // Strip ist im Leerlauf
    p_mqtt->sendMSG(topic, "idle");
  }
}