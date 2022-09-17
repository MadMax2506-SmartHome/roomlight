#include "./src/Constants.h"

boolean b_isWlanConnected;
boolean b_isMqttConnected;
  
char** ppc_topicsToSuscribe;
char** ppc_topicsToPublish;

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
