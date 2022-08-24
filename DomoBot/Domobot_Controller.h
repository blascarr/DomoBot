class DomoBot{
  public:

    typedef void ( DomoBot::*_f_bot )();
    
    DomoBot( uint16_t motor_config ){
    
    }

    void init(){
      pinMode(IntPin_A, INPUT_PULLUP);
      pinMode(IntPin_B, INPUT_PULLUP);
      pinMode(IntPin_C, INPUT_PULLUP);
      pinMode(IntPin_D, INPUT_PULLUP);
      attachInterrupt(IntPin_A, doEncodeA, CHANGE);
      attachInterrupt(IntPin_A, doEncodeB, CHANGE);
      attachInterrupt(IntPin_C, doEncodeC, CHANGE);
      attachInterrupt(IntPin_D, doEncodeD, CHANGE);
      pinMode (IZQ_PWM, OUTPUT);
      pinMode (IZQ_AVZ, OUTPUT);
      pinMode (IZQ_RET, OUTPUT);
      pinMode (DER_PWM, OUTPUT);
      pinMode (DER_AVZ, OUTPUT);
      pinMode (DER_RET, OUTPUT);
    
      #if defined(ESP32)
        ledcSetup(IZQ_PWM_Ch, PWM_Freq, PWM_Res);
        ledcSetup(DER_PWM_Ch, PWM_Freq, PWM_Res);
        ledcAttachPin(IZQ_PWM, IZQ_PWM_Ch);
        ledcAttachPin(DER_PWM, DER_PWM_Ch);
      #endif
    
      digitalWrite(IZQ_AVZ, LOW);
      digitalWrite(IZQ_RET, LOW);
      digitalWrite(DER_AVZ, LOW);
      digitalWrite(DER_RET, LOW);
      ledcWrite (DER_PWM_Ch, 0);
      ledcWrite (IZQ_PWM_Ch, 0);
    
      sensor.init();
      if (sensor.getLastError())
      {
        Serial.print(F("Failed to initialize OPT3101: error "));
        Serial.println(sensor.getLastError());
        while (1) {}
      }
      sensor.setFrameTiming(256);
      sensor.setChannel(0);
      sensor.setBrightness(OPT3101Brightness::Adaptive);
      sensor.startSample();
    }
};


void esquivaObstaculos ()
{
  if (distances[0] && distances[1] && distances[2] > 300)
  {
    MoverDch ( 1, 0, 1000);
    MoverIzq ( 1, 0, 1000);
  }
  if (distances[0] && distances[1] && distances[2] < 300)
  {
    MoverDch ( 1, 0, 800);
    MoverIzq ( 1, 0, 800);
  }
  if (distances[0] && distances[1] && distances[2] < 200)
  {
    MoverDch ( 1, 0, 600);
    MoverIzq ( 1, 0, 600);
  }
  if (distances[1] < 120  )
  {
    if (distances [0] <= distances [2]) giraDch ();
    else giraIzq ();
  }
  if (distances[0] < 110 )
  {
    MoverIzq ( 1, 0, 600);
  }
  if ( distances[2] < 110 )
  {
    MoverDch ( 1, 0, 600);
  }
}
