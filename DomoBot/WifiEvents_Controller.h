#if defined(ESP8266)

  void onWifiConnect(const WiFiEventStationModeGotIP& event) {
    DUMPSLN("Connected to Wi-Fi.");
  }
  
  void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
    DUMPSLN("Disconnected from Wi-Fi.");
    wifiReconnectTimer.once(3, connectToWifi);
  }
#endif

#if defined(ESP32)
  void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    DUMPSLN("Connected to Wi-Fi.");
  }
  
  void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    DUMPSLN("Disconnected from Wi-Fi.");
    wifiReconnectTimer.once(2, connectToWifi);
  }
#endif

void onWifiEvents() {
  #if defined(ESP8266)
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  #endif
  #if defined(ESP32)
    //WiFi.onEvent(onWifiConnect, SYSTEM_EVENT_STA_CONNECTED);
    //WiFi.onEvent(onWifiDisconnect, SYSTEM_EVENT_STA_DISCONNECTED);
  #endif
}
