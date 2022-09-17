#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

// general
#define DEVICE_NAME "roomlight"
#define COUNT_LIGHTS 3

// pins
#define PIN_D4 2
#define PIN_D3 0
#define PIN_D2 4
#define PIN_D1 5

// mqtt
#define MQTT_CONF_TOPIC "conf"
#define MQTT_GLOBAL_CONF_TOPIC String(MQTT_CONF_TOPIC) + "/devices"

#define MQTT_STATUS_TOPIC "status"
#define MQTT_GLOBAL_STATUS_TOPIC String(MQTT_STATUS_TOPIC) + "/devices"

#endif //_CONSTANTS_H_
