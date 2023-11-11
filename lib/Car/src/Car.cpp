#ifndef CAR_H
#define CAR_H
#include <Arduino.h>
#include <DCMotor.h>
#include <DigitalLed.h>
#include <Car.h>

Car::Car(DCMotor &m1,
         DCMotor &m2,
         DigitalLed &fLed) : motor1(m1),
                             motor2(m2),
                             frontLed(fLed)
{
    this->motor1 = m1;
    this->motor2 = m2;
    this->frontLed = fLed;
}

void Car::backward(uint8_t speed)
{
    this->motor1.backward(speed);
    this->motor2.backward(speed);
}

void Car::backwardLeft(uint8_t speed)
{
    this->turn(-1 * speed, -1 * speed * 5 / 10);
}

void Car::backwardRight(uint8_t speed)
{
    this->turn(-1 * speed * 5 / 10, -1 * speed);
}

void Car::forward(uint8_t speed)
{
    this->motor1.forward(speed);
    this->motor2.forward(speed);
}

void Car::forwardLeft(uint8_t speed)
{
    this->turn(speed, speed * 5 / 10);
}

void Car::forwardRight(uint8_t speed)
{
    this->turn(speed * 5 / 10, speed);
}

void Car::turn(int16_t leftSpeed, int16_t rightSpeed)
{
    if (leftSpeed > 0)
    {
        this->motor1.forward(leftSpeed);
    }
    else
    {
        this->motor1.backward(abs(leftSpeed));
    }

    if (rightSpeed > 0)
    {
        this->motor2.forward(rightSpeed);
    }
    else
    {
        this->motor2.backward(abs(rightSpeed));
    }
}

void Car::turnLeft(uint8_t speed)
{
    this->turn(speed, 0);
}

void Car::turnRight(uint8_t speed)
{
    this->turn(0, speed);
}

void Car::frontLightsOff()
{
    this->frontLed.turnOff();
}

void Car::frontLightsOn()
{
    this->frontLed.turnOn();
}

void Car::setMinAbsSpeed(uint8_t absSpeed)
{
    this->motor1.setMinAbsSpeed(absSpeed);
    this->motor2.setMinAbsSpeed(absSpeed);
}

void Car::stop()
{
    this->motor1.stop();
    this->motor2.stop();
}

#endif