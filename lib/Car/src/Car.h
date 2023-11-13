#ifndef THECAR_H
#define THECAR_H
#include <Arduino.h>
#include <DCMotor.h>
#include <DigitalLed.h>

class Car
{
public:
    Car(DCMotor &m1,
        DCMotor &m2,
        DigitalLed &fLed,
        DigitalLed &camLed);
    void backward(uint8_t speed = 100);
    void backwardLeft(uint8_t speed = 100);
    void backwardRight(uint8_t speed = 100);
    void forward(uint8_t speed = 100);
    void forwardLeft(uint8_t speed = 100);
    void forwardRight(uint8_t speed = 100);
    void frontLightsOff();
    void frontLightsOn();
    void camLedOn();
    void camLedOff();
    void setMinAbsSpeed(uint8_t absSpeed);
    void stop();
    void turn(int16_t leftSpeed, int16_t rightSpeed);
    void turnLeft(uint8_t speed = 100);
    void turnRight(uint8_t speed = 100);

private:
    DCMotor motor1;
    DCMotor motor2;
    DigitalLed frontLed;
    DigitalLed cameraLed;
};
#endif