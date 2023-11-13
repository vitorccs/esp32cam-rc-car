#ifndef WEBJOYSTICKHANDLER_H
#define WEBJOYSTICKHANDLER_H
#include <Arduino.h>
#include <Car.h>
#include <JoyCoords.h>

class WebJoystickHandler {
public:
    WebJoystickHandler(Car &car);
    void handle(JoyCoords coords);
    void toggleFrontLights(bool enable);
    void toggleCamLed(bool enable);
    void setDebug(bool enable);
private:
    Car &_car;
    bool debug = false;
    void debugMovement(JoyCoords coords, String direction);
};
#endif