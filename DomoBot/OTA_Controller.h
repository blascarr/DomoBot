#include <ArduinoOTA.h>

void OTA_init(){
    // Port defaults to 8266
    // ArduinoOTA.setPort(8266);
  
    // Hostname defaults to esp8266-[ChipID]
    ArduinoOTA.setHostname(OTA_HOST);
  
    // No authentication by default
    // ArduinoOTA.setPassword("admin");
  
    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
    
   ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    DUMPLN("Start updating ", type);
  });
  ArduinoOTA.onEnd([]() {
    DUMPSLN("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    DUMPF("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    DUMPF("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      DUMPSLN("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      DUMPSLN("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      DUMPSLN("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      DUMPSLN("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      DUMPSLN("End Failed");
    }
  });
  ArduinoOTA.begin();
}
