#ifdef OPTO
  #include <OPT3101.h>
  
  OPT3101 sensor;
  
  void doEncodeA()
  {
    if (millis() > timeCounter + timeThreshold)
    {
      if (digitalRead(IntPin_A) == digitalRead(IntPin_B))
      {
        IsCW_L = true;
        ISRCounter_L++;
      }
      else
      {
        IsCW_L = false;
         ISRCounter_L--;
      }
      timeCounter = millis();
    }
  }
  
  void doEncodeB()
  {
    if (millis() > timeCounter + timeThreshold)
    {
      if (digitalRead(IntPin_A) != digitalRead(IntPin_B))
      {
        IsCW_L = true;
       ISRCounter_L++;
      }
      else
      {
        IsCW_L = false;
       ISRCounter_L--;
      }
      timeCounter = millis();
    }
  }
  
  void doEncodeC()
  {
    if (millis() > timeCounter + timeThreshold)
    {
      if (digitalRead(IntPin_C) == digitalRead(IntPin_D))
      {
        IsCW_R = true;
         ISRCounter_R++;
      }
      else
      {
        IsCW_R = false;
        ISRCounter_R--;
      }
      timeCounter = millis();
    }
  }
  
  void doEncodeD()
  {
    if (millis() > timeCounter + timeThreshold)
    {
      if (digitalRead(IntPin_C) != digitalRead(IntPin_D))
      {
        IsCW_L = true;
         ISRCounter_R++;
      }
      else
      {
        IsCW_R = false;
       ISRCounter_R--;
      }
      timeCounter = millis();
    }
  }
  
  void lecturasOPT()
  {
    if (sensor.isSampleDone())
    {
      sensor.readOutputRegs();
  
      amplitudes[sensor.channelUsed] = sensor.amplitude;
      distances[sensor.channelUsed] = sensor.distanceMillimeters;
  
      if (sensor.channelUsed == 2)
      {
  //      SerialBT.print("D- L, C, R: ");
        for (uint8_t i = 0; i < 3; i++)
        {
          // Serial.print(amplitudes[i]);
          //Serial.print(',');
  
  //        SerialBT.print(distances[i]);
  //        SerialBT.print(", ");
        }
  //      SerialBT.println();
      }
      sensor.nextChannel();
      sensor.startSample();
    }
  }
#endif
