class WheelController{
  public:
      int PWM_pin, IN1_pin, IN2_pin;
      WheelController( int pwm_pin, int in1_pin, int in2_pin ): PWM_pin( pwm_pin ),IN1_pin( in1_pin ), IN2_pin( in2_pin ) {
        
      }

      void setPins( int pwm_pin, int in1_pin, int in2_pin ){
        PWM_pin = pwm_pin;
        IN1_pin = in1_pin;
        IN2_pin = in2_pin;
      }
      
      void wheel ( bool left_pin, bool right_pin){
        digitalWrite( IN1_pin, left_pin );
        digitalWrite( IN2_pin, right_pin);
      }
      
      void wheel ( int _pin ){
        bool left_pin = false;
        bool right_pin = false;
        if ( _pin > 0 ) left_pin = true;
        if ( _pin < 0 ) right_pin = true;
        wheel( left_pin , right_pin );
      }

      void begin(){
        pinMode (PWM_pin, OUTPUT);
        pinMode (IN1_pin, OUTPUT);
        pinMode (IN2_pin, OUTPUT);
        digitalWrite(IN1_pin, LOW);
        digitalWrite(IN2_pin, LOW);
      }
};
