#if IMU_ENABLE
  #include "I2Cdev.h"
  #include "MPU6050_6Axis_MotionApps20.h"
  MPU6050 IMU;
  // MPU control/status vars
  bool dmpReady = false;  // set true if DMP init was successful
  uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
  uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
  uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
  uint16_t fifoCount;     // count of all bytes currently in FIFO
  uint8_t fifoBuffer[64]; // FIFO storage buffer
  
  // orientation/motion vars
  Quaternion q;           // [w, x, y, z]         quaternion container
  VectorInt16 aa;         // [x, y, z]            accel sensor measurements
  VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
  VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
  VectorFloat gravity;    // [x, y, z]            gravity vector
  float euler[3];         // [psi, theta, phi]    Euler angle container
  float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

  #if IMU_INTERRUPT_MODE
    volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
  
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
    
    DUMPSLN("Initializing I2C devices...");
    IMU.initialize();
    
    #if IMU_INTERRUPT_MODE
      pinMode(MPU_INTERRUPT_PIN, INPUT);
    #endif
    
    // Verify connection
    DUMPSLN( "Testing device connections..." );
    
    DUMPSLN( IMU.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed" );
  
    // load and configure the DMP
    DUMPSLN("Initializing DMP...");
    devStatus = IMU.dmpInitialize();
  
    // supply your own gyro offsets here, scaled for min sensitivity
    IMU.setXGyroOffset(220);
    IMU.setYGyroOffset(76);
    IMU.setZGyroOffset(-85);
    IMU.setZAccelOffset(1788); // 1688 factory default for my test chip
  
    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // Calibration Time: generate offsets and calibrate our MPU6050
        IMU.CalibrateAccel(6);
        IMU.CalibrateGyro(6);
        IMU.PrintActiveOffsets();
        // turn on the DMP, now that it's ready
        DUMPSLN("Enabling DMP...");
        IMU.setDMPEnabled(true);
  
        #if IMU_INTERRUPT_MODE
          // enable Arduino interrupt detection
          DUMPLN("Enabling interrupt detection (Arduino external interrupt ",digitalPinToInterrupt(MPU_INTERRUPT_PIN));
          attachInterrupt(digitalPinToInterrupt(MPU_INTERRUPT_PIN), dmpDataReady, RISING);
          mpuIntStatus = IMU.getIntStatus();
  
        #endif
        dmpReady = true;
  
        // get expected DMP packet size for later comparison
        packetSize = IMU.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        DUMPLN("DMP Initialization failed (code ", devStatus);
        switch (devStatus){
          case 1:
            DUMPSLN(") - Initial memory load failed");
          break;
          case 2:
            DUMPSLN(") - DMP configuration updates failed");
          break;
          default:
            DUMPSLN(") - Unknown");
          break;
        }
    }
  }
    void IMU_Debug(){
        DUMP("Qx : ",q.x );
        DUMP("\t Qy : ", q.y );
        DUMP("\t Qz : ", q.z );
        DUMPLN("\t Qw : ", q.w );
    }
    
    void updateIMU(){
          // read a packet from FIFO
          if (!mpuInterrupt) return;
          if (IMU.dmpGetCurrentFIFOPacket(fifoBuffer)) { // Get the Latest packet       
              IMU.dmpGetQuaternion(&q, fifoBuffer);
              IMU.dmpGetGravity(&gravity, &q);
              IMU.dmpGetYawPitchRoll(ypr, &q, &gravity);
              
          }
    }



#endif
