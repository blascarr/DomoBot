enum modes { UNDEF, RUN, OFF };
enum movement { DISCRETE, CONTINUOUS, TEST };
enum reference { RELATIVE, ABSOLUTE};
enum autonomy { MANUAL, AUTOMATIC, STOP };
enum wheelStatus { FORWARD = 1, BACKWARD = -1, LTURN = 4, RTURN = 5, LEFT = 2, RIGHT = 3, LTURNBACK = 6, RTURNBACK = 7, POWEROFF = 0, UNKNOWN = -2 };

struct statusBot{
  uint8_t power = 100;
  float theta = 0;
  uint8_t dir = 0;
  autonomy autobot = AUTOMATIC;
  modes controller = RUN;
  movement movemode = CONTINUOUS;
  wheelStatus wheels = POWEROFF;
  reference ref = ABSOLUTE;
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
        this->currentStatus.power = 50;
        
        if( automode ){
          this->currentStatus.controller = RUN;
          this->currentStatus.autobot = AUTOMATIC;
        }else{
          this->currentStatus.controller = OFF;
          this->currentStatus.autobot = STOP;
        }
      }

      if (domoJSON.hasOwnProperty("movemode")) {
        bool movemode = (bool) domoJSON["movemode"];
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
        if( mode.indexOf("RUN") == 0 ){
           setStatusMode( RUN );
        }
        if( mode.indexOf("AUTO") == 0 ){
           setStatusMode( RUN );
           this->currentStatus.autobot = AUTOMATIC;
        }
        if( mode.indexOf("MAN") == 0 ){
           setStatusMode( RUN );
           this->currentStatus.autobot = MANUAL;
        }
        if( mode.indexOf("OFF") == 0 ){
           setStatusMode( OFF );
           this->currentStatus.autobot = STOP;
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
    
    #if SERIAL_CONTROL
      void serial_loop(){
        if( millis() - serial_millis >= serial_latency ){
          serial_millis = millis();
          serialController();
        }
      }
      
      void serialController(){
        if( Serial.available() ){
          char data = Serial.read();
          switch( data ){
            case 'A':
              this->currentStatus.autobot = AUTOMATIC;
              this->currentStatus.power = 50;
            break;
            case 'F':
              this->currentStatus.autobot = MANUAL;
              this->currentStatus.movemode = TEST;
              this->currentStatus.wheels = FORWARD;
            break;
            case 'B':
              this->currentStatus.autobot = MANUAL;
              this->currentStatus.movemode = TEST;
              this->currentStatus.wheels = BACKWARD;
            break;
            case 'L':
              this->currentStatus.autobot = MANUAL;
              this->currentStatus.movemode = TEST;
              this->currentStatus.wheels = LEFT;
            break;
            case 'R':
              this->currentStatus.autobot = MANUAL;
              this->currentStatus.movemode = TEST;
              this->currentStatus.wheels = RIGHT;
            break;
            case 'S':
              this->currentStatus.autobot = STOP;
            break;
            case 'P':
              this->currentStatus.power = Serial.parseInt();
              Serial.print("Serial Power : ");Serial.println( this->currentStatus.power );
            break;
            default:
        
            break;
          }
        }
        setDomo();
      }
    #endif
};
