#include <Arduino.h>
#include <DigitalLed.h>

DigitalLed::DigitalLed(uint8_t pin)
{
  this->pin = pin;
  pinMode(pin, OUTPUT);
}

void DigitalLed::turnOn()
{
  setState(HIGH);
}

void DigitalLed::turnOff()
{
  setState(LOW);
}

void DigitalLed::setState(uint8_t state)
{
  this->state = state;
  digitalWrite(this->pin, this->state);
}