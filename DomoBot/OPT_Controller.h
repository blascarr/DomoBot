#if OPTO
  #include "Wire.h"
  #include <OPT3101.h>
  
  OPT3101 sensor;
  
  void lecturasOPT()
  {
    
    if (sensor.isSampleDone()){
      sensor.readOutputRegs();
  
      amplitudes[sensor.channelUsed] = sensor.amplitude;
      distances[sensor.channelUsed] = sensor.distanceMillimeters;
      
      if (sensor.channelUsed == 2){

        #if OPTO_DEBUG
          for (uint8_t i = 0; i < 3; i++){
            Serial.print( i );
            Serial.print(" - OPTO: amplitude - ");
            Serial.print(amplitudes[i]);
            Serial.print(" distance - ");
            Serial.println(distances[i]);
          }
        #endif
      }
      sensor.nextChannel();
      sensor.startSample();
    }
  }
#endif
