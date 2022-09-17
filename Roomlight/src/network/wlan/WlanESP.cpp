#include "WlanESP.h"

boolean WlanESP::connect() {
  //connecting to network
  WiFi.mode(WIFI_STA);
  p_WiFiMulti.addAP(pc_ssid, pc_password);

  //Puffer
  Serial.println("\n\n--------------------------------------------------");
  Serial.print("Wait for WiFi...");

  //versucht eine Verbindung aufzubauen (maximal: i_maxTrys)
  int i_trys = 0;
  b_isAvailable =  true;
  while (p_WiFiMulti.run() != WL_CONNECTED) {
    if(i_trys == i_maxTrys) {
      b_isAvailable = false;
      break;
    }
    Serial.print(".");
    delay(500);
    i_trys++;
  }

  //print debugs
  Serial.println("\n");
  if(b_isAvailable) {
    Serial.println("WiFi is available");
    Serial.print("ip-address: ");
    Serial.println(WiFi.localIP());
    Serial.print("mac-address: ");
    Serial.println(WiFi.macAddress());
  } else {
    Serial.println("WiFi is  not available");
  }
  Serial.println("--------------------------------------------------\n");
  return b_isAvailable;
}

//Getter-Methoden
char* WlanESP::getMac() {
  char* pc_mac = new char[20];
  WiFi.macAddress().toCharArray(pc_mac, 20);
  return pc_mac;
}

char* WlanESP::getIp() {
  String strIp = String(WiFi.localIP()[0]) + String(".") + String(WiFi.localIP()[1]) + String(".") + String(WiFi.localIP()[2]) + String(".") + String(WiFi.localIP()[3]) ;
  char* pc_ip = new char[20];
  strIp.toCharArray(pc_ip, 20);
  return pc_ip;
}
