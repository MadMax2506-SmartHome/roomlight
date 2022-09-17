#include "./src/Constants.h"

OTA_ESP* ota;

struct wlan {
  boolean b_isConnected = false;
  char* pc_mac;

  WlanESP* p_connection;
  WiFiClient p_espClient;
} wlan;

struct mqtt {
  int i_countTopicsToSubscribe;
  int i_countTopicsToPublish;

  boolean b_isConnected = false;

  char** ppc_topicsToSuscribe;
  char** ppc_topicsToPublish;

  MQTT_ESP* p_connection;
} mqtt;

Colors* p_color;

Ledstrip* stripKeyboard;
Ledstrip* stripBedWall;
Ledstrip* stripBedSide;

Animation* keyboardAnimation;
Animation* bedWallAnimation;
Animation* bedSideAnimation;

void setup() {
  Serial.begin(9600);

  wlan.p_connection = new WlanESP(WLAN_SSID, WLAN_PASSWORD, WLAN_MAX_CONNETION_TRYS);
  ota = new OTA_ESP();
  mqtt.p_connection = new MQTT_ESP(MQTT_SERVER_IP_ADDRESS, MQTT_SERVER_PORT, wlan.p_espClient, MQTT_MAX_CONNETION_TRYS, MQTT_RETAINED);

  p_color = new Colors(MODUS_RGB);

  // keyboard
  stripKeyboard = new Ledstrip(PIN_D4, KEYBOARD_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  keyboardAnimation = new Animation(stripKeyboard, p_color, KEYBOARD_CONFIG_CRC_STORAGE_INDEX, KEYBOARD_CONFIG_START_STORAGE_INDEX, KEYBOARD_CONFIG_END_STORAGE_INDEX);

  //bed-wall
  stripBedWall = new Ledstrip(PIN_D3, BED_WALL_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  bedWallAnimation = new Animation(stripBedWall, p_color, BED_WALL_CONFIG_CRC_STORAGE_INDEX, BED_WALL_CONFIG_START_STORAGE_INDEX, BED_WALL_CONFIG_END_STORAGE_INDEX);

  //bed-side
  stripBedSide = new Ledstrip(PIN_D2, BED_SIDE_STRIP_LEDS, BRIGHTNESS, MODUS_RGB);
  bedSideAnimation = new Animation(stripBedSide, p_color, BED_SIDE_CONFIG_CRC_STORAGE_INDEX, BED_SIDE_CONFIG_START_STORAGE_INDEX, BED_SIDE_CONFIG_END_STORAGE_INDEX);
}

void loop() {
  bedWallAnimation->animate();
  bedSideAnimation->animate();
  keyboardAnimation->animate();

  initNetwork();
}

char** getConfAsJSON() {
  //json beginnen
  String str_json;
  char** ppc_json = new char*[COUNT_LIGHTS];

  str_json    = "{\"keyboard\":" + String(keyboardAnimation->getConfAsJSON()) + "}";
  ppc_json[0] = new char[255];
  str_json.toCharArray(ppc_json[0], 255);

  str_json    = "{\"bed-wall\":" + String(bedWallAnimation->getConfAsJSON()) + "}";
  ppc_json[1] = new char[255];
  str_json.toCharArray(ppc_json[1], 255);

  str_json    = "{\"bed-side\":" + String(bedSideAnimation->getConfAsJSON()) + "}";
  ppc_json[2] = new char[255];
  str_json.toCharArray(ppc_json[2], 255);

  return ppc_json;
}

char* getDeviceInfoAsJSON() {
  //json beginnen
  String str_json = "{";

  //name
  str_json+= "\"name\": \"" + String(DEVICE_NAME) + "\",";

  //mac-Adresse
  str_json+= "\"mac-address\": \"" + String(wlan.pc_mac) + "\",";

  //topics
  str_json+= "\"topic\":";
  str_json+= "{";
  str_json+= "\"conf\": \"" + String(mqtt.ppc_topicsToSuscribe[1]) + "\",";
  str_json+= "\"status\": \"" + String(mqtt.ppc_topicsToPublish[0]) + "\"";
  str_json+= "}";

  //json beenden
  str_json+= "}";
  char* pc_json = new char[255];
  str_json.toCharArray(pc_json, 255);
  return pc_json;
}

void callback(char* topic, byte* payload, unsigned int length) {
  if(length == 0) {
    return;
  }

  payload[length] = '\0';
  char* pc_msg    = (char*) payload;

  char c_delimiterCommand[] = ": ";
  char c_delimiterCommandContent[] = "; ";

  char* pc_command = strtok(pc_msg, c_delimiterCommand);

  if(strcmp(topic, mqtt.ppc_topicsToSuscribe[0]) == 0) {
    if(strcmp(pc_command, "list-devices") == 0) {
      //device info
      mqtt.p_connection->sendMSG(mqtt.ppc_topicsToSuscribe[0], getDeviceInfoAsJSON());
    }
  } else if(strcmp(topic, mqtt.ppc_topicsToSuscribe[1]) == 0) {
    //Global Conf

    if(strcmp(pc_command, "get-conf") == 0) {
      //Konfiguration publishen
      char** ppc_configs = getConfAsJSON();

      mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[0], "start");
      for(int i = 0; i < COUNT_LIGHTS; i++) {
        mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[0], ppc_configs[i]);
      }
      mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[0], "end");
    }
  } else if(strcmp(topic, mqtt.ppc_topicsToSuscribe[2]) == 0
          || strcmp(topic, mqtt.ppc_topicsToSuscribe[4]) == 0
          || strcmp(topic, mqtt.ppc_topicsToSuscribe[5]) == 0) {
    //keyboard / bedWall / bedSide

    char* pc_topicToPublish = NULL;
    Animation* p_animation  = NULL;
    if(strcmp(topic, mqtt.ppc_topicsToSuscribe[2]) == 0) {
      // keyboard

      pc_topicToPublish  = mqtt.ppc_topicsToPublish[1];
      p_animation       = keyboardAnimation;
    } else if(strcmp(topic, mqtt.ppc_topicsToSuscribe[4]) == 0){
      // bedWall

      pc_topicToPublish = mqtt.ppc_topicsToPublish[3];
      p_animation       = bedWallAnimation;
    } else {
      // bedSide

      pc_topicToPublish = mqtt.ppc_topicsToPublish[4];
      p_animation       = bedSideAnimation;
    }

    char* pc_commandContent = strtok(NULL, " ");

    // Statusabfrage
    if(strcmp(pc_command, "status") == 0) {
      char* statusStr = NULL;
      if(p_animation->getStatus()) { statusStr = "active"; }
      else { statusStr = "idle"; }
      mqtt.p_connection->sendMSG(pc_topicToPublish, statusStr);
    }

    //Art der Konfigurationsänderung prüfen
    if(strcmp(pc_command, "color") == 0) {
      //Farbe
      int i_r = atoi(strtok(pc_commandContent, c_delimiterCommandContent));
      int i_g = atoi(strtok(NULL, c_delimiterCommandContent));
      int i_b = atoi(strtok(NULL, c_delimiterCommandContent));
      p_animation->setColor(p_color->getMix(i_r, i_g, i_b));
    } else if(strcmp(pc_command, "orientation") == 0) {
      //Richtung der Animation
      p_animation->setOrientation((strtok(pc_commandContent, c_delimiterCommandContent))[0]);
    } else if(strcmp(pc_command, "animation-typ") == 0) {
      //Art der Animation
      char* pc_tmp = new char[255];
      pc_tmp = strtok(pc_commandContent, c_delimiterCommandContent);

      if(strcmp(pc_tmp, "fade") == 0) {
        p_animation->setType('f');
      } else if(strcmp(pc_tmp, "rainbow") == 0) {
        p_animation->setType('r');
      } else {
        p_animation->setType('t');
      }
    } else if(strcmp(pc_command, "animation-time") == 0) {
      //Zeit der Animation von jeder LED
      p_animation->setTime(atoi(strtok(pc_commandContent, c_delimiterCommandContent)));
    } else if(strcmp(pc_command, "restart-animation") == 0) {
      //Animation neuladen
      p_animation->restart();
    } else if(strcmp(pc_command, "active") == 0) {
      //Strip ist aktiv
      if(!p_animation->getStatus()) {
        p_animation->setStatus(true);
        mqtt.p_connection->sendMSG(pc_topicToPublish, "active");
      }
    } else if(strcmp(pc_command, "idle") == 0) {
      //Strip ist im Leerlauf
      if(p_animation->getStatus()) {
        p_animation->setStatus(false);
        mqtt.p_connection->sendMSG(pc_topicToPublish, "idle");
      }
    } else if(strcmp(pc_command, "reload-conf") == 0) {
      //Konfiguration zurücksetzen
      p_animation->reloadConf();
      p_animation->readConf();
    } else if(strcmp(pc_command, "save-conf") == 0) {
      //Konfiguration speichern
      p_animation->writeConf();
    }
  }
}
