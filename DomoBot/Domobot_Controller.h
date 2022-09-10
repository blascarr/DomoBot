#include "Domo_Controller.h"
Ticker botTicker;

#include "EncoderStepCounter.h"

EncoderStepCounter Right_Encoder(  EncoderPin_B , EncoderPin_A );
EncoderStepCounter Left_Encoder( EncoderPin_D, EncoderPin_C );

#include "OPT_Controller.h" 
#include "IMU_Controller.h"

#include "Motor_Controller.h" 
#include "PID_Controller.h"
void RightInterrupt() {
  Right_Encoder.tick();
}

void LeftInterrupt() {
  Left_Encoder.tick();
}

class DomoBot : public Domo {
  public:

    typedef void ( DomoBot::*_f_bot )();
    _f_bot controller = &DomoBot::idle; //Loop Controller Function

    MotorController motors;
    int last_rightPos, last_leftPos;
    long wheelR_position, wheelL_position;
    long bot_position;
    
    #if IMU_ENABLE 
      IMU mpu;
    #endif
    
    #if OPTO
      OPT3101 OPT;
    #endif

    DomoBot( uint16_t motor_config ): Domo(){
    
    }

    void init(){
      #if OPTO || IMU_ENABLE
        Wire.begin();
      #endif
      motors.setRightEncoder( Right_Encoder );
      motors.setLeftEncoder( Left_Encoder );
      motors.init();
      attachInterrupt(EncoderPin_A, RightInterrupt , CHANGE);
      attachInterrupt(EncoderPin_B, RightInterrupt , CHANGE);
      attachInterrupt(EncoderPin_C, LeftInterrupt , CHANGE);
      attachInterrupt(EncoderPin_D, LeftInterrupt , CHANGE);

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
      
      #if IMU_ENABLE 
        mpu.initIMU();
      #endif

      #if OPTO
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
      #endif
    }

    void loop(){
      (this->*controller)();
      
      #if IMU_ENABLE 
        mpu.updateIMU();
      #endif

      #if OPTO
        lecturasOPT();
      #endif
    }

    void idle(){
      if( millis() - run_millis >= this->currentStatus.latency ){
        run_millis = millis();
        //encoder_update();
        calculate_position();
      }
    };

    void setDomo(){
      switch (this->currentStatus.effect) {
        case OFF:

        break;
        default:
        
        break;
      }
    };

    void calculate_position(){
      encoder_update();
      if( last_rightPos != 0 || last_leftPos != 0 ){
        wheelR_position += last_rightPos;
        wheelL_position += last_leftPos;
        Left_Encoder.reset();
        Right_Encoder.reset();

        //Send Position to /map_events endpoint
        eventSource->send(getReadings().c_str(), MAP_STREAM , millis());
        
        #if DOMOBOT_DEBUG
          Serial.print("Right Wheel: ");
          Serial.print( wheelR_position);
          Serial.print(" - Left Wheel: ");
          Serial.print(wheelL_position);
          Serial.print(" -> Current Position: ");
          Serial.println( bot_position );
        #endif
      }
    }

    String getReadings(){
      JSONVar bot_info;
      bot_info["wheelR"] = String( wheelR_position );
      bot_info["wheelL"] = String( wheelL_position );
      return JSON.stringify( bot_info );
    }
    
    void encoder_update(){
        signed char rightPos = Right_Encoder.getPosition();
        signed char leftPos = Left_Encoder.getPosition();
        if( rightPos != last_rightPos || leftPos != last_leftPos ){
          last_rightPos = rightPos;
          last_leftPos = leftPos;
          
          #if ENCODER_DEBUG
            Serial.print("Right Encoder: ");
            Serial.print(rightPos);
            Serial.print(" - Left Encoder: ");
            Serial.println(leftPos);
          #endif
        }
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
