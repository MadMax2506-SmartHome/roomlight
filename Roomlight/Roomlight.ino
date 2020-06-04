#include <Ledstrip.h>
#include <Colors.h>

#include <WlanESP.h>
#include <OTA_ESP.h>
#include <MQTT_ESP.h>

#include "src/Animation/Animation.h"

#define PIN_D4 2
#define PIN_D3 0
#define PIN_D2 4
#define PIN_D1 5

#define COUNT_LIGHTS 3

//netwok
struct wlan {
  int i_maxConnectionTrys             = 10;
  
  boolean b_isConnected = false;

  char* pc_ssid     = "Max";
  char* pc_password = "handball";
  char* pc_mac;

  WlanESP* p_connection;
  WiFiClient p_espClient;
} wlan;

struct ota {
  char* pc_password = "madmax";
  OTA_ESP* p_setup;
} ota;

struct mqtt {
  byte* pbyte_ip                      = new byte[4]{192, 168, 178, 150};
  int i_port                          = 1883;
  int i_maxConnectionTrys             = 2;
  int i_countTopicsToSubscribe;
  int i_countTopicsToPublish;
  
  boolean b_retained       = false;
  boolean b_isConnected    = false;

  char* devicename  = "Roomlight Max";
  
  char** ppc_topicsToSuscribe;
  char** ppc_topicsToPublish;

  MQTT_ESP* p_connection;
} mqtt;

//animation
Colors* p_color;

struct ledstrip {
  int i_pin;
  int i_countLeds;
  int i_brightness    = 250;
  int i_colormodus    = 0;

  Ledstrip* p_physical;
} stripKeyboard, stripBedWall, stripBedSide;

struct animation {
  int i_crcStorageIndex;
  int i_startStorageIndex;
  int i_endStorageIndex;
  Animation* p_animation;
} keyboard, bedWall, bedSide;

void setup() {
  setupData();
}

void loop() {
  bedWall.p_animation->animate();
  bedSide.p_animation->animate();
  keyboard.p_animation->animate();

  initNetwork();
}

void setupData() {
//Serial
  Serial.begin(9600);
  
  //wlan
  wlan.p_connection = new WlanESP(wlan.pc_ssid, wlan.pc_password, wlan.i_maxConnectionTrys);

  //ota
  ota.p_setup = new OTA_ESP();

  //mqtt
  mqtt.p_connection = new MQTT_ESP(mqtt.pbyte_ip, mqtt.i_port, wlan.p_espClient, mqtt.i_maxConnectionTrys, mqtt.b_retained);
  
  //color erstellen
  p_color = new Colors(stripKeyboard.i_colormodus);

//keyboardlight
  //ledstrip erstellen
  stripKeyboard.i_pin           = PIN_D4;
  stripKeyboard.i_countLeds     = 12;
  stripKeyboard.p_physical      = new Ledstrip(stripKeyboard.i_pin, stripKeyboard.i_countLeds, stripKeyboard.i_brightness, stripKeyboard.i_colormodus);

  //animation
  keyboard.i_crcStorageIndex      = 0;
  keyboard.i_startStorageIndex    = 1;
  keyboard.i_endStorageIndex      = 7;
  keyboard.p_animation            = new Animation(stripKeyboard.p_physical, p_color, keyboard.i_crcStorageIndex, keyboard.i_startStorageIndex, keyboard.i_endStorageIndex);

//bedlight - wall
  //ledstrip erstellen
    stripBedWall.i_pin       = PIN_D3;
    stripBedWall.i_countLeds = 60;
    stripBedWall.p_physical  = new Ledstrip(stripBedWall.i_pin, stripBedWall.i_countLeds, stripBedWall.i_brightness, stripBedWall.i_colormodus);
  
    //animation
    bedWall.i_crcStorageIndex      = 8;
    bedWall.i_startStorageIndex    = 9;
    bedWall.i_endStorageIndex      = 15;
    bedWall.p_animation            = new Animation(stripBedWall.p_physical, p_color, bedWall.i_crcStorageIndex, bedWall.i_startStorageIndex, bedWall.i_endStorageIndex);

//bedlight -side
  //ledstrip erstellen
    stripBedSide.i_pin       = PIN_D2;
    stripBedSide.i_countLeds = 60;
    stripBedSide.p_physical  = new Ledstrip(stripBedSide.i_pin, stripBedSide.i_countLeds, stripBedSide.i_brightness, stripBedSide.i_colormodus);
  
    //animation
    bedSide.i_crcStorageIndex      = 16;
    bedSide.i_startStorageIndex    = 17;
    bedSide.i_endStorageIndex      = 23;
    bedSide.p_animation            = new Animation(stripBedSide.p_physical, p_color, bedSide.i_crcStorageIndex, bedSide.i_startStorageIndex, bedSide.i_endStorageIndex);
}

void initNetwork() {
  //prüfen, ob WLAN verbunden ist
  if(wlan.b_isConnected) {
    //WLAN verbunden
    ota.p_setup->handle();
    
    //prüfen, ob MQTT verbunden ist
    if(mqtt.b_isConnected) {
      //MQTT verbunden
      mqtt.p_connection->loop();
    } else {
      //MQTT nicht verbunden
      initMQTT();
    }
  } else {
    //WLAN nicht verbunden
    initWLAN();
  
    //prüfen, ob WLAN verbunden
    if(!wlan.b_isConnected) {
      //WLAN nicht verbunden
      return;
    }
    
    //mqtt
    initMQTT();
  }
}

void initWLAN() {
  //pürfen, ob wlan verbunden ist
  if(!(wlan.b_isConnected = wlan.p_connection->connect())) {
    //WLAN nicht verbunden
    return;
  }

  wlan.pc_mac = wlan.p_connection->getMac();

  //ota initalisieren
  ota.p_setup->init(wlan.pc_mac, ota.pc_password);

  //mqtt
  //subscribs initalisieren
  mqtt.i_countTopicsToSubscribe  = 6;
  mqtt.ppc_topicsToSuscribe      = new char*[mqtt.i_countTopicsToSubscribe];
  
  mqtt.ppc_topicsToSuscribe[0] = "devices";

  //global
  char* pc_tmp = new char[255];
  ("conf/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[1] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[1]) + "/keyboard").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[2] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[1]) + "/bed").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[3] = pc_tmp;
  
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[3]) + "/wall").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[4] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToSuscribe[3]) + "/side").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToSuscribe[5] = pc_tmp;

  //publishes initalisieren
  mqtt.i_countTopicsToPublish = 5;
  mqtt.ppc_topicsToPublish    = new char*[mqtt.i_countTopicsToPublish];

  //global
  pc_tmp = new char[255];
  ("status/" + String(wlan.pc_mac)).toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[0] = pc_tmp;

  //keyboard
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[0]) + "/keyboard").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[1] = pc_tmp;

  //bed
  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[0]) + "/bed").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[2] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[2]) + "/wall").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[3] = pc_tmp;

  pc_tmp = new char[255];
  (String(mqtt.ppc_topicsToPublish[2]) + "/side").toCharArray(pc_tmp, 255);
  mqtt.ppc_topicsToPublish[4] = pc_tmp;
}

void initMQTT() {
  //prüfen, ob MQTT verbunden
  if(!(mqtt.b_isConnected = mqtt.p_connection->connect(mqtt.ppc_topicsToSuscribe, mqtt.i_countTopicsToSubscribe, callback))) {
    //MQTT nicht verbunden
    return;
  }
  mqtt.p_connection->loop();
  
  //send msg
  mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[0], "power-on");

  //prüfen, ob der zustand des keyboardstrips aktiv ist
  if(keyboard.p_animation->getStatus()) {
     //Strip ist aktiv
     mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[1], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[1], "idle");
  }
  
  //prüfen, ob der zustand des bedwall strips aktiv ist
  if(bedWall.p_animation->getStatus()) {
    //Strip ist aktiv
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[3], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[3], "idle");
  }

   //prüfen, ob der zustand des bedside strips aktiv ist
  if(bedSide.p_animation->getStatus()) {
    //Strip ist aktiv
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[4], "active");
  } else {
    //Strip ist im Leerlauf
    mqtt.p_connection->sendMSG(mqtt.ppc_topicsToPublish[4], "idle");
  }
}

char** getConfAsJSON() {
  //json beginnen
  String str_json;
  char** ppc_json = new char*[COUNT_LIGHTS];

  str_json    = "{\"keyboard\":" + String(keyboard.p_animation->getConfAsJSON()) + "}";
  ppc_json[0] = new char[255];
  str_json.toCharArray(ppc_json[0], 255);

  str_json    = "{\"bed-wall\":" + String(bedWall.p_animation->getConfAsJSON()) + "}";
  ppc_json[1] = new char[255];
  str_json.toCharArray(ppc_json[1], 255);

  str_json    = "{\"bed-side\":" + String(bedSide.p_animation->getConfAsJSON()) + "}";
  ppc_json[2] = new char[255];
  str_json.toCharArray(ppc_json[2], 255);

  return ppc_json;
}

char* getDeviceInfoAsJSON() {
  //json beginnen
  String str_json = "{";

  //name
  str_json+= "\"name\": \"" + String(mqtt.devicename) + "\",";

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
      p_animation       = keyboard.p_animation;
    } else if(strcmp(topic, mqtt.ppc_topicsToSuscribe[4]) == 0){
      // bedWall
      
      pc_topicToPublish = mqtt.ppc_topicsToPublish[3];
      p_animation       = bedWall.p_animation;
    } else {
      // bedSide
      
      pc_topicToPublish = mqtt.ppc_topicsToPublish[4];
      p_animation       = bedSide.p_animation;
    }
    
    char* pc_commandContent = strtok(NULL, " ");
    
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
