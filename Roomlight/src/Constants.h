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

#define COUNT_LIGHTS 3

#define DEVICE_NAME "roomlight"

#endif //_CONSTANTS_H_
