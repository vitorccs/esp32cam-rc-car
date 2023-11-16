#include <Arduino.h>
#include <WifiHandler.h>
#include <StreamServer.h>
#include <SocketServer.h>
#include <DCMotor.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>
#include <Car.h>

// Replace with your network credentials
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PWD "YOUR_PWD"
#define JOYSTICK_DEBUG true
#define PIN_FRONT_LED 2
#define PIN_CAMERA_LED 4
#define PIN_M1_IN1 14
#define PIN_M1_IN2 15
#define PIN_M2_IN1 12
#define PIN_M2_IN2 13
#define MIN_MOTOR_SPEED 80 // (between 0 to 255)

// Car components
DCMotor motor1(PIN_M1_IN1, PIN_M1_IN2);
DCMotor motor2(PIN_M2_IN1, PIN_M2_IN2);
DigitalLed frontLed(PIN_FRONT_LED);
DigitalLed camLed(PIN_CAMERA_LED);
Car car(motor1, motor2, frontLed, camLed);

// Handlers
WifiHandler wifiHandler;
WebJoystickHandler webJoystickHandler(car);

// Stream and Socket servers
StreamServer streamServer = StreamServer();
SocketServer socketServer = SocketServer();

void setup()
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  car.stop();
  car.setMinAbsSpeed(MIN_MOTOR_SPEED);

  streamServer.init();

  // Wi-Fi connection
  wifiHandler.connect(WIFI_SSID, WIFI_PWD);

  // Start streaming web server
  streamServer.startStream();

  // Set Web Joystick (Web sockets)
  webJoystickHandler.setDebug(JOYSTICK_DEBUG);

  // Start Web Sockets
  CoordsHandlerFunction coordsHandler = [&](JoyCoords coords)
  {
    webJoystickHandler.handle(coords);
  };

  ButtonToggleHandlerFunction buttonAHandler = [&](bool toggle)
  {
    webJoystickHandler.toggleCamLed(toggle);
  };

  ButtonToggleHandlerFunction buttonBHandler = [&](bool toggle)
  {
    webJoystickHandler.toggleFrontLights(toggle);
  };

  socketServer.init(coordsHandler,
                    buttonAHandler,
                    buttonBHandler);
}

void loop()
{
  socketServer.loop();
}
