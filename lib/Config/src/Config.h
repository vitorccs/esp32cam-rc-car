// WiFi credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PWD"

// Access Point mode 
// + true = creates a WiFi network without internet connection
// + false = connect to an existing WiFi with internet connection
#define WIFI_AP_MODE false

// Customize PINS
#define PIN_FRONT_LED_1 2
#define PIN_FRONT_LED_2 3
#define PIN_CAMERA_LED 4
#define PIN_M1_IN1 14
#define PIN_M1_IN2 15
#define PIN_M2_IN1 12
#define PIN_M2_IN2 13

// Set minimum motor speed (0 to 255)
#define MIN_MOTOR_SPEED 80

// Set JPEG resolution 
// NOTE: higher resolutions affect stream FPS
#define FRAME_SIZE FRAMESIZE_SVGA

// Set JPEG quality (0 to 63 - lower means higher quality)
// NOTE: Higher quality affect stream FPS
#define JPEG_QUALITY 25

// Enable debug
#define JOYSTICK_DEBUG true

// Set camera model
#define CAMERA_MODEL_AI_THINKER
// #define CAMERA_MODEL_WROVER_KIT
// #define CAMERA_MODEL_ESP_EYE
// #define CAMERA_MODEL_ESP32S3_EYE
// #define CAMERA_MODEL_M5STACK_PSRAM
// #define CAMERA_MODEL_M5STACK_V2_PSRAM
// #define CAMERA_MODEL_M5STACK_WIDE
// #define CAMERA_MODEL_M5STACK_ESP32CAM
// #define CAMERA_MODEL_M5STACK_UNITCAM
// #define CAMERA_MODEL_TTGO_T_JOURNAL
// #define CAMERA_MODEL_XIAO_ESP32S3
// #define CAMERA_MODEL_ESP32_CAM_BOARD
// #define CAMERA_MODEL_ESP32S2_CAM_BOARD
// #define CAMERA_MODEL_ESP32S3_CAM_LCD
// #define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3
// #define CAMERA_MODEL_DFRobot_Romeo_ESP32S3