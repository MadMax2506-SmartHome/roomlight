#include "MqttCallbackHandler.h"

void MqttCallbackHandler::onMqttPayload(char* pc_topic, u_int8_t* pi_payload, unsigned int i_length) {
  if(i_length == 0) return;
  pi_payload[i_length] = '\0'; // close the string

  String s_payload = (char*) pi_payload;
  
  String s_command = s_payload.substring(0, s_payload.indexOf((": ")));
  String s_value = s_payload.substring(s_command.length());

  handleMqttPayload(
    pc_topic,
    s_command,
    s_value
  );  
}

void MqttCallbackHandler::handleMqttPayload(String s_topic, String s_command, String s_value) {
  // TODO
  if(!s_topic.startsWith("conf/")) return;

  if(s_topic.equals(MQTT_GLOBAL_CONF_TOPIC)) {
    if(s_command.equals("list-devices")) {
      // TODO
      //device info
      p_mqtt->sendMSG(
        MQTT_GLOBAL_STATUS_TOPIC,
        ""
      );
    }
  } else if(s_topic.equals("TODO keyboard")) {
    handleAnimation(p_keyboardAnimation, "TODO keyboard", s_command, s_value);
  } else if(s_topic.equals("TODO bed wall")) {
    handleAnimation(p_bedWallAnimation, "TODO bed wall", s_command, s_value);
  } else if(s_topic.equals("TODO bed side")) {
    handleAnimation(p_bedSideAnimation, "TODO bed side", s_command, s_value);
  }
}

void MqttCallbackHandler::handleAnimation(Animation* p_animation, char* pc_topicToPublish, String s_command, String s_value) {
  //Art der Konfigurationsänderung prüfen
  if(s_command.equals("color")) {
    //Farbe
    int i_indexFirstDelimiter = s_value.indexOf(";");
    int i_indexSecondDelimiter = s_value.lastIndexOf(";");

    int* color = p_color->getMix(
      s_value.substring(0, i_indexFirstDelimiter).toInt(), // red
      s_value.substring(i_indexFirstDelimiter + 1, i_indexSecondDelimiter).toInt(), // green
      s_value.substring(i_indexSecondDelimiter + 1).toInt() // blue
    );
    p_animation->setColor(color);
  } else if(s_command.equals("orientation")) {
    //Richtung der Animation
    p_animation->setOrientation(s_value.charAt(0));
  } else if(s_command.equals("animation-typ")) {
    //Art der Animation
    if(s_value.equals("fade")) p_animation->setType('f');
    else if(s_value.equals("rainbow")) p_animation->setType('r');
    else p_animation->setType('t');
  } else if(s_value.equals("animation-time")) {
    //Zeit der Animation von jeder LED
    p_animation->setTime(s_value.toInt());
  } else if(s_command.equals("restart-animation")) {
    //Animation neuladen
    p_animation->restart();
  } else if(s_command.equals("active")) {
    //Strip einschalten
    if(!p_animation->getStatus()) {
      p_animation->setStatus(true);
      p_mqtt->sendMSG(pc_topicToPublish, "active");
    }
  } else if(s_command.equals("idle")) {
    //Strip ausschalten
    if(p_animation->getStatus()) {
      p_animation->setStatus(false);
      p_mqtt->sendMSG(pc_topicToPublish, "idle");
    }
  } else if(s_command.equals("reload-conf")) {
    //Konfiguration zurücksetzen
    p_animation->reloadConf();
    p_animation->readConf();
  } else if(s_command.equals("save-conf")) {
    //Konfiguration speichern
    p_animation->writeConf();
  }
}
