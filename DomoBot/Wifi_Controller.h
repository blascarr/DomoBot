#if defined(ESP32)
  #include <WiFi.h>
  #include <mDNS.h>
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
  ESPAsync_WiFiManager wifiManager(&server,&dns);
#endif

Ticker wifiReconnectTimer;

void connectToWifi() {
  Serial.println("Wi-Fi ...");
  WiFi.mode(WIFI_STA);
  #if WIFIMANAGER 
    Serial.println("Connecting to WifiManager...");
    wifiManager.addParameter(&custom_sleep);
    
    bool wifiManagerConnected = wifiManager.autoConnect(WIFI_HOST);
    
    if( wifiManagerConnected ){
        Serial.println("Async WifiManager On");
        Serial.print("Connected to Wifi - Local IP : ");
        // Configures static IP address
        if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARYDNS, SECONDARYDNS)) {
          Serial.println("STA Failed to configure");
        }
    }
    else {
        Serial.println("Configportal running");
    }
    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
      Serial.println("Connection Failed! Rebooting...");
      delay(5000);
      ESP.restart();
    }
  #else
    WiFi.begin( WIFI_SSID, WIFI_PASS );
    if (!WiFi.config(LOCAL_IP, GATEWAY, SUBNET, PRIMARYDNS, SECONDARYDNS)) {
      Serial.println("STA Failed to configure");
    }
    Serial.println("Connecting to Wifi...");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print('.');
      delay(1000);
    }
  #endif

  Serial.println(WiFi.localIP());
}
