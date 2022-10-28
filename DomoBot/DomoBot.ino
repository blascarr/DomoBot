#include <Ticker.h>
#include <Arduino_JSON.h>

#include "config.h"
#include "Debug_Controller.h"
#include "OTA_Controller.h"
#include "Server_Controller.h"

#include "Domobot_Controller.h" 
#include "Bot_Config.h" 

#include "ServerBot_Controller.h"

#include "Wifi_Controller.h" 
#include "WifiEvents_Controller.h"

#if OPTO
  LIDAR lidar;
#endif

void setup() {
  Serial.begin(115200);   //Serial para debug en el PC
  
  //onWifiEvents();
  //OTA_init();
  
  connectToWifi();
  initWebServer();
  
  bot.setEventSource( botmap_events ); //IMPORTANT TO DEFINE bot event source to avoid crashing
  #if OPTO
    bot.setOpto( lidar );
  #endif
  
  bot.init();
  botTicker.attach_ms( time_interval, botloop );
  
  #if SERIAL_CONTROL
    serialTicker.attach_ms( time_interval, serial_loop );
  #endif
}

void loop() {

}
