#include "./Secrets.h"
#include "./src/Constants.h"

#include "./src/led_strip/Ledstrip.h"
#include "./src/colors/Colors.h"
#include "./src/animation/Animation.h"
#include "./src/network/Network.h"
#include "./src/mqttCallbackHandler/MqttCallbackHandler.h"

boolean b_isWlanConnected;
boolean b_isMqttConnected;

WlanESP* p_wlan;
WiFiClient p_espClient;
OTA_ESP* p_ota;
MQTT_ESP* p_mqtt;

Colors* p_color;
Ledstrip* p_stripKeyboard;
Ledstrip* p_stripBedWall;
Ledstrip* p_stripBedSide;

Animation* p_keyboardAnimation;
Animation* p_bedWallAnimation;
Animation* p_bedSideAnimation;

void setup() {
  Serial.begin(9600);

  p_wlan = new WlanESP(WLAN_SSID, WLAN_PASSWORD);
  p_ota = new OTA_ESP();
  p_mqtt = new MQTT_ESP(
    MQTT_SERVER_IP_ADDRESS,
    MQTT_SERVER_PORT,
    p_espClient
  );
  
  p_color = new Colors(MODUS_RGB);

  // keyboard
  p_stripKeyboard = new Ledstrip(PIN_D4, DEVICE_KEYBOARD_COUNT_LEDS, BRIGHTNESS, MODUS_RGB);
  p_keyboardAnimation = new Animation(
    p_stripKeyboard,
    p_color,
    DEVICE_KEYBOARD_CRC_STORAGE_INDEX,
    DEVICE_KEYBOARD_START_STORAGE_INDEX,
    DEVICE_KEYBOARD_END_STORAGE_INDEX
  );

  //bed-wall
  p_stripBedWall = new Ledstrip(PIN_D3, DEVICE_BEDWALL_COUNT_LEDS, BRIGHTNESS, MODUS_RGB);
  p_bedWallAnimation = new Animation(
    p_stripBedWall,
    p_color,
    DEVICE_BEDWALL_CRC_STORAGE_INDEX,
    DEVICE_BEDWALL_START_STORAGE_INDEX,
    DEVICE_BEDWALL_END_STORAGE_INDEX
  );

  //bed-side
  p_stripBedSide = new Ledstrip(PIN_D2, DEVICE_BEDSIDE_COUNT_LEDS, BRIGHTNESS, MODUS_RGB);
  p_bedSideAnimation = new Animation(
    p_stripBedSide,
    p_color,
    DEVICE_BEDSIDE_CRC_STORAGE_INDEX,
    DEVICE_BEDSIDE_START_STORAGE_INDEX,
    DEVICE_BEDSIDE_END_STORAGE_INDEX
  );
}

void loop() {
  p_bedWallAnimation->animate();
  p_bedSideAnimation->animate();
  p_keyboardAnimation->animate();

  initNetwork();
}

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