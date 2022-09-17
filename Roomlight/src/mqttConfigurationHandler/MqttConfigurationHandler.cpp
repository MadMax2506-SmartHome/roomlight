#include "MqttConfigurationHandler.h"

void MqttConfigurationHandler::callback(char* topic, u_int8_t* payload, unsigned int length) {
  if(length == 0) return;

  payload[length] = '\0';
  char* pc_msg    = (char*) payload;

  char c_delimiterCommand[] = ": ";
  char c_delimiterCommandContent[] = "; ";

  char* pc_command = strtok(pc_msg, c_delimiterCommand);

  if(strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[0]) == 0) {
    if(strcmp(pc_command, "list-devices") == 0) {
      //device info
      MqttConfigurationHandler::p_mqtt->sendMSG(
        MqttConfigurationHandler::ppc_topicsToSuscribe[0],
        getDeviceInfoAsJSON()
      );
    }
  } else if(strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[1]) == 0) {
    //Global Conf

    if(strcmp(pc_command, "get-conf") == 0) {
      //Konfiguration publishen
      char** ppc_configs = getConfAsJSON();

      MqttConfigurationHandler::p_mqtt->sendMSG(MqttConfigurationHandler::ppc_topicsToPublish[0], "start");
      for(int i = 0; i < COUNT_LIGHTS; i++) {
        MqttConfigurationHandler::p_mqtt->sendMSG(MqttConfigurationHandler::ppc_topicsToPublish[0], ppc_configs[i]);
      }
      MqttConfigurationHandler::p_mqtt->sendMSG(MqttConfigurationHandler::ppc_topicsToPublish[0], "end");
    }
  } else if(strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[2]) == 0
          || strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[4]) == 0
          || strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[5]) == 0) {
    //keyboard / bedWall / bedSide

    char* pc_topicToPublish = NULL;
    Animation* p_animation  = NULL;
    if(strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[2]) == 0) {
      // keyboard

      pc_topicToPublish = MqttConfigurationHandler::ppc_topicsToPublish[1];
      p_animation = MqttConfigurationHandler::p_keyboardAnimation;
    } else if(strcmp(topic, MqttConfigurationHandler::ppc_topicsToSuscribe[4]) == 0){
      // bedWall

      pc_topicToPublish = MqttConfigurationHandler::ppc_topicsToPublish[3];
      p_animation = MqttConfigurationHandler::p_bedWallAnimation;
    } else {
      // bedSide

      pc_topicToPublish = MqttConfigurationHandler::ppc_topicsToPublish[4];
      p_animation = MqttConfigurationHandler::p_bedSideAnimation;
    }

    char* pc_commandContent = strtok(NULL, " ");

    // Statusabfrage
    if(strcmp(pc_command, "status") == 0) {
      char* statusStr = NULL;
      if(p_animation->getStatus()) { statusStr = "active"; }
      else { statusStr = "idle"; }
      MqttConfigurationHandler::p_mqtt->sendMSG(pc_topicToPublish, statusStr);
    }

    //Art der Konfigurationsänderung prüfen
    if(strcmp(pc_command, "color") == 0) {
      //Farbe
      int i_r = atoi(strtok(pc_commandContent, c_delimiterCommandContent));
      int i_g = atoi(strtok(NULL, c_delimiterCommandContent));
      int i_b = atoi(strtok(NULL, c_delimiterCommandContent));
      p_animation->setColor(MqttConfigurationHandler::p_color->getMix(i_r, i_g, i_b));
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
        MqttConfigurationHandler::p_mqtt->sendMSG(pc_topicToPublish, "active");
      }
    } else if(strcmp(pc_command, "idle") == 0) {
      //Strip ist im Leerlauf
      if(p_animation->getStatus()) {
        p_animation->setStatus(false);
        MqttConfigurationHandler::p_mqtt->sendMSG(pc_topicToPublish, "idle");
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

// TODO
char** MqttConfigurationHandler::getConfAsJSON() {
  //json beginnen
  String str_json;
  char** ppc_json = new char*[COUNT_LIGHTS];

  str_json    = "{\"keyboard\":" + String(MqttConfigurationHandler::p_keyboardAnimation->getConfAsJSON()) + "}";
  ppc_json[0] = new char[255];
  str_json.toCharArray(ppc_json[0], 255);

  str_json    = "{\"bed-wall\":" + String(MqttConfigurationHandler::p_bedWallAnimation->getConfAsJSON()) + "}";
  ppc_json[1] = new char[255];
  str_json.toCharArray(ppc_json[1], 255);

  str_json    = "{\"bed-side\":" + String(MqttConfigurationHandler::p_bedSideAnimation->getConfAsJSON()) + "}";
  ppc_json[2] = new char[255];
  str_json.toCharArray(ppc_json[2], 255);

  return ppc_json;
}

char* MqttConfigurationHandler::getDeviceInfoAsJSON() {
  //json beginnen
  String str_json = "{";

  //name
  str_json+= "\"name\": \"" + String(DEVICE_NAME) + "\",";

  //mac-Adresse
  str_json+= "\"mac-address\": \"" + String("TODO") + "\",";

  //topics
  str_json+= "\"topic\":";
  str_json+= "{";
  str_json+= "\"conf\": \"" + String(MqttConfigurationHandler::ppc_topicsToSuscribe[1]) + "\",";
  str_json+= "\"status\": \"" + String(MqttConfigurationHandler::ppc_topicsToPublish[0]) + "\"";
  str_json+= "}";

  //json beenden
  str_json+= "}";
  char* pc_json = new char[255];
  str_json.toCharArray(pc_json, 255);
  return pc_json;
}
