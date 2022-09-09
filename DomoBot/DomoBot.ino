#include <Ticker.h>
#include <Arduino_JSON.h>

#include "config.h"

#include "Motor_Controller.h" 
#include "PID_Controller.h"
#include "IMU_Controller.h"

#include "Domobot_Controller.h" 
#include "Bot_Config.h" 

#include "OTA_Controller.h"
#include "Server_Controller.h"

#include "Wifi_Controller.h" 
#include "WifiEvents_Controller.h"
 

void setup() {
  Serial.begin(115200);   //Serial para debug en el PC
  
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
      Wire.begin();
      Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
      Fastwire::setup(400, true);
  #endif

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


  /*if (counter_L != ISRCounter_L || counter_R != ISRCounter_R)
  {
    counter_L = ISRCounter_L;
    counter_R = ISRCounter_R;
    */
    //    SerialBT.print(counter_L);
    //    SerialBT.print (" : ");
    //    SerialBT.println(counter_R);
    // SerialBT.print (" // ");
  //}
}
