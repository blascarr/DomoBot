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

  #if IMU_ENABLE
    IMU.setGyroOffsets( 220, 76, -85 );
    IMU.calibrateAcc(6);
    IMU.calibrateGyro(6);
    IMU.setAccelOffsets(0,0, 1788); // 1688 factory default for my test chip
    IMU.init();
    bot.setIMU( IMU );
  #endif
    
  bot.init();
  botTicker.attach_ms( time_interval, botloop );
  
  #if SERIAL_CONTROL
    serialTicker.attach_ms( time_interval, serial_loop );
  #endif
  
  #if IMU_ENABLE
    IMUTicker.attach_ms( IMU_time_interval, imu_loop );
  #endif
}

void loop() {
  #if IMU_ENABLE
    IMU.update();
    //IMU_Debug();
  #endif
}
