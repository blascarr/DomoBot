enum modes { UNDEF, FORWARD, BACKWARD, LTURN, RTURN, LEFT, RIGHT, STOP, OFF };
struct statusBot{
  uint8_t speed = 100;
  modes effect;
  String mode;
  uint16_t latency;
};

class Domo{
  public:
    statusBot currentStatus;
    long run_millis;
    Domo(){
      
    }
    virtual void setDomo(){};
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

      if (domoJSON.hasOwnProperty("effect")) {
        int len = STREAM_CHARLENGTH;
        char messageArray[len];
        memcpy ( messageArray, (const char*)domoJSON["effect"], len );
        String message(messageArray);
        Serial.println(message);
        this->currentStatus.mode = message;
        Serial.println(this->currentStatus.mode);
        setDomoStatus( this->currentStatus.mode );
      }

      setDomo();
    }
    
    void setDomoStatus( String mode ){
        if( mode.indexOf("FORW") == 0 ){
           this->currentStatus.effect = FORWARD;
           Serial.println("FORWARD");
        }
        if( mode.indexOf("BACK") == 0 ){
           this->currentStatus.effect = BACKWARD;
           Serial.println("BACKWARD");
        }
        if( mode.indexOf("LTUR") == 0 ){
           this->currentStatus.effect = LTURN;
           Serial.println("TURNLEFT");
        }
        if( mode.indexOf("RTUR") == 0 ){
           this->currentStatus.effect = RTURN;
           Serial.println("TURNRIGHT");
        }
        if( mode.indexOf("LEFT") == 0 ){
           this->currentStatus.effect = LEFT;
           Serial.println("LEFT");
        }
        if( mode.indexOf("RIGH") == 0 ){
           this->currentStatus.effect = RIGHT;
           Serial.println("RIGHT");
        }
        if( mode.indexOf("STOP") == 0 ){
           this->currentStatus.effect = STOP;
           Serial.println("STOP");
        }
        if( mode.indexOf("OFF") == 0 ){
           this->currentStatus.effect = OFF;
           Serial.println("OFF");
        }
        if( mode.indexOf("UNDE") == 0 ){
           this->currentStatus.effect = UNDEF;
           Serial.println("UNDEF");
        }
    }
    
};
