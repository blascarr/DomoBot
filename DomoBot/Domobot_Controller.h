#include "Domo_Controller.h"
Ticker botTicker;

#if SERIAL_CONTROL
  Ticker serialTicker;
#endif

#if IMU_ENABLE
  Ticker IMUTicker;
#endif

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
    float x, y, theta;
    int opto_distance = 300;
    
    #if IMU_ENABLE 
      MPU6050 *mpu;
    #endif
    
    #if OPTO
      LIDAR *OPT;
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
        initIMU();
      #endif

      #if OPTO
        OPT->begin();
      #endif
      setDomo();
    }

    void loop(){
      (this->*controller)();
    }
    
    #if OPTO
        void setOpto( LIDAR &opto ){
            OPT = &opto;
        }
    #endif

    #if IMU_ENABLE 
        void setIMU( MPU6050 &imu ){
            mpu = &imu;
        }
    #endif
    void idle(){
      if( millis() - run_millis >= this->currentStatus.latency ){
        run_millis = millis();
        calculate_position();
      }
    };
    
    void setDomo(){
      switch (this->currentStatus.autobot) {
        case AUTOMATIC:
            controller = &DomoBot::autonomous;
        break;
        case MANUAL:
            if( this->currentStatus.movemode == DISCRETE ){
                motors.powerWheels( this->currentStatus.power , this->currentStatus.power );
                /*
                 * case 0:   // TOP             Status = FORWARD;
                 * case 1:   // TOP_RIGHT       Status = FORWARD-RIGHT;
                 * case 2:   // RIGHT           Status = RIGHT;
                 * case 3:   // BOTTOM_RIGHT    Status = RIGHT;
                 * case 4:   // BOTTOM          Status = BACKWARD;
                 * case 5:   // BOTTOM_LEFT     Status = BACKWARD;
                 * case 6:   // LEFT            Status = LEFT;
                 * case 7:   // TOP_LEFT        Status = LEFT;
                 */
                wheelStatus joystick_directionList[] = { FORWARD, FORWARD, RIGHT, RIGHT, BACKWARD, BACKWARD, LEFT, LEFT };
                motors.setStatus( joystick_directionList[ this->currentStatus.dir ]);
                
                controller = &DomoBot::discreteMovement;
            }
            
            if( this->currentStatus.movemode == CONTINUOUS ){
              float offset = -45;
              float theta_offset = this->currentStatus.theta + offset;
              float power_left = this->currentStatus.power*sin( theta_offset*PI/180 );
              float power_right = this->currentStatus.power*cos( theta_offset*PI/180 );
              motors.setWheelsPower( power_left , power_right , true );
              controller = &DomoBot::continuousMovement;
            }
            if( this->currentStatus.movemode == TEST ){
              motors.powerWheels( this->currentStatus.power , this->currentStatus.power );
              motors.setStatus( this->currentStatus.wheels );
              controller = &DomoBot::testMovement;
            }
            break;
        case STOP:
            controller = &DomoBot::stop;
            break;
        default:
        
        break;
      }
    };

    void discreteMovement(){
      motors.run();
      calculate_position();
    }

    void continuousMovement(){
      motors.run();
      calculate_position();
    }

    void testMovement(){
      motors.run();
      calculate_position();
    }

    void autonomous(){
      #if OPTO
        /*
         * case 0:   // FREE          nextStatus = FORWARD;
         * case 1:   // LEFT_EDGE     nextStatus = RIGHT;
         * case 2:   // FRONTWALL     nextStatus = UNKNOWN;    if( OPT->emittersQueue[1]->position == OPTO_LEFT ){ nextStatus = LEFT;} else nextStatus = RIGHT;
         * case 3:   // LEFT_CORNER   nextStatus = RIGHT;
         * case 4:   // RIGHT_EDGE    nextStatus = LEFT;
         * case 5:   // BOTTLENECK    nextStatus = FORWARD;
         * case 6:   // RIGHT_CORNER  nextStatus = LEFT;
         * case 7:   // DEAD_END      nextStatus = POWEROFF;
         * case -1:   // NOREAD        OPTO SAMPLE NOT READ
         */
         PASSAGE currentPassage = OPT->read();
         if ( currentPassage < 0 ) return;
         wheelStatus passageStatusList[] = { FORWARD, RIGHT, UNKNOWN, RIGHT, LEFT, FORWARD, LEFT, POWEROFF };
         wheelStatus nextStatus = passageStatusList[ currentPassage ];
         
         if ( nextStatus == UNKNOWN ){  // FRONTWALL CASE - Decision taken to move RIGHT OR LEFT
              if( OPT->emittersQueue[1]->position == OPTO_LEFT ){
                nextStatus = LEFT;
              }else{
                nextStatus = RIGHT;
              }
          }
          
          bool statusChange = motors.setStatus( nextStatus );
          
          if( statusChange ){
            motors.move( MAX_POWER );
          }
      #endif
      motors.run();
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

        int diff =  wheelL_position - wheelR_position ;
        int sum =  ( wheelL_position + wheelR_position );
        float v_sum = sum*PI*WHEEL_DIAMETER/MOTOR_STEPS;
        float v_diff = diff*PI*WHEEL_DIAMETER/MOTOR_STEPS;
        
        theta = v_diff/WHEELS_DISTANCE;
        float theta_deg = theta*180/PI;
        
        x = v_sum/2*cos( theta );
        y = v_sum/2*sin( theta );
        
        sendServerPose();
        #if DOMOBOT_POSE
          DUMP("Right Wheel: ", wheelR_position);
          DUMP(" - Left Wheel: ", wheelL_position);
          DUMP(" -> Current Position: X - ", x );
          DUMP("\t Y - ", y );
          DUMPLN("\t O - ", theta_deg );
        #endif
      }
    }

    void sendServerPose(){
      if( millis() - server_millis >= server_latency ){
        server_millis = millis();
        //Send Position to /map_events endpoint
        eventSource->send(getReadings().c_str(), MAP_STREAM , millis());
      }
    }
    
    String getReadings(){
      JSONVar bot_info;
      bot_info["x"] = String( x );
      bot_info["y"] = String( y );
      bot_info["theta"] = String( theta );
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
            DUMP("Right Encoder: ", rightPos);
            DUMPLN(" - Left Encoder: ", leftPos);
          #endif
        }
    }
    void imu_loop(){
      IMU_Debug();
    }
};
