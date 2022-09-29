
class MotorController{
  public:
      EncoderStepCounter *Right_Encoder;
      EncoderStepCounter *Left_Encoder;
      WheelController *left_wheel;
      WheelController *right_wheel;

      int leftWheel_power = 0;
      int rightWheel_power = 0;
      
      wheelStatus currentStatus = POWEROFF;
      
      typedef void ( MotorController::*_f_motor )( );
      _f_motor controller = &MotorController::idle; //Loop Controller Function
      
      MotorController( ){
        
      }
      
      MotorController( EncoderStepCounter &Right_Enc , EncoderStepCounter &Left_Enc ){
        Right_Encoder = &Right_Enc;
        Left_Encoder = &Left_Enc;
      }
      
      void setEncoders( EncoderStepCounter &Right_Enc, EncoderStepCounter &Left_Enc ){
        Right_Encoder = &Right_Enc;
        Left_Encoder = &Left_Enc;
        Right_Encoder->begin();
        Left_Encoder->begin();
      }

      void setWheels( WheelController &Right_Wheel, WheelController &Left_Wheel ){
        right_wheel = &Right_Wheel;
        left_wheel= &Left_Wheel;
        right_wheel->begin();
        left_wheel->begin();
      }

      void setRightWheel( WheelController &Right_Wheel ){
        right_wheel = &Right_Wheel;
        right_wheel->begin();
      }

      void setLeftWheel( WheelController &Left_Wheel ){
        left_wheel = &Left_Wheel;
        left_wheel->begin();
      }
      
      void setRightEncoder( EncoderStepCounter &Right_Enc ){
        Right_Encoder = &Right_Enc;
        Right_Encoder->begin();
      }

      void setLeftEncoder( EncoderStepCounter &Left_Enc ){
        Left_Encoder = &Left_Enc;
        Left_Encoder->begin();
      }
      
      void init(){
        #if defined(ESP32)
          ledcSetup(LEFT_WHEEL_PWM_Ch, PWM_Freq, PWM_Res);
          ledcSetup(RIGHT_WHEEL_PWM_Ch, PWM_Freq, PWM_Res);
          ledcAttachPin(LEFT_WHEEL_PWM, LEFT_WHEEL_PWM_Ch);
          ledcAttachPin(RIGHT_WHEEL_PWM, RIGHT_WHEEL_PWM_Ch);
          ledcWrite (RIGHT_WHEEL_PWM_Ch, 0);
          ledcWrite (LEFT_WHEEL_PWM_Ch, 0);
        #endif
      }

      void powerWheels( int left_power, int right_power ){
        leftWheel_power = map ( left_power, 0, MAX_POWER, 0, 1023 );
        rightWheel_power = map ( right_power, 0, MAX_POWER, 0, 1023 );
      }
      
      void move( int power, float theta ){
          int analogPower = map ( power, 0, MAX_POWER, 0, 1023 );
          int analogTheta = map ( theta, 0, 360, 0, 1023 );
          
          int left_power = analogPower;
          int right_power = analogPower;
          
          wheels( FORWARD );
      
         #if defined(ESP32)
            ledcWrite (RIGHT_WHEEL_PWM_Ch, right_power);
            ledcWrite (LEFT_WHEEL_PWM_Ch, left_power);
         #endif
      }

      void idle( ){
        
      }

      void wheels( wheelStatus status ){
        if( status != currentStatus){
          currentStatus = status;
          switch (currentStatus){
            case POWEROFF:
              setWheelsDir( 0, 0 );
            break;
            case FORWARD:
              setWheelsDir( 1, 1 );
            break;
            case BACKWARD:
              setWheelsDir( -1, -1 );
            break;
            case LEFT:
              setWheelsDir( -1, 1 );
            break;
            case RIGHT:
              setWheelsDir( 1, -1 );
            break;
            case LTURN:
              setWheelsDir( 0, 1 );
            break;
            case RTURN:
              setWheelsDir( 1, 0 );
            break;
          }
        }
      }

      void setWheelsDir ( int _left_dir , int _right_dir ){
        left_wheel->wheel( _left_dir );
        right_wheel->wheel( _right_dir );
      }

      void setWheelsPower ( int _left_power , int _right_power, bool invert = false ){
        if ( invert ){
          if( (_left_power < 0 && _right_power > 0) or ( _left_power > 0 && _right_power < 0 ) ){
            left_wheel->wheel( - _left_power );
            right_wheel->wheel( - _right_power );
          }else{
            left_wheel->wheel( _left_power );
            right_wheel->wheel( _right_power );
          }
        }else{
          left_wheel->wheel( _left_power );
          right_wheel->wheel( _right_power );
        }
        powerWheels( abs(_left_power), abs(_right_power) );
      }

      void run(){
        (this->*controller)();
        #if defined(ESP32)
            ledcWrite (RIGHT_WHEEL_PWM_Ch, rightWheel_power);
            ledcWrite (LEFT_WHEEL_PWM_Ch, leftWheel_power);
         #endif
      }
};


void xyAdiferencial()
{
  int baseSpeed;
  int turnDelta;

  // velocidad base
  baseSpeed = map ( pad_y, MIN_RAW_ADC, MAX_RAW_ADC, MAX_SPEED_SETTING, MIN_SPEED_SETTING );
  // valor diferencial para girar
  turnDelta = map ( pad_x, MIN_RAW_ADC, MAX_RAW_ADC, MIN_TURN_DELTA, MAX_TURN_DELTA );

  //Lspeed = baseSpeed + turnDelta;
  //Rspeed = baseSpeed - turnDelta;

}
