#if defined(ESP32)
  #include <WiFi.h>
#endif

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#endif

#if defined(ESP8266)
WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
#endif

#if WIFIMANAGER
#include "ESPAsync_WiFiManager.h"

ESPAsync_WMParameter  custom_sleep("SleepMode", "Sleep Mode", SLEEP_TIME, 20);
ESPAsync_WiFiManager wifiManager(&server, &dns);
#endif

Ticker wifiReconnectTimer;

void connectToWifi() {
  DUMPSLN("Wi-Fi ...");
  WiFi.mode(WIFI_STA);
  #if WIFIMANAGER
    DUMPSLN("Connecting to WifiManager...");
    wifiManager.addParameter(&custom_sleep);
  
    bool wifiManagerConnected = wifiManager.autoConnect(WIFI_HOST);
  
    if ( wifiManagerConnected ) {
      DUMPSLN("Async WifiManager On");
      
      // Configures static IP address
      if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARYDNS, SECONDARYDNS)) {
        DUMPSLN("STA Failed to configure");
      }
    }
    else {
      DUMPSLN("Configportal running");
    }
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      DUMPSLN("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
  #else
    WiFi.begin( WIFI_SSID, WIFI_PASS );
    if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARYDNS, SECONDARYDNS)) {
      DUMPSLN("STA Failed to configure");
    }
    DUMPS("Connecting to Wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      DUMPS('.');
      delay(1000);
    }
  #endif
  DUMPPRINTLN();
  DUMPLN("Connected to Wifi - Local IP : ", WiFi.localIP());
}
