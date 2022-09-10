#include <Ticker.h>
#include <Arduino_JSON.h>

#include "config.h"

#include "OTA_Controller.h"
#include "Server_Controller.h"

#include "Domobot_Controller.h" 
#include "Bot_Config.h" 

#include "ServerBot_Controller.h"

#include "Wifi_Controller.h" 
#include "WifiEvents_Controller.h"
 

void setup() {
  Serial.begin(115200);   //Serial para debug en el PC
  
  //onWifiEvents();
  //OTA_init();
  
  connectToWifi();
  initWebServer();
  
  bot.setEventSource( botmap_events ); //IMPORTANT TO DEFINE bot event source to avoid crashing
  bot.init();
  botTicker.attach_ms( time_interval, botloop );
}

void loop() {
  //events.send(getReadings().c_str(), "indoor_info", millis());
  //  lecturasOPT();
  //  leerBT();
  //  xyAdiferencial();
  //  salidaMotores ();
  //  esquivaObstaculos ();
  //  paroMotores ();

}
