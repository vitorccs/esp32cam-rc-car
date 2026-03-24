# ESP32-CAM RC Car
Remote-controlled car using ESP32-CAM board (Ai Thinker).

Also published at:
https://www.instructables.com/ESP32-CAM-RC-Car/

<img src="https://github.com/user-attachments/assets/ec22a4e1-2925-4fb7-b9bf-a37df63d4eb8" width="380">
<img src="https://github.com/user-attachments/assets/d911967d-e4df-4131-a469-6d09c001c91d" width="380">
<img src="https://github.com/user-attachments/assets/9c37acfe-ae61-46e8-ac19-afca65610c92" width="380">
<img src="https://github.com/user-attachments/assets/7ff3bae6-3929-4e11-98ef-cb2d855ba641" width="380">

## Recording
Note: enable the sound by clicking on the speaker icon from the video toolbar.

https://github.com/user-attachments/assets/16f1a4c5-d164-4bd8-8a95-21b05898c83b

https://github.com/user-attachments/assets/695c0d93-d09e-49f0-810b-0f244609043a


## Description
Build a Wi-Fi-controlled surveillance car using an ESP32-CAM board. 

The car is controlled by a Web page containing a virtual joystick.

<img width="300" src="https://github.com/user-attachments/assets/0c0fc0e3-b696-42f6-bae1-b432512325a1" />

You can also control the car with a Keyboard. Activate it by pressing the *Enter* key at any time, and press it again to switch back to the virtual joystick. Use the W, A, S, D or arrow keys, along with (L)ed and (F)lash.

<img width="900" src="https://github.com/user-attachments/assets/b2f910d6-1ad3-48d7-b75a-4b1d420e2f14" />

If you are familiar with configuring internet routers and setting port forwarding, you can also control your car remotely!
1) Ensure you assign a static IP.
2) Set up the following port forwarding rules:
* 8000 (web page)
* 8001 (video streaming)
* 8002 (WebSocket / joystick commands).
3) If your ISP changes your public IP frequently, set up a DDNS service (e.g., No-IP)

## Improvements in version v1.1.0
**1) IMPORTANT: Updated port range (8000–8002)**\
*The Web page is now served on port 8000 (!). This change avoids the undesirable behavior of some browsers which usually redirect HTTP to HTTPS. Also, port 80 is usually blocked by many ISP (Internet Service Providers).*

**2) IMPORTANT: Assigned a different PIN to the 2nd LED**\
*Distributes current and avoids exceeding GPIO limits.*

**3) Keyboard control support**\
*Perfect for desktop use.*

**4) Centralized configuration `Config/src/Config.h`**\
*Simplifies customization without modifying core source files*

**5) README updated with DRV8833 version**\
*Includes instructions for a more efficient, higher-speed setup.*

## Schematics

### Option 1 - Easier to build (uses L298N bridge)
I recommend starting with this project because it is easier and quicker to build, and you car will work perfectly well.

<img src="https://github.com/user-attachments/assets/76a8762d-ec05-48bb-81ab-8f1a1016342f" />

### Option 2 - More complex to build (uses DRV8833 bridge)
If you want to upgrade to a more energy-efficient project with motors that reach higher speeds, and you also have more time and experience with soldering.

The DRV8833 uses modern MOSFET transistor and drops around **0.2-0.4v**, while the L298N is based on older bipolar transistor technology and drops around **2-4 V**. This means more voltage reaches the motor, resulting in better performance, less heat, and improved battery efficiency.

<img src="https://github.com/user-attachments/assets/06f11c5d-3a95-4e8c-bf1e-cf66719f1314" />

## Components
* 01 - ESP32-CAM board
* 01 - Car Chassis (2WD or 4WD)
* 02 - DC Motors (3v - 6v)
* 02 - White Leds
* 02 - 18650 batteries (3.7v - 4.2v)
* 01 - Battery support
* 01 - (Optional) Antenna for ESP32-CAM board - improves video streaming and prevents lags
* 01 - (Optional) OV2640 ESP32 CAM (120 or 160 degree lens) - wide-angle capture

### Option 1 (L298N)
* 01 - L298N Dual H-Bridge board
* 01 - 1000 μF capacitor

### Option 2 (DRV8833)
* 01 - DRV8833 Dual H-Bridge board
* 01 - L2596 Bulk Converter
* 02 - 1000 μF capacitor
* 01 - 100 nF capacitor

_Note: in order to enable the external antenna, it is required to change resistor position in ESP32-CAM board [see tutorial](https://randomnerdtutorials.com/esp32-cam-connect-external-antenna/)._

## About PlatformIO IDE
<img width="110" src="https://github.com/user-attachments/assets/9f92d4e1-022f-43d1-85e7-297b0b146c29" />

This project was created on "Visual Studio Code" with "PlatformIO" plugin.

Compared to the official Arduino IDE, this setup offers better dependency management, multi-environment (board) support, and a project structure that promotes modular and object-oriented code.

## About the code
The PINs can be customized in the `Config/src/Config.h` 
```c++
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

// Enable debug (prints car speed and direction in the serial)
#define JOYSTICK_DEBUG true

// Set camera model
#define CAMERA_MODEL_AI_THINKER
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
* [esp32cam-rc-car-v4.zip](https://github.com/user-attachments/files/25831553/esp32cam-rc-car-v4.zip)
