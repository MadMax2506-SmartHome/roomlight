void initNetwork() {
  //prüfen, ob WLAN verbunden ist
  if(b_isWlanConnected) {
    //WLAN verbunden
    p_ota->handle();

    if(b_isMqttConnected) p_mqtt->loop();
    else initMQTT();
  } else {
    //WLAN nicht verbunden
    initWLAN();

    if(!b_isWlanConnected) return;
    initMQTT();
  }
}

void initWLAN() {
  if(!(b_isWlanConnected = p_wlan->connect())) return;
  p_ota->init(p_wlan->getMac(), OTA_PASSWORD);
}

void initMQTT() {
  b_isMqttConnected = p_mqtt->connect(ppc_topicsToSuscribe, MQTT_COUNT_TOPICS_TO_SUBSCRIBE, onMqttPayload);
  if(!b_isMqttConnected) return;
  p_mqtt->loop();

  initSingleMqttDevice(p_keyboardAnimation, "TODO");
  initSingleMqttDevice(p_bedWallAnimation, "TODO");
  initSingleMqttDevice(p_bedSideAnimation, "TODO");
}

void initSingleMqttDevice(Animation* animation, char* topic) {
  p_mqtt->sendMSG(topic, "power-on");

  if(animation->getStatus()) {
     // Strip ist aktiv
     p_mqtt->sendMSG(topic, "active");
  } else {
    // Strip ist im Leerlauf
    p_mqtt->sendMSG(topic, "idle");
  }
}