#include "OTA_ESP.h"

//Konstruktoren
OTA_ESP::OTA_ESP(){}

OTA_ESP::OTA_ESP(int i_port){
  ArduinoOTA.setPort(i_port);
}

//OTA konfigurieren
void OTA_ESP::init() {
  //zu Beginn des hochladens
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });

  //beim Ende des hochladens
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });

  //während des hochladens
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });

  //bei Fehlern, während des hochladens des hochladens
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });

  //debug prints
  Serial.println("\n\n--------------------------------------------------");
  Serial.println("OTA is available");
  Serial.println();
  Serial.print("hostname: ");
  Serial.println(ArduinoOTA.getHostname());
  Serial.println("--------------------------------------------------\n");

  //ota starten
  ArduinoOTA.begin();
}

void OTA_ESP::init(char* pc_hostname) {
  //hostname setzen
  ArduinoOTA.setHostname(pc_hostname);

  //initalisieren
  init();
}

void OTA_ESP::init(char* pc_hostname, char* pc_password) {
  //hostname und passwort setzen
  ArduinoOTA.setHostname(pc_hostname);
  ArduinoOTA.setPassword(pc_password);

  //initalisieren
  init();
}

//OTA fortführen
void OTA_ESP::handle() {
  ArduinoOTA.handle();
}
