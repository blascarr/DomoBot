#include "Domo_Controller.h"
Ticker botTicker;

#include "EncoderStepCounter.h"
#include "Wheel_Controller.h"

EncoderStepCounter Right_Encoder(  EncoderPin_B , EncoderPin_A );
EncoderStepCounter Left_Encoder( EncoderPin_D, EncoderPin_C );

WheelController Left_Wheel( LEFT_WHEEL_PWM , LEFT_WHEEL_IN1 , LEFT_WHEEL_IN2 );
WheelController Right_Wheel( RIGHT_WHEEL_PWM , RIGHT_WHEEL_IN1 , RIGHT_WHEEL_IN2 );

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
      motors.setEncoders( Right_Encoder, Left_Encoder );
      motors.setWheels( Right_Wheel, Left_Wheel );
      motors.init();
      
      attachInterrupt(EncoderPin_A, RightInterrupt , CHANGE);
      attachInterrupt(EncoderPin_B, RightInterrupt , CHANGE);
      attachInterrupt(EncoderPin_C, LeftInterrupt , CHANGE);
      attachInterrupt(EncoderPin_D, LeftInterrupt , CHANGE);
      
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
        calculate_position();
      }
    };

    void setDomo(){
      switch (this->currentStatus.controller) {
        case JOYSTICK:
            if( this->currentStatus.movemode == DISCRETE ){
              if( this->currentStatus.wheels != motors.currentStatus ){
                motors.powerWheels( this->currentStatus.power , this->currentStatus.power );
                
                switch ( this->currentStatus.dir  ){
                  case 0 :
                    motors.wheels( FORWARD );
                  break;
                  case 2 :
                    motors.wheels( RIGHT );
                  break;
                  case 4 :
                    motors.wheels( BACKWARD );
                  break;
                  case 6 :
                    motors.wheels( LEFT );
                  break;
                }
                
                controller = &DomoBot::discreteMovement;
              }
            }
            
            if( this->currentStatus.movemode == CONTINUOUS ){
              controller = &DomoBot::manualMovement;
            }
            if( this->currentStatus.movemode == INCREMENTAL ){
              controller = &DomoBot::incrementalMovement;
            }
            break;
        case STOP:
            controller = &DomoBot::stop;
            break;
        case OFF:
            controller = &DomoBot::stop;
        break;
        default:
        
        break;
      }
    };

    void manualMovement(){
      motors.move( this->currentStatus.power, this->currentStatus.theta );
      calculate_position();
    }

    void discreteMovement(){
      motors.run();
      calculate_position();
    }

    void continuousMovement(){
      motors.move( this->currentStatus.power, this->currentStatus.theta );
      calculate_position();
    }

    void incrementalMovement(){
      motors.move( this->currentStatus.power, this->currentStatus.theta );
      calculate_position();
    }
    
    void stop(){
      motors.move( 0, 0 );
      calculate_position();
      controller = &DomoBot::idle;
    }
    
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
    //MoverDch ( 1, 0, 1000);
    //MoverIzq ( 1, 0, 1000);
  }
  if (distances[0] && distances[1] && distances[2] < 300)
  {
   // MoverDch ( 1, 0, 800);
    //MoverIzq ( 1, 0, 800);
  }
  if (distances[0] && distances[1] && distances[2] < 200)
  {
    //MoverDch ( 1, 0, 600);
    //MoverIzq ( 1, 0, 600);
  }
  if (distances[1] < 120  )
  {
    //if (distances [0] <= distances [2]) giraDch ();
    //else giraIzq ();
  }
  if (distances[0] < 110 )
  {
    //MoverIzq ( 1, 0, 600);
  }
  if ( distances[2] < 110 )
  {
    //MoverDch ( 1, 0, 600);
  }
}
