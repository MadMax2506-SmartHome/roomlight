#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

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

#endif //_CONSTANTS_H_
