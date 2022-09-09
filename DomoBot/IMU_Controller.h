#if IMU_ENABLE

  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"

  #if IMU_INTERRUPT_MODE
    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
  #endif
  
  class IMU : public MPU6050{
    public:
    
      // MPU control/status vars
      bool dmpReady = false;  // set true if DMP init was successful
      uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
      uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
      uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
      uint16_t fifoCount;     // count of all bytes currently in FIFO
      uint8_t fifoBuffer[64]; // FIFO storage buffer
      long IMU_millis;
      
      // orientation/motion vars
      Quaternion q;           // [w, x, y, z]         quaternion container
      VectorInt16 aa;         // [x, y, z]            accel sensor measurements
      VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
      VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
      VectorFloat gravity;    // [x, y, z]            gravity vector
      float euler[3];         // [psi, theta, phi]    Euler angle container
      float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

      IMU():MPU6050(){
        
      }
      
      // ================================================================
      // ===               INTERRUPT DETECTION ROUTINE                ===
      // ================================================================
      
      #if IMU_INTERRUPT_MODE
        static void dmpDataReady() {
            mpuInterrupt = true;
        }
      #endif
      
       void initIMU(){
        // Initialize device
          // join I2C bus (I2Cdev library doesn't do this automatically)
          #if IMU_ENABLE && I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
              Wire.begin();
              Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties
          #elif IMU_ENABLE && I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
              Fastwire::setup(400, true);
          #endif
          
          Serial.println(F("Initializing I2C devices..."));
          this->initialize();
          
          #if IMU_INTERRUPT_MODE
            pinMode(MPU_INTERRUPT_PIN, INPUT);
          #endif
          
          // Verify connection
          Serial.println(F("Testing device connections..."));
          Serial.println(this->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
      
          // wait for ready
          Serial.println(F("\nSend any character to begin DMP programming and demo: "));
          while (Serial.available() && Serial.read()); // empty buffer
          while (!Serial.available());                 // wait for data
          while (Serial.available() && Serial.read()); // empty buffer again
      
          // load and configure the DMP
          Serial.println(F("Initializing DMP..."));
          devStatus = this->dmpInitialize();
      
          // supply your own gyro offsets here, scaled for min sensitivity
          this->setXGyroOffset(220);
          this->setYGyroOffset(76);
          this->setZGyroOffset(-85);
          this->setZAccelOffset(1788); // 1688 factory default for my test chip
      
          // make sure it worked (returns 0 if so)
          if (devStatus == 0) {
              // Calibration Time: generate offsets and calibrate our MPU6050
              this->CalibrateAccel(6);
              this->CalibrateGyro(6);
              this->PrintActiveOffsets();
              // turn on the DMP, now that it's ready
              Serial.println(F("Enabling DMP..."));
              this->setDMPEnabled(true);

              #if IMU_INTERRUPT_MODE
                // enable Arduino interrupt detection
                Serial.print(F("Enabling interrupt detection (Arduino external interrupt "));
                Serial.print(digitalPinToInterrupt(MPU_INTERRUPT_PIN));
                Serial.println(F(")..."));
                
                mpuIntStatus = this->getIntStatus();
        
                // set our DMP Ready flag so the main loop() function knows it's okay to use it
                Serial.println(F("DMP ready! Waiting for first interrupt..."));
              #endif
              dmpReady = true;
      
              // get expected DMP packet size for later comparison
              packetSize = this->dmpGetFIFOPacketSize();
          } else {
              // ERROR!
              // 1 = initial memory load failed
              // 2 = DMP configuration updates failed
              // (if it's going to break, usually the code will be 1)
              Serial.print(F("DMP Initialization failed (code "));
              Serial.print(devStatus);
              switch (devStatus){
                case 1:
                  Serial.println(F(") - Initial memory load failed"));
                break;
                case 2:
                  Serial.println(F(") - DMP configuration updates failed"));
                break;
                default:
                  Serial.println(F(") - Unknown"));
                break;
              }
          }
      }
  
      void updateIMU(){
            // read a packet from FIFO
            if( millis() - IMU_millis > IMU_time_interval ){
              IMU_millis = millis();
              if (this->dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet 
                  
                  this->dmpGetQuaternion(&q, fifoBuffer);
                  this->dmpGetGravity(&gravity, &q);
                  this->dmpGetYawPitchRoll(ypr, &q, &gravity);
                  IMU_Debug();
              }
            } 
      }

      void IMU_Debug(){
            Serial.print("Qx : ");
            Serial.print( q.x );
            Serial.print(" Qy : ");
            Serial.print( q.y );
            Serial.print(" Qz : ");
            Serial.print( q.z );
            Serial.print(" Qw : ");
            Serial.println( q.w );
      }
  };

#endif
