#define BOT_ID_LEN         16
char DOMOLAMP_ID  [BOT_ID_LEN]   = "D0M0B0T";

// Pinout CONFIG

#define IntPin_A     36
#define IntPin_B     39
#define IntPin_C     35
#define IntPin_D     34

// Pinout MPU CONFIG
#define MPU_INTERRUPT_PIN   19

// Pinout MOTOR CONFIG
#define IZQ_PWM      27
#define IZQ_AVZ      26
#define IZQ_RET      25
#define DER_PWM      13
#define DER_AVZ      12
#define DER_RET      14

// Setup PWM ESP32
const int DER_PWM_Ch = 0;
const int IZQ_PWM_Ch = 2;
const int PWM_Res = 10;
const int PWM_Freq = 20000;

//  Variables modo, velocidad y JOYSTICK
int mode;
int pad_x, pad_y;
int Lspeed = 0;
int Rspeed = 0;

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
#define DATA_REQUEST_INPUT "botData"

// SLEEP CONFIG
#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define SLEEP_TIME_LEN    10
char SLEEP_TIME  [SLEEP_TIME_LEN] = "3000";

long time_stamp;
long time_interval = 10;
