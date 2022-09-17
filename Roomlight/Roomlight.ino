#include "./src/Constants.h"

MqttConfigurationHandler* p_mqttConfigurationHandler;
Network* p_network;

Colors* p_color;

Ledstrip* p_stripKeyboard;
Ledstrip* p_stripBedWall;
Ledstrip* p_stripBedSide;

Animation* p_keyboardAnimation;
Animation* p_bedWallAnimation;
Animation* p_bedSideAnimation;

void setup() {
  Serial.begin(9600);

  p_network = new Network(
    p_color,
    p_keyboardAnimation,
    p_bedWallAnimation,
    p_bedSideAnimation
  );
  
  p_color = new Colors(MODUS_RGB);

  // keyboard
  p_stripKeyboard = new Ledstrip(PIN_D4, KEYBOARD_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  p_keyboardAnimation = new Animation(
    p_stripKeyboard,
    p_color,
    KEYBOARD_CONFIG_CRC_STORAGE_INDEX,
    KEYBOARD_CONFIG_START_STORAGE_INDEX,
    KEYBOARD_CONFIG_END_STORAGE_INDEX
  );

  //bed-wall
  p_stripBedWall = new Ledstrip(PIN_D3, BED_WALL_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  p_bedWallAnimation = new Animation(
    p_stripBedWall,
    p_color,
    BED_WALL_CONFIG_CRC_STORAGE_INDEX,
    BED_WALL_CONFIG_START_STORAGE_INDEX,
    BED_WALL_CONFIG_END_STORAGE_INDEX
  );

  //bed-side
  p_stripBedSide = new Ledstrip(PIN_D2, BED_SIDE_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  p_bedSideAnimation = new Animation(
    p_stripBedSide,
    p_color,
    BED_SIDE_CONFIG_CRC_STORAGE_INDEX,
    BED_SIDE_CONFIG_START_STORAGE_INDEX,
    BED_SIDE_CONFIG_END_STORAGE_INDEX
  );
}

void loop() {
  p_bedWallAnimation->animate();
  p_bedSideAnimation->animate();
  p_keyboardAnimation->animate();

  p_network->init(
    p_keyboardAnimation->getStatus(),
    p_bedWallAnimation->getStatus(),
    p_bedSideAnimation->getStatus()
  );
}
