

enum wheelStatus { FWD, BCK, TURNLEFT, TURNRIGHT, TURNL, TURNR, POWEROFF };

class MotorController{
  public:
      EncoderStepCounter *Right_Encoder;
      EncoderStepCounter *Left_Encoder;
      WheelController *left_wheel;
      WheelController *right_wheel;
      
      wheelStatus currentStatus = POWEROFF;
      
      //_f_motor controller = &MotorController::wheels; //Loop Controller Function
      
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
        //attachInterrupt(EncoderPin_A, Right_Encoder->tick() , CHANGE);
        //attachInterrupt(EncoderPin_B, Right_Encoder->tick() , CHANGE);
        //attachInterrupt(IntPin_B, interrupt, CHANGE);
      }
      
      void move( int power, float theta ){
        Serial.print("MOVE: ");
          int analogPower = map ( power, 0, MAX_POWER, 0, 1023 );
          int analogTheta = map ( theta, 0, 360, 0, 1023 );
          Lspeed = analogPower;
          Rspeed = analogPower;
          //Serial.println( analogPower );
          wheels( FWD );
      
         #if defined(ESP32)
            ledcWrite (RIGHT_WHEEL_PWM_Ch, Rspeed);
            ledcWrite (LEFT_WHEEL_PWM_Ch, Lspeed);
         #endif
      }

      void wheels( wheelStatus status ){
        if( status != currentStatus){
          currentStatus = status;
          switch (currentStatus){
            case POWEROFF:
              setWheelsDir( 0, 0 );
            break;
            case FWD:
              setWheelsDir( 1, 1 );
            break;
            case BCK:
              setWheelsDir( -1, -1 );
            break;
            case TURNLEFT:
              setWheelsDir( -1, 1 );
            break;
            case TURNRIGHT:
              setWheelsDir( 1, -1 );
            break;
            case TURNL:
              setWheelsDir( 0, 1 );
            break;
            case TURNR:
              setWheelsDir( 1, 0 );
            break;
          }
        }
      }

      void setWheelsDir ( int _left_dir , int _right_dir ){
        left_wheel->wheel( _left_dir );
        right_wheel->wheel( _right_dir );
      }
};


void MoverDch (bool DerAvz, bool DerRet, int DSpeed){

}
void MoverIzq(bool IzqAvz, bool IzqRet, int ISpeed){

}

void giraDch () {
  MoverDch ( 0, 1, 400);
  MoverIzq ( 1, 0, 400);
  delay (150);
}
void giraIzq () {
  MoverDch ( 1, 0, 400);
  MoverIzq ( 0, 1, 400);
  delay (150);
}
void paroMotores () {
  MoverDch ( 0, 0, 0);
  MoverIzq ( 0, 0, 0);
}

void salidaMotores () {
    if (Lspeed < 0) {

  } else {

  }
  if (Rspeed < 0) {

  } else {

  }
  Lspeed = abs(Lspeed);  if (Lspeed < 30) Lspeed = 0;
  Rspeed = abs(Rspeed);  if (Rspeed < 30) Rspeed = 0;
  Lspeed = constrain (Lspeed, 0, 1023);
  Rspeed = constrain (Rspeed, 0, 1023);

  // Serial.print ("L  "); Serial.print (Lspeed); Serial.print ("R  "); Serial.println(Rspeed);
}

void xyAdiferencial()
{
  int baseSpeed;
  int turnDelta;

  // velocidad base
  baseSpeed = map ( pad_y, MIN_RAW_ADC, MAX_RAW_ADC, MAX_SPEED_SETTING, MIN_SPEED_SETTING );
  // valor diferencial para girar
  turnDelta = map ( pad_x, MIN_RAW_ADC, MAX_RAW_ADC, MIN_TURN_DELTA, MAX_TURN_DELTA );

  Lspeed = baseSpeed + turnDelta;
  Rspeed = baseSpeed - turnDelta;

}
