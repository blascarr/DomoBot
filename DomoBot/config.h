// DOMOBOT SETUP

#define BOT_ID_LEN         16
char DOMOLAMP_ID  [BOT_ID_LEN]   = "D0M0B0T";

// Pinout MOTOR CONFIG
#define RIGHT_WHEEL_PWM      5
#define RIGHT_WHEEL_IN1      18
#define RIGHT_WHEEL_IN2      19
#define LEFT_WHEEL_PWM      23
#define LEFT_WHEEL_IN1      17
#define LEFT_WHEEL_IN2      16

#define DOMOBOT_DEBUG false
#define DOMOBOT_POSE false

// DOMOBOT SPECS [mm]

#define CATERPILLAR_WIDTH   19    //  [mm]
#define DOMOBOT_WIDTH       98    //  [mm]
#define DOMOBOT_LENGTH      86    //  [mm]
#define WHEEL_DIAMETER      39    //  [mm]
#define MOTOR_STEPS         1040  //  [Steps/revolution]
const int WHEELS_DISTANCE = DOMOBOT_WIDTH - CATERPILLAR_WIDTH;

// Setup PWM ESP32
const int LEFT_WHEEL_PWM_Ch = 0;
const int RIGHT_WHEEL_PWM_Ch = 2;
const int PWM_Res = 10;
const int PWM_Freq = 20000;

// Pinout Encoder CONFIG
#define EncoderPin_A     36  
#define EncoderPin_B     39
#define EncoderPin_C     35
#define EncoderPin_D     34
#define ENCODER_DEBUG false

// Pinout IMU MPU CONFIG
#define IMU_ENABLE true
#define IMU_INTERRUPT_MODE true
#define MPU_INTERRUPT_PIN   4
long IMU_time_interval = 20;
#define IMU_DEBUG true

// Pinout OPT
#define OPTO false
#define OPTO_SENSORS 3
int OPTO_SECURITY_DISTANCE = 120;

#define OPTO_DEBUG false
#define PASSAGE_DEBUG false

//  Variables modo, velocidad y JOYSTICK
int MAX_POWER = 50;

int mode;
int pad_x, pad_y;

// Valores para encoder e ISR
const int timeThreshold = 2;
long timeCounter = 0;

volatile long ISRCounter_L = 0;
long counter_L = 0;
bool IsCW_L = true;

volatile long ISRCounter_R = 0;
long counter_R = 0;
bool IsCW_R = true;

// WIFI CONFIG
#define WIFIMANAGER   false

#define WIFI_SSID   "ZMS"
#define WIFI_PASS   "ZM4K3RS:P"
#define OTA_HOST   "DOMO BOT"
#define WIFI_HOST   "DOMO BOT"
#define LOCAL_IP IPAddress(192, 168, 1, 140)
#define GATEWAY IPAddress(192, 168, 1, 1)
#define SUBNET IPAddress(255, 255, 0, 0)
#define PRIMARYDNS IPAddress(9, 9, 9, 9)
#define SECONDARYDNS IPAddress(208, 67, 222, 222)

// SERVER CONFIG
#define SERVER_EVENTS_ENDPOINT "/bot_events"
#define DOMOBOT_ENDPOINT "/domobot"
#define DATA_REQUEST_INPUT "botData"
#define MAP_EVENTS_ENDPOINT "/map_events"
#define MAP_STREAM "map_data"
long server_millis;
long server_latency = 200;

// STREAM CONFIG
#define STREAM_CHARLENGTH 4
#define SERIAL_CONTROL true
long serial_latency = 200;
long serial_millis;
#define SERIAL_DEBUG true
#if SERIAL_DEBUG
    #define SERIALDEBUG Serial
#endif

// SLEEP CONFIG
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define SLEEP_TIME_LEN    10
char SLEEP_TIME  [SLEEP_TIME_LEN] = "3000";

long time_stamp;
long time_interval = 10;
