#include <Ticker.h>
#include <Arduino_JSON.h>

#include "config.h"

#include "Domobot_Controller.h" 
#include "Bot_Config.h" 

#include "OTA_Controller.h"
#include "Server_Controller.h"

#include "Wifi_Controller.h" 
#include "WifiEvents_Controller.h"
 

void setup() {
  Serial.begin(115200);   //Serial para debug en el PC
  
  //onWifiEvents();
  //OTA_init();
  
  connectToWifi();
  initWebServer();
  bot.init();
  botTicker.attach_ms( time_interval, botloop );
}

void loop() {
  
  //  lecturasOPT();
  //  leerBT();
  //  xyAdiferencial();
  //  salidaMotores ();
  //  esquivaObstaculos ();
  //  paroMotores ();

}
