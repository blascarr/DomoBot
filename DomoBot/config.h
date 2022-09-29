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

// Setup PWM ESP32
const int LEFT_WHEEL_PWM_Ch = 0;
const int RIGHT_WHEEL_PWM_Ch = 2;
const int PWM_Res = 10;
const int PWM_Freq = 20000;

// Pinout Encoder CONFIG
// Encoder 1000 steps = 180mm
#define EncoderPin_A     36  
#define EncoderPin_B     39
#define EncoderPin_C     35
#define EncoderPin_D     34
#define ENCODER_DEBUG false

// Pinout IMU MPU CONFIG
#define IMU_ENABLE false
#define IMU_INTERRUPT_MODE false
#define MPU_INTERRUPT_PIN   4
long IMU_time_interval = 20;
#define IMU_DEBUG false

// Pinout OPT
#define OPTO false
#define OPTO_DEBUG false

//  Variables modo, velocidad y JOYSTICK
int MAX_POWER = 50;

int mode;
int pad_x, pad_y;

//Arrays amplitud y distancia OPT3101
uint16_t amplitudes[3];
int16_t distances[3];

// Constantes para trasladar los datos en bruto del joystick pad_x pad_y
// a velocidades en cada rueda RSpeed LSpeed
const int MIN_RAW_ADC = 0;
const int MAX_RAW_ADC = 100;
const int MAX_SPEED_SETTING =  1023;
const int MIN_SPEED_SETTING = -MAX_SPEED_SETTING;
const int MAX_TURN_DELTA =  512; // velocidad maxima [in/de]crease a girar; Left
const int MIN_TURN_DELTA = -MAX_TURN_DELTA; // Right

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

// STREAM CONFIG
#define STREAM_CHARLENGTH 4

// SLEEP CONFIG
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define SLEEP_TIME_LEN    10
char SLEEP_TIME  [SLEEP_TIME_LEN] = "3000";

long time_stamp;
long time_interval = 10;
