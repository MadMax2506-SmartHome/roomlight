#include "MqttCallbackHandler.h"

void MqttCallbackHandler::onMqttPayload(char* pc_topic, u_int8_t* pi_payload, unsigned int i_length) {
  if(i_length == 0) return;
  pi_payload[i_length] = '\0'; // close the string

  String s_payload = String((char*) pi_payload);
  s_payload.trim();
  
  String s_command = s_payload.substring(0, s_payload.indexOf((": ")));
  s_command.trim();

  String s_value = s_command.length() == s_payload.length() ? "" : s_payload.substring(s_payload.indexOf((": ")) + 1);
  s_value.trim();

  handleMqttPayload(
    pc_topic,
    s_command,
    s_value
  );
}

void MqttCallbackHandler::handleMqttPayload(String s_topic, String s_command, String s_value) {
  if(!s_topic.startsWith(MQTT_CONF_TOPIC)) return;

  if(s_topic.equals(MQTT_GLOBAL_CONF_TOPIC)) {
    if(s_command.equals("list-devices")) {
      //device info
     listDevice(p_keyboardDevice);
     listDevice(p_bedWallDevice);
     listDevice(p_bedSideDevice);
    }
  } else if(s_topic.equals(p_keyboardDevice->getConfigTopic())) {
    handleDeviceConfigurations(p_keyboardDevice, s_command, s_value);
  } else if(s_topic.equals(p_bedWallDevice->getConfigTopic())) {
    handleDeviceConfigurations(p_bedWallDevice, s_command, s_value);
  } else if(s_topic.equals(p_bedSideDevice->getConfigTopic())) {
    handleDeviceConfigurations(p_bedSideDevice, s_command, s_value);
  }
}

void MqttCallbackHandler::listDevice(Device* p_device) {
  char* pc_status_topic = stringToChar(String(MQTT_GLOBAL_STATUS_TOPIC));
  p_mqtt->sendMSG(pc_status_topic, p_device->getData());
}

void MqttCallbackHandler::handleDeviceConfigurations(Device* p_device, String s_command, String s_value) {
  char* pc_topicToPublish = stringToChar(p_device->getStatusTopic());
  Animation* p_animation = p_device->p_animation;

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
    p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
  } else if(s_command.equals("orientation")) {
    //Richtung der Animation
    p_animation->setOrientation(s_value.charAt(0));
    p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
  } else if(s_command.equals("animation-typ")) {
    //Art der Animation
    if(s_value.equals("fade")) p_animation->setType('f');
    else if(s_value.equals("rainbow")) p_animation->setType('r');
    else p_animation->setType('t');

    p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
  } else if(s_value.equals("animation-time")) {
    //Zeit der Animation von jeder LED
    p_animation->setTime(s_value.toInt());
    p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
  } else if(s_command.equals("restart-animation")) {
    //Animation neuladen
    p_animation->restart();
  } else if(s_command.equals("active")) {
    //Strip einschalten
    if(!p_animation->getStatus()) {
      p_animation->setStatus(true);
      p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
    }
  } else if(s_command.equals("idle")) {
    //Strip ausschalten
    if(p_animation->getStatus()) {
      p_animation->setStatus(false);
      p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
    }
  } else if(s_command.equals("reload-conf")) {
    //Konfiguration zurücksetzen
    p_animation->reloadConf();
    p_animation->readConf();
  } else if(s_command.equals("save-conf")) {
    //Konfiguration speichern
    p_animation->writeConf();
  } else if(s_command.equals("get-conf")) {
    //Konfiguration ausgeben
    p_mqtt->sendMSG(pc_topicToPublish, p_device->getConfiguration());
  }
}
