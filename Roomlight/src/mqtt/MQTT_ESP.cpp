#include "MQTT_ESP.h"

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

void MQTT_ESP::setRetained(boolean b_retained) {
  MQTT_ESP::b_retained = b_retained;
}

boolean MQTT_ESP::connect(void callback(char* topic, byte* payload, unsigned int length)) {
  //neuer client
  p_client = new PubSubClient(p_espClient);

  //Server und port setzen
  p_client->setServer(pbyte_ip, i_port);

  //callback function setzen
  p_client->setCallback(callback);

  //initalisieren
  b_isMqttInit = true;

  return reconnect();
}

boolean MQTT_ESP::connect(char** ppc_topicsToSubscribe, int i_countTopicsToSubscribe, void callback(char* topic, byte* payload, unsigned int length)) {
  //topics zum subscriben und  anzahl, der topics zum subscriben setzen
  MQTT_ESP::ppc_topicsToSubscribe = ppc_topicsToSubscribe;
  MQTT_ESP::i_countTopicsToSubscribe = i_countTopicsToSubscribe;

  //verbindung initalisieren
  return connect(callback);
}

boolean MQTT_ESP::reconnect() {
  //prüfen, ob mqtt initalisieren ist und keine Verbindung besteht
  if(b_isMqttInit
    && !p_client->connected()) {
    //debug print
    Serial.println("\n\n--------------------------------------------------");
    Serial.print("Wait for MQTT-Server...");

    //wiederholen, solange keine Verbindung besteht
    int i_connectionTrys  = 0;
    while (!p_client->connected()) {
      //prüfen, ob Anzahl, der Versuche zur Verbindungsaufname, der maximalen Anzahl Versuche zur Verbindungsaufname entspricht
      if(i_connectionTrys == i_maxConnectionTrys) {
        b_isMqttAvailable = false;

        //debug print
        Serial.println();
        Serial.println("\nMQTT-Brocker is not available");
        break;
      }

      //prüfen, ob Verbindung besteht
      srand(time(NULL));

      int number = rand();
      char client[255];
      sprintf(client, "esp32_%d",number);

      if(p_client->connect( client )) {
        b_isMqttAvailable = true;

        //debug print
        Serial.println();
        Serial.println("MQTT-Brocker is available");
        Serial.print("\nip-address: ");

        //wiederholt viermal, für jedes Segment der IP-Adresse
        for(int i = 0; i < 4; i++) {
          if(i != 0) {
            //debug print
            Serial.print(".");
          }
          Serial.print(pbyte_ip[i]);
        }

        //debug print
        Serial.print(" on port: ");
        Serial.println(i_port);

        Serial.print("client_id: ");
        Serial.println(client);

        //wiederholt für jede topic
        for(int i = 0; i < i_countTopicsToSubscribe; i++) {
          if(i == 0) {
            Serial.println();
          }

          //prüfen, ob subscribe zur topic funktioniert hat
          if(p_client->subscribe(ppc_topicsToSubscribe[i])) {
            //debug print
            Serial.print("subscribe to ");
            Serial.println(ppc_topicsToSubscribe[i]);
          } else {
            //debug print
            Serial.print("cannot subscribe to ");
            Serial.println(ppc_topicsToSubscribe[i]);
          }
        }
      } else {
        //debug print
        Serial.print(".");
        i_connectionTrys++;
      }
    }
    //debug print
    Serial.println("--------------------------------------------------\n");
  }

  //client wiederholt starten
  loop();
  return b_isMqttAvailable;
}

void MQTT_ESP::loop() {
  //prüfen, ob mqtt nicht initalisiert wurde
  if(!b_isMqttInit) {
    return;
  }

  //client wiederholt starten
  p_client->loop();
}

boolean MQTT_ESP::sendMSG(char* pc_topic, char* pc_msg) {
  //prüfen, ob mqtt nicht initalisiert wurde
  if(!b_isMqttInit) {
    return false;
  }

  //erneut verbinden
  reconnect();

  //publish
  boolean b_result = false;
  if(b_isMqttAvailable) {
    //prüfen, ob kurze Nachricht
    int i_length = strlen(pc_msg);

    if(i_length < 100) {
      //kurze Nachricht

      b_result = p_client->publish(pc_topic, pc_msg, b_retained);
    } else {
      //lange Nachricht

      if((b_result = p_client->beginPublish(pc_topic, i_length, b_retained))) {
        for(int i = 0; i <= i_length; i++) {
          p_client->print(pc_msg[i]);
        }
        b_result = p_client->endPublish();
      }
    }
  }
  delay(100);
  return b_result;
}
