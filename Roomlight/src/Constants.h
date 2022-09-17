#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// includes
#include "../Secrets.h"

#include "./led_strip/Ledstrip.h"
#include "./colors/Colors.h"

#include "./wlan/WlanESP.h"
#include "./refresh_over_the_air/OTA_ESP.h"
#include "./mqtt/MQTT_ESP.h"

#include "./animation/Animation.h"

// pins
#define PIN_D4 2
#define PIN_D3 0
#define PIN_D2 4
#define PIN_D1 5

// network
#define WLAN_MAX_CONNETION_TRYS 10

#define MQTT_MAX_CONNETION_TRYS 10
#define MQTT_RETAINED false

// lights
#define COUNT_LIGHTS 3
#define BRIGHTNESS 250

#define KEYBOARD_STRIP_LEDS 12
#define KEYBOARD_CONFIG_CRC_STORAGE_INDEX 0
#define KEYBOARD_CONFIG_START_STORAGE_INDEX 1
#define KEYBOARD_CONFIG_END_STORAGE_INDEX 7

#define BED_WALL_STRIP_LEDS 60
#define BED_WALL_CONFIG_CRC_STORAGE_INDEX 8
#define BED_WALL_CONFIG_START_STORAGE_INDEX 9
#define BED_WALL_CONFIG_END_STORAGE_INDEX 15

#define BED_SIDE_STRIP_LEDS 60
#define BED_SIDE_CONFIG_CRC_STORAGE_INDEX 16
#define BED_SIDE_CONFIG_START_STORAGE_INDEX 17
#define BED_SIDE_CONFIG_END_STORAGE_INDEX 23

// other
#define DEVICE_NAME "roomlight"

#endif //_CONSTANTS_H_
