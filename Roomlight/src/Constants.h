#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// includes
#include "../Secrets.h"

#include "./led_strip/Ledstrip.h"
#include "./colors/Colors.h"
#include "./animation/Animation.h"
#include "./network/Network.h"
#include "./mqttCallbackHandler/MqttCallbackHandler.h"

// general
#define DEVICE_NAME "roomlight"

#define COUNT_LIGHTS 3
#define BRIGHTNESS 250

// pins
#define PIN_D4 2
#define PIN_D3 0
#define PIN_D2 4
#define PIN_D1 5

// mqtt
#define MQTT_GLOBAL_CONF_TOPIC "conf/devices"
#define MQTT_GLOBAL_STATUS_TOPIC "status/devices"

#define MQTT_COUNT_TOPICS_TO_SUBSCRIBE 6
#define MQTT_COUNT_TOPICS_TO_PUBLISH 5

// devices
#define DEVICE_KEYBOARD_ID "deskLightingKeyboard"
#define DEVICE_KEYBOARD_COUNT_LEDS 12
#define DEVICE_KEYBOARD_CRC_STORAGE_INDEX 0
#define DEVICE_KEYBOARD_START_STORAGE_INDEX 1
#define DEVICE_KEYBOARD_END_STORAGE_INDEX 7

#define DEVICE_BEDWALL_ID "deskLightingBedWall"
#define DEVICE_BEDWALL_COUNT_LEDS 60
#define DEVICE_BEDWALL_CRC_STORAGE_INDEX 8
#define DEVICE_BEDWALL_START_STORAGE_INDEX 9
#define DEVICE_BEDWALL_END_STORAGE_INDEX 15

#define DEVICE_BEDSIDE_ID "deskLightingBedSide"
#define DEVICE_BEDSIDE_COUNT_LEDS 60
#define DEVICE_BEDSIDE_CRC_STORAGE_INDEX 16
#define DEVICE_BEDSIDE_START_STORAGE_INDEX 17
#define DEVICE_BEDSIDE_END_STORAGE_INDEX 23

#endif //_CONSTANTS_H_
