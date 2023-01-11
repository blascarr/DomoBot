#if IMU_ENABLE
  
  #include "I2Cdev.h"
  #include "Domo_IMU.h"

  #if MPU_INTERRUPT_MODE
    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
  
    static void dmpDataReady() {
        mpuInterrupt = true;
    }
    Domo_IMU IMU( MPU_INTERRUPT_PIN , dmpDataReady );
  #else
    Domo_IMU IMU;
  #endif

  
  /*class IMU : public Domo_IMU{
    public:

      IMU():Domo_IMU(){
        
      }

      IMU( uint16_t interrupt_pin, void (*fn)(), uint8_t address=MPU6050_DEFAULT_ADDRESS ):Domo_IMU( interrupt_pin, fn, address ){
        
      }

      void IMU_Debug(){
            DUMP("Qx : ",q.x );
            DUMP("\t Qy : ", q.y );
            DUMP("\t Qz : ", q.z );
            DUMPLN("\t Qw : ", q.w );
      }
  };*/

      void IMU_Debug(){
            DUMP("Qx : ",IMU.q.x );
            DUMP("\t Qy : ", IMU.q.y );
            DUMP("\t Qz : ", IMU.q.z );
            DUMPLN("\t Qw : ", IMU.q.w );
      }
#endif
