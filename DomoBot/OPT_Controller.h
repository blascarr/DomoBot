#if OPTO
  #include "Wire.h"
  #include <OPT3101.h>
  enum OPTICALS { OPTO_LEFT = 0, OPTO_CENTER=1, OPTO_RIGHT= 2};
  enum PASSAGE { DEAD_END = 7, BOTTLENECK=5, FRONTWALL=2, LEFT_CORNER=3, RIGHT_CORNER=6, LEFT_EDGE=1, RIGHT_EDGE=4, FREE = 0, NOREAD = -1 };
  /* BIT PASSAGE
   *    SENSORBIT       PASSAGE NAME        NUMBER
        1 - 1 - 1   --> DEAD_END        -->   7
        1 - 0 - 1   --> BOTTLENECK      -->   5
        0 - 1 - 0   --> FRONTWALL       -->   2
        1 - 1 - 0   --> LEFT_CORNER     -->   3
        0 - 1 - 1   --> RIGHT_CORNER    -->   6
        1 - 0 - 0   --> LEFT_EDGE       -->   1
        0 - 0 - 1   --> RIGHT_EDGE      -->   4
        0 - 0 - 0   --> FREE            -->   0
  */
  
  class lidarEmitter{
    public:
      int16_t distance;   // [mm]
      uint16_t position;
      int16_t amplitude;

      lidarEmitter(){};
      lidarEmitter( uint16_t pos ):position( pos ){  }

      void setPosition( uint16_t pos ){ position = pos;}
      int operator - ( const lidarEmitter& obj ) {
        return ( distance - obj.distance );
      }
  };

  int lidarSort (const void * a, const void * b){
    const lidarEmitter* pa = *( const lidarEmitter ** )a;
    const lidarEmitter* pb = *( const lidarEmitter ** )b;
    return ( pa->distance > pb->distance );
  }
  
  class LIDAR : public OPT3101 {
    public:
      bool disabled = false;
      lidarEmitter emitters[OPTO_SENSORS];
      int16_t amplitudes[OPTO_SENSORS]; 
      int16_t distances[OPTO_SENSORS];
      PASSAGE currentPassage = DEAD_END;

      lidarEmitter *emittersQueue[OPTO_SENSORS];
      
      LIDAR():OPT3101(){}
      
      void begin(){
        init();
        if (getLastError()){
          Serial.print(F("Failed to initialize OPT3101: error "));
          Serial.println( getLastError() );
          disabled = true;
          return;
        }
        
        for (uint8_t i = 0; i < OPTO_SENSORS; i++){
            emitters[i].position = i;
            emittersQueue[i] = &emitters[i];
        }
        
        setFrameTiming(256);
        setChannel(0);
        setBrightness(OPT3101Brightness::Adaptive);
        startSample();
      }
      
      PASSAGE read(){
        if ( isSampleDone() && !disabled ){
          readOutputRegs();
          emitters[ channelUsed ].amplitude = amplitude;
          emitters[ channelUsed ].distance = distanceMillimeters;
          
          if ( channelUsed == 2 ){
            distanceSort();
            #if OPTO_DEBUG
              for (uint8_t i = 0; i < OPTO_SENSORS; i++){
                Serial.print( "\t\t\t" );
                Serial.print( i );
              }
              Serial.println();
              for (uint8_t i = 0; i < OPTO_SENSORS; i++){
                Serial.print( "\t\t\t" );
                Serial.print(emitters[i].position);
                Serial.print( " - " );
                Serial.print(emitters[i].distance);
              }
              Serial.println();
              for (uint8_t i = 0; i < OPTO_SENSORS; i++){
                Serial.print( "\t\t\t" );
                Serial.print(emittersQueue[i]->position);
                Serial.print( " - " );
                Serial.print(emittersQueue[i]->distance);
              }
              Serial.println();
              Serial.println();
            #endif
          }
          nextChannel();
          startSample();
          return getPassage();
        }
        return NOREAD;
      }

      PASSAGE getPassage(){
        int passagenumber = calculatePassage();
        PASSAGE passageList[] = {FREE, LEFT_EDGE, FRONTWALL, LEFT_CORNER, RIGHT_EDGE, BOTTLENECK, RIGHT_CORNER, DEAD_END };
        return passageList[passagenumber];
      }

      int calculatePassage(){
        int passage = 0;
        for (uint8_t i = 0; i < OPTO_SENSORS; i++){
          bool emitterCheck = emitters[i].distance < OPTO_SECURITY_DISTANCE;
          passage += (uint8_t)( emitterCheck )*pow(2,emitters[i].position);
          #if PASSAGE_DEBUG
            Serial.print( emitterCheck );
            Serial.print(" \t\t ");
          #endif
        }
        #if PASSAGE_DEBUG
          Serial.println(passage);
        #endif
        return passage;
      }
      
      void distanceSort(){
          int n = sizeof(emittersQueue) / sizeof(emittersQueue[0]); 
          qsort( emittersQueue, n, sizeof(emittersQueue[0]) , lidarSort);
      }
  };

  
#endif
