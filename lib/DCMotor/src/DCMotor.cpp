#include <Arduino.h>
#include <DCMotor.h>

DCMotor::DCMotor(uint8_t pinIn1,
                 uint8_t pinIn2)
{
    this->pinIn1 = pinIn1;
    this->pinIn2 = pinIn2;

    pinMode(this->pinIn1, OUTPUT);
    pinMode(this->pinIn2, OUTPUT);
}

/** Speed must be between 0 and 100 */
void DCMotor::backward(uint8_t speed)
{
    setSpeed(speed);
    analogWrite(this->pinIn1, this->absSpeed);
    analogWrite(this->pinIn2, 0);
}

/** Speed must be between 0 and 100 */
void DCMotor::forward(uint8_t speed)
{
    setSpeed(speed);
    analogWrite(this->pinIn1, 0);
    analogWrite(this->pinIn2, this->absSpeed);
}

void DCMotor::setSpeed(uint8_t speed)
{
    this->absSpeed = map(speed, 0, 100, 0, this->maxAbsSpeed);

    if (this->absSpeed <= this->ignoreAbsSpeed)
    {
        this->absSpeed = 0;
    }

    if (this->absSpeed > 0 && this->absSpeed <= this->minAbsSpeed)
    {
        this->absSpeed = minAbsSpeed;
    }
}

void DCMotor::setMinAbsSpeed(uint8_t absSpeed)
{
    this->minAbsSpeed = absSpeed;
}

void DCMotor::stop()
{
    setSpeed(0);
    analogWrite(this->pinIn1, 0);
    analogWrite(this->pinIn2, 0);
}