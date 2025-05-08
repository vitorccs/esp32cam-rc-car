# ESP32-CAM RC Car
Remote Controlled Car using ESP32-CAM board (Ai Thinker).

Also published at:
https://www.instructables.com/ESP32-CAM-RC-Car/

<img src="https://github.com/user-attachments/assets/ec22a4e1-2925-4fb7-b9bf-a37df63d4eb8" width="380">
<img src="https://github.com/user-attachments/assets/d911967d-e4df-4131-a469-6d09c001c91d" width="380">
<img src="https://github.com/user-attachments/assets/9c37acfe-ae61-46e8-ac19-afca65610c92" width="380">
<img src="https://github.com/user-attachments/assets/7ff3bae6-3929-4e11-98ef-cb2d855ba641" width="380">

## Recording
Note: enable the sound by clicking on the speaker icon from the video toolbar

https://github.com/user-attachments/assets/16f1a4c5-d164-4bd8-8a95-21b05898c83b

https://github.com/user-attachments/assets/695c0d93-d09e-49f0-810b-0f244609043a


## Description
![Image](https://github.com/user-attachments/assets/4778a96d-b3dc-4130-8555-dee16d9bb4db)

This is a [Platform IO IDE](https://platformio.org/platformio-ide) project coded in C++. 

The ESP32-CAM connects to your Wi-Fi network and provides a Web page containing a virtual joystick.

<img src="https://github.com/vitorccs/esp32cam-rc-car/assets/9891961/ea42572a-f59b-4444-ae26-d2b4ac0d762e" width="300">

_Note: In order to control your car remotely from your public internet IP address, you need to set up Port Forwarding in your home router for ports 80 (Web page), 81 (video streaming) and 82 (web socket/joystick commands)._

## Components
* 01 - ESP32-CAM board
* 01 - Car Chassis (2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 01 - L298N Dual H-Bridge board
* 02 - White Leds
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support
* 01 - Electrolytic Capacitor 1000 μF (16v - 50v)
* 01 - (Optional) Antenna for ESP32-CAM board - improves video streaming and prevents lags
* 01 - (Optional) OV2640 ESP32 CAM (120 or 160 degree lens) - wide-angle capture

_Note: in order to enable the external antenna, it is required to change resistor position in ESP32-CAM board [see tutorial](https://randomnerdtutorials.com/esp32-cam-connect-external-antenna/)._

## About PlatformIO IDE
PlatformIO is a plugin for Microsoft Visual Studio Code. It is a more robust IDE compared to the official Arduino IDE. It also allows us to easily create our own private libraries and use a more object oriented code.

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
#define WIFI_AP_MODE false // Access Point mode (no internet connection)
#define JOYSTICK_DEBUG true
#define PIN_FRONT_LED 2
#define PIN_CAMERA_LED 4
#define PIN_M1_IN1 14
#define PIN_M1_IN2 15
#define PIN_M2_IN1 12
#define PIN_M2_IN2 13
#define MIN_MOTOR_SPEED 80 // (0 to 255)
#define FRAME_SIZE FRAMESIZE_VGA
#define JPEG_QUALITY 25 // (0 to 63) lower means higher quality

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
  if (WIFI_AP_MODE)
  {
    wifiHandler.apMode(WIFI_SSID, WIFI_PWD);
  }
  else
  {
    wifiHandler.connect(WIFI_SSID, WIFI_PWD);
  }

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

Fine-tuning customization can be done in the individual files like `DCMotor.h` for changing speed parameters
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

### Reference 1 (Brazil):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/a79de4b0-ea64-42f6-b933-a801766820ab" width="300">

https://www.robocore.net/robotica-robocore/plataforma-robotica-rocket-tank

### Reference 2 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/6e92b8e4-c452-4b1f-b8ce-b5e51e591bed" width="300">

https://www.aliexpress.us/item/3256801542172576.html

### Reference 3 (International):
<img src="https://github.com/vitorccs/nodemcu-rc-car/assets/9891961/ab262906-3d15-41b7-8fab-b9664d6263a4" width="300">

https://www.aliexpress.us/item/3256805855273192.html

### About the Power Supply
I recommend to use high quality 18650 batteries (3.7v - 4.2v, 2200mAh, at least 2C of discharge rate).

Most people prefer to use different power sources for ESP32-CAM (3.3v or 5v) and Bridge driver (7.4 - 8.4v).

I prefer to have a single power source and thus a single power switch. However, it is recommended to use capacitor to filter the electrical noise created by the bridge.

## Fritzing file
The electronic schematic was created in the [Fritzing](https://fritzing.org/) software and can be downloaded at
[Esp32CamRcCar_v3.zip](https://github.com/user-attachments/files/19996296/Esp32CamRcCar_v3.zip)
