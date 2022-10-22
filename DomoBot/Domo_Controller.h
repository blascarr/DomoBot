enum modes { UNDEF, JOYSTICK, AUTO, STOP, OFF, TEST  };
enum movement { DISCRETE, CONTINUOUS, RELATIVE, ABSOLUTE};
enum autonomy { MANUAL, AUTOMATIC };
enum wheelStatus { FORWARD = 1, BACKWARD = -1, LTURN = 4, RTURN = 5, LEFT = 2, RIGHT = 3, LTURNBACK = 6, RTURNBACK = 7, POWEROFF = 0, UNKNOWN = -2 };

struct statusBot{
  uint8_t power = 100;
  float theta = 0;
  uint8_t dir = 0;
  autonomy autobot = MANUAL;
  modes controller = STOP;
  movement movemode = CONTINUOUS;
  wheelStatus wheels = POWEROFF;
  String mode;
  uint16_t latency;
};

class Domo{
  public:
    statusBot currentStatus;
    AsyncEventSource *eventSource;
    
    long run_millis;
    Domo(){
      
    }
    virtual void setDomo(){};

    void setEventSource( AsyncEventSource &source ){
        eventSource = &source;
    }
      
    void setStatus( String payload ){
      Serial.print("Set status from JSON: ");
      Serial.println( payload );
      JSONVar domoJSON = JSON.parse(payload);
      if (JSON.typeof(domoJSON) == "undefined") {
        Serial.println("Parsing input failed!");
        return;
      }
      
      Serial.print("JSON.typeof(myObject) = ");
      Serial.println(JSON.typeof(domoJSON)); // prints: object
      Serial.println();

      if (domoJSON.hasOwnProperty("power")) {
        this->currentStatus.power = (int) domoJSON["power"];
      }
      if (domoJSON.hasOwnProperty("angle")) {
        this->currentStatus.theta = (double) domoJSON["angle"]["degree"];
      }
         
      if (domoJSON.hasOwnProperty("event")) {
        int len = STREAM_CHARLENGTH;
        char messageArray[len];
        memcpy ( messageArray, (const char*)domoJSON["event"], len );
        String message(messageArray);
        
        setDomoStatus( message );
      }

      if (domoJSON.hasOwnProperty("auto")) {
        bool automode = (bool) domoJSON["auto"];
        if( automode ){
          this->currentStatus.autobot = AUTOMATIC;
          this->currentStatus.controller = AUTO;
        }else{
          this->currentStatus.autobot = MANUAL;
          this->currentStatus.controller = JOYSTICK;
        }
      }

      if (domoJSON.hasOwnProperty("movemode")) {
        bool movemode = (bool) domoJSON["movemode"];
        this->currentStatus.controller = JOYSTICK;
        if( movemode ){
          this->currentStatus.movemode = CONTINUOUS;
        }else{
          this->currentStatus.movemode = DISCRETE;
        }
      }

      if (domoJSON.hasOwnProperty("direction")) {
        int len = 2;
        char messageArray[len];
        memcpy ( messageArray, (const char*)domoJSON["direction"]["angle"], len );
        String message(messageArray);

        if( message.indexOf("up") == 0 ){
            this->currentStatus.dir = 0;
            this->currentStatus.wheels = FORWARD;
        }
        if( message.indexOf("do") == 0 ){
            this->currentStatus.dir = 4;
            this->currentStatus.wheels = BACKWARD;
        }
        if( message.indexOf("le") == 0 ){
            this->currentStatus.dir = 6;
            this->currentStatus.wheels = LEFT;
        }
        if( message.indexOf("ri") == 0 ){
            this->currentStatus.dir = 2;
            this->currentStatus.wheels = RIGHT;
        }
      }

      if (domoJSON.hasOwnProperty("mode")) {
        int len = STREAM_CHARLENGTH;
        char messageArray[len];
        memcpy ( messageArray, (const char*)domoJSON["mode"], len );
        String message(messageArray);

        setDomoStatus( message );
      }

      setDomo();
    }
    
    void setDomoStatus( String mode ){
        if( mode.indexOf("joys") == 0 ){
           setStatusMode( JOYSTICK );
        }
        if( mode.indexOf("STOP") == 0 ){
           setStatusMode( STOP );
           Serial.println("STOP");
        }
        if( mode.indexOf("OFF") == 0 ){
           setStatusMode( OFF );
           Serial.println("OFF");
        }
        if( mode.indexOf("UNDE") == 0 ){
           setStatusMode( UNDEF );
           Serial.println("UNDEF");
        }
    }

    bool setStatusMode( modes status){
      if( this->currentStatus.controller != status ){
        this->currentStatus.controller = status;
        return true;
      }
      return false;
    }
    
};
