# ESP32-CAM RC Car
Remote Controlled Car using ESP32-CAM board (Ai Thinker). 

<img src="https://github.com/vitorccs/esp32cam-rc-car/assets/9891961/c4210169-0d88-4f1c-a159-0541f533dd56" width="400">

<img src="https://github.com/vitorccs/esp32cam-rc-car/assets/9891961/43eabcb8-0e50-4a8b-b5bc-8e9e2dd9ea60" width="400">


## Description
![Esp32CamRcCar_v2_bb](https://github.com/vitorccs/esp32cam-rc-car/assets/9891961/a4bcdd9c-a1f8-49e0-ada0-388ce839ec51)
_Note: the Electrolytic Capacitor is used to prevent ESP32-CAM board from rebooting when DC Motors consume more current and cause a voltage drop in the circuit._

This is a [Platform IO IDE](https://platformio.org/platformio-ide) project coded in C++. 

The car is controlled by a Virtual Joystick in a Web page. The ESP32-CAM connects to your Wi-Fi network and provides a local IP where the Virtual Joystick can be found. To control your car remotelly with your public internet IP address, you need to set up Port Forwarding in your home router for ports 80 (Web page), 81 (video streaming) and 82 (web socket/joystick commands).

<img src="https://github.com/vitorccs/esp32cam-rc-car/assets/9891961/ea42572a-f59b-4444-ae26-d2b4ac0d762e" width="300">

## Components
* 01 - ESP32-CAM board
* 01 - Car Chassis (2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 01 - L298N Dual H-Bridge board
* 02 - White Leds
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support
* 01 - Mini protoboard (170 holes) - using as board support
* 01 - Antenna for ESP32-CAM board - improves video streaming and prevents lags

## About PlatformIO IDE
Platform IO is a plugin for Microsoft Visual Studio Code. It is a more robust IDE compared to the official Arduino IDE. It also allows us to easily create our own private libraries and use a more object oriented code.

## About the code
The PINs can be customized in the `main.cpp` 
```c++
#include <Arduino.h>
#include <WifiHandler.h>
#include <StreamServer.h>
#include <SocketServer.h>
#include <DCMotor.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>
#include <Car.h>
#include <sensor.h>

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
#define MIN_MOTOR_SPEED 80 // (0 to 255)
#define FRAME_SIZE FRAMESIZE_VGA
#define JPEG_QUALITY 15 // (0 to 63) lower means higher quality

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

  streamServer.init(FRAME_SIZE, JPEG_QUALITY);

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

```

Fine-tuning customizations can be done in the individual files like `DCMotor.h` for changing speed parameters
```c++
#ifndef DCMOTOR_H
#define DCMOTOR_H
#include <Arduino.h>

class DCMotor
{
public:
    DCMotor(uint8_t pinIn1, uint8_t pinIn2);
    void backward(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();

private:
    uint8_t pinIn1;
    uint8_t pinIn2;
    uint8_t absSpeed = 0;
    uint8_t maxAbsSpeed = 255;
    uint8_t minAbsSpeed = 50;
    uint8_t ignoreAbsSpeed = 30;

    void setSpeed(uint8_t speed);
};
#endif
```
## About Car Chassis
This project can work with a 2WD or 4WD car chassis like these ones:

### Reference 1 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/6e92b8e4-c452-4b1f-b8ce-b5e51e591bed" width="300">

https://www.aliexpress.us/item/3256801542172576.html

### Reference 2 (Brazil):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/a79de4b0-ea64-42f6-b933-a801766820ab" width="300">

https://www.robocore.net/robotica-robocore/plataforma-robotica-rocket-tank

### Reference 3 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/ab262906-3d15-41b7-8fab-b9664d6263a4" width="300">

https://www.aliexpress.us/item/3256805855273192.html

### About the Power Supply
I recommend to use high quality 18650 batteries (3.7V and 2200mAh). Their discharge rate must be at least 2C, so they can provide up to 4.4A the circuit.

The 4WD chassis requires a higher power supply, you may need to:
+ Add a second support of two 18650 batteries connected in parallel with the first support to increase the current in the circuit. [Difference between series and parallel](https://www.primaryconnections.org.au/themes/custom/connections/assets/SBR/data/Phy/sub/seriespara/seriespara.htm). 
+ Use LiPo batteries instead, since they have great voltage (7 - 12V) and high discharge current (20 - 30C)
+ You may provide a separate power supply for the ESP32-CAM board

## Fritzing file
The electronic schematic was created in the [Fritzing](https://fritzing.org/) software and can be downloaded at
[Esp32CamRcCar_v2.zip](https://github.com/vitorccs/esp32cam-rc-car/files/13378547/Esp32CamRcCar_v2.zip)

