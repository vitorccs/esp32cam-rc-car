#include <Arduino.h>
#include <Car.h>
#include <WebJoystickHandler.h>
#include <JoyCoords.h>

WebJoystickHandler::WebJoystickHandler(Car &car) : _car(car)
{
    this->_car = car;
}

void WebJoystickHandler::setDebug(bool enable)
{
    this->debug = enable;
}

void WebJoystickHandler::handle(JoyCoords coords)
{
    const String direction = coords.direction;
    const int16_t speed = coords.speed;

    if (direction == "N")
    {
        debugMovement(coords, "Forward");
        this->_car.forward(speed);
        return;
    }

    if (direction == "S")
    {
        debugMovement(coords, "Backward");
        this->_car.backward(speed);
        return;
    }

    if (direction == "W")
    {
        debugMovement(coords, "Left");
        this->_car.turn(speed, 0);
        return;
    }

    if (direction == "E")
    {
        debugMovement(coords, "Right");
        this->_car.turn(0, speed);
        return;
    }

    if (direction == "NW")
    {
        debugMovement(coords, "Forward Left");
        this->_car.forwardLeft(speed);
        return;
    }

    if (direction == "NE")
    {
        debugMovement(coords, "Forward Right");
        _car.forwardRight(speed);
        return;
    }

    if (direction == "SW")
    {
        debugMovement(coords, "Backward Left");
        this->_car.backwardLeft(speed);
        return;
    }

    if (direction == "SE")
    {
        debugMovement(coords, "Backward Right");
        this->_car.backwardRight(speed);
        return;
    }

    debugMovement(coords, "Stop");
    this->_car.stop();
}

void WebJoystickHandler::toggleFrontLights(bool enable)
{
    if (enable)
    {
        this->_car.frontLightsOn();
    }
    else
    {
        this->_car.frontLightsOff();
    }
}

void WebJoystickHandler::debugMovement(JoyCoords coords, String direction)
{
    if (!this->debug)
    {
        return;
    }

    Serial.print("(speed: ");
    Serial.print(coords.speed);
    Serial.print(", direction: ");
    Serial.print(coords.direction);
    Serial.println(") " + direction);
}
