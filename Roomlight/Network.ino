void initNetwork() {
  //prüfen, ob WLAN verbunden ist
  if(wlan.b_isConnected) {
    //WLAN verbunden
    ota->handle();

    //prüfen, ob MQTT verbunden ist
    if(mqtt.b_isConnected) {
      //MQTT verbunden
      mqtt.p_connection->loop();
    } else {
      //MQTT nicht verbunden
      initMQTT();
    }
  } else {
    //WLAN nicht verbunden
    initWLAN();

    //prüfen, ob WLAN verbunden
    if(!wlan.b_isConnected) {
      //WLAN nicht verbunden
      return;
    }

    //mqtt
    initMQTT();
  }
}

void initWLAN() {
  //pürfen, ob wlan verbunden ist
  if(!(wlan.b_isConnected = wlan.p_connection->connect())) {
    //WLAN nicht verbunden
    return;
  }

  wlan.pc_mac = wlan.p_connection->getMac();

  //ota initalisieren
  ota->init(wlan.pc_mac, OTA_PASSWORD);

  //mqtt
  //subscribs initalisieren
  mqtt.i_countTopicsToSubscribe = 6;
  mqtt.ppc_topicsToSuscribe = new char*[mqtt.i_countTopicsToSubscribe];

  mqtt.ppc_topicsToSuscribe[0] = "devices";

  //global
  char* pc_tmp = new char[255];
  ("conf/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[1] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[1]) + "/keyboard").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[2] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[1]) + "/bed").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[3] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[3]) + "/wall").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[4] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[3]) + "/side").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[5] = pc_tmp;

  //publishes initalisieren
  mqtt.i_countTopicsToPublish = 5;
  mqtt.ppc_topicsToPublish    = new char*[mqtt.i_countTopicsToPublish];

  //global
  pc_tmp = new char[255];
  ("status/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[0] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[0]) + "/keyboard").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[1] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[0]) + "/bed").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[2] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[2]) + "/wall").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[3] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[2]) + "/side").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[4] = pc_tmp;
}

void initMQTT() {
  //prüfen, ob MQTT verbunden
  if(!(mqtt.b_isConnected = mqtt.p_connection->connect(mqtt.ppc_topicsToSuscribe, mqtt.i_countTopicsToSubscribe, callback))) {
    //MQTT nicht verbunden
    return;
  }
  mqtt.p_connection->loop();

  //send msg
  mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[0], "power-on");

  //prüfen, ob der zustand des keyboardstrips aktiv ist
  if(keyboardAnimation->getStatus()) {
     //Strip ist aktiv
     mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[1], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[1], "idle");
  }

  //prüfen, ob der zustand des bedwall strips aktiv ist
  if(bedWallAnimation->getStatus()) {
    //Strip ist aktiv
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[3], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[3], "idle");
  }

   //prüfen, ob der zustand des bedside strips aktiv ist
  if(bedSideAnimation->getStatus()) {
    //Strip ist aktiv
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[4], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[4], "idle");
  }
}