#include "./Secrets.h"
#include "./src/utils/Utils.h"

#include "./src/device/Device.h"
#include "./src/network/Network.h"
#include "./src/mqttCallbackHandler/MqttCallbackHandler.h"

WlanESP* p_wlan;
WiFiClient p_espClient;
OTA_ESP* p_ota;
MQTT_ESP* p_mqtt;
Network* p_network;

Colors* p_color;
Device* p_keyboardDevice;
Device* p_bedWallDevice;
Device* p_bedSideDevice;

MqttCallbackHandler* mqttCallbackHandler;

void setup() {
  Serial.begin(9600);

  p_wlan = new WlanESP(WLAN_SSID, WLAN_PASSWORD);
  p_ota = new OTA_ESP();
  p_mqtt = new MQTT_ESP(MQTT_SERVER_IP_ADDRESS, MQTT_SERVER_PORT, p_espClient);
  p_network = new Network(p_wlan, p_ota, p_mqtt);

  p_color = new Colors(MODUS_RGB);
  p_keyboardDevice = new Device("deskLightingKeyboard", PIN_D4, 12, 0, 1, 7, p_color);
  p_bedWallDevice = new Device("deskLightingBedWall", PIN_D3, 60, 8, 9, 15, p_color);
  p_bedSideDevice = new Device("deskLightingBedSide", PIN_D2, 60, 16, 17, 23, p_color);

  mqttCallbackHandler = new MqttCallbackHandler(
    p_mqtt,
    p_color,
    p_keyboardDevice,
    p_bedWallDevice,
    p_bedSideDevice
  );

  p_network->init(
    p_keyboardDevice,
    p_bedWallDevice,
    p_bedSideDevice,
    onMqttPayload
  );
}

void loop() {
  p_keyboardDevice->animate();
  p_bedWallDevice->animate();
  p_bedSideDevice->animate();
  
  p_network->loop();
}

void onMqttPayload(char* pc_topic, u_int8_t* pi_payload, unsigned int i_length) {
  mqttCallbackHandler->onMqttPayload(pc_topic, pi_payload, i_length);
}
