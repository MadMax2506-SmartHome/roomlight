#include "Network.h"

void Network::init(boolean b_isKeyboardActive, boolean b_isBedWallActive, boolean b_isBedSideActive) {
  //prüfen, ob WLAN verbunden ist
  if(Network::b_isWlanConnected) {
    //WLAN verbunden
    Network::p_ota->handle();

    //prüfen, ob MQTT verbunden ist
    if(Network::b_isMqttConnected) {
      //MQTT verbunden
      Network::p_mqtt->loop();
    } else {
      //MQTT nicht verbunden
      initMQTT(b_isKeyboardActive, b_isBedWallActive, b_isBedSideActive);
    }
  } else {
    //WLAN nicht verbunden
    initWLAN();

    //prüfen, ob WLAN verbunden
    if(!Network::b_isWlanConnected) {
      return;
    }

    //mqtt
    initMQTT(b_isKeyboardActive, b_isBedWallActive, b_isBedSideActive);
  }
}

void Network::initWLAN() {
  //pürfen, ob wlan verbunden ist
  if(!(Network::b_isWlanConnected = Network::p_wlan->connect())) {
    //WLAN nicht verbunden
    return;
  }

  //ota initalisieren
  Network::p_ota->init(Network::p_wlan->getMac(), OTA_PASSWORD);

  //mqtt
  //subscribs initalisieren
  Network::ppc_topicsToSuscribe = new char*[MQTT_COUNT_TOPICS_TO_SUBSCRIBE];

  Network::ppc_topicsToSuscribe[0] = "devices";

  //global
  char* pc_tmp = new char[255];
  ("conf/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  Network::ppc_topicsToSuscribe[1] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(Network::ppc_topicsToSuscribe[1]) + "/keyboard").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToSuscribe[2] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(Network::ppc_topicsToSuscribe[1]) + "/bed").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToSuscribe[3] = pc_tmp;

  pc_tmp = new char[255];
  (String(Network::ppc_topicsToSuscribe[3]) + "/wall").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToSuscribe[4] = pc_tmp;

  pc_tmp = new char[255];
  (String(Network::ppc_topicsToSuscribe[3]) + "/side").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToSuscribe[5] = pc_tmp;

  //publishes initalisieren
  Network::ppc_topicsToPublish = new char*[MQTT_COUNT_TOPICS_TO_PUBLISH];

  //global
  pc_tmp = new char[255];
  ("status/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  Network::ppc_topicsToPublish[0] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(Network::ppc_topicsToPublish[0]) + "/keyboard").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToPublish[1] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(Network::ppc_topicsToPublish[0]) + "/bed").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToPublish[2] = pc_tmp;

  pc_tmp = new char[255];
  (String(Network::ppc_topicsToPublish[2]) + "/wall").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToPublish[3] = pc_tmp;

  pc_tmp = new char[255];
  (String(Network::ppc_topicsToPublish[2]) + "/side").toCharArray(pc_tmp, 255);
  Network::ppc_topicsToPublish[4] = pc_tmp;
}

void Network::initMQTT(boolean b_isKeyboardActive, boolean b_isBedWallActive, boolean b_isBedSideActive) {
  //prüfen, ob MQTT verbunden
  if(!(Network::b_isMqttConnected = Network::p_mqtt->connect(Network::ppc_topicsToSuscribe, Network::i_countTopicsToSubscribe))) {
    //MQTT nicht verbunden
    return;
  }
  Network::p_mqtt->loop();

  //send msg
  Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[0], "power-on");

  //prüfen, ob der Zustand des keyboardstrips aktiv ist
  if(b_isKeyboardActive) {
     //Strip ist aktiv
     Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[1], "active");
  } else {
    //Strip ist im Leerlauf
    Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[1], "idle");
  }

  //prüfen, ob der Zustand des bedwall strips aktiv ist
  if(b_isBedWallActive) {
    //Strip ist aktiv
    Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[3], "active");
  } else {
    //Strip ist im Leerlauf
    Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[3], "idle");
  }

   //prüfen, ob der Zustand des bedside strips aktiv ist
  if(b_isBedSideActive) {
    //Strip ist aktiv
    Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[4], "active");
  } else {
    //Strip ist im Leerlauf
    Network::p_mqtt->sendMSG(Network::ppc_topicsToPublish[4], "idle");
  }
}