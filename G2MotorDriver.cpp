/**
 * G2MotorDriver.cpp - Library for controlling a G2 MotorDriver.
 * Created by Walt Sorensen, March 23, 2019.
**/

#include "Arduino.h"
#include "G2MotorDriver.h"

// Constructors
G2MotorDriver::G2MotorDriver()
{
    // Pin Map
    _DIR = 2;
    _PWMPin = 9;
    _SLP = 4;
    _FLT = 6;
    _CS  = A0;
}

G2MotorDriver::G2MotorDriver(
            unsigned char DIR, //INA1,
            unsigned char PWMPin,
            unsigned char SLP,
            unsigned char FLT, //EN1DIAG1,
            unsigned char CS)
{
    // Pin Map
    _DIR = DIR;
    _PWMPin = PWMPin;
    _SLP = SLP;
    _FLT = FLT;
    _CS  = CS;
}

// Public Methods
void G2MotorDriver::init()
{
    // Define pinMode for the pins
    pinMode(_DIR, OUTPUT);
    pinMode(_PWMPin, OUTPUT);
    pinMode(_SLP, OUTPUT);
    digitalWrite(_SLP, HIGH); // SLP must be driven logic high to enable the driver.
    pinMode(_FLT, INPUT_PULLUP); // Output is driven low when a fault has occurred INPUT_PULLUP where HIGH means the sensor is off, and LOW means the sensor is on
    pinMode(_CS, INPUT);

    // Set the frequency for timer1.
    #ifdef G2MOTORDRIVER_TIMER1_AVAILABLE
    if (_PWMPin == _PWM_TIMER1_PIN)
    {
        /**
         * Timer 1 configuration
         * prescaler: clockI/O / 1
         * outputs enabled
         * phase-correct PWM
         * top of 400
         * 
         * PWM frequency calculation
         * 16MHz / 1 (prescaler) / 2 (phase-correct) / 400 (top) = 20kHz
        **/
        TCCR1A = 0b10100000;
        TCCR1B = 0b00010001;
        ICR1 = 400;
    }
    #endif
}

// Set speed for motor 1, speed is a number betwenn -400 and 400
void G2MotorDriver::setSpeed(int speed)
{
    unsigned char reverse = 0;

    if (speed < 0)
    {
        speed = -speed;  // Make speed a positive quantity
        reverse = 1;  // Preserve the direction
    }

    if (speed > 400)  // Max PWM dutycycle
        speed = 400;

    #ifdef G2MOTORDRIVER_TIMER1_AVAILABLE
    if (_PWMPin == _PWM_TIMER1_PIN)
    {
        OCR1A = speed;
    }
    else
    {
        analogWrite(_PWMPin, speed * 51 / 80); // map 400 to 255
    }
    #else
        analogWrite(_PWMPin, speed * 51 / 80); // map 400 to 255
    #endif

    if (reverse ^ _flip) // flip if speed was negative or _flip setting is active, but not both
    {
        digitalWrite(_DIR, HIGH); // DIR is high, current will flow from OUTA to OUTB
    }
    else
    {
        digitalWrite(_DIR, LOW); // DIR is low, current will flow from OUTB to OUTA.
    }
}

void G2MotorDriver::flip(boolean flip)
{
    G2MotorDriver::_flip = flip;
}

// Brake motor, brake is a number between 0 and 400
void G2MotorDriver::setBrake(int brake)
{
    // normalize brake
    if (brake < 0)
    {
        brake = -brake;
    }

    if (brake > 400)  // Max brake
        brake = 400;
        digitalWrite(_DIR, LOW);

    #ifdef G2MOTORDRIVER_TIMER1_AVAILABLE
    if (_PWMPin == _PWM_TIMER1_PIN)
    {
        OCR1A = brake;
    }
    else
    {
        analogWrite(_PWMPin, brake * 51 / 80); // map 400 to 255
    }
    #else
        analogWrite(_PWMPin, brake * 51 / 80); // map 400 to 255
    #endif
}

// Set voltage offset of Motor current reading at 0 speed.
void G2MotorDriver::calibrateCurrentOffset()
{
  setSpeed(0);
  Wake();
  delay(1);
  G2MotorDriver::_currentOffset = getCurrentReading();
}

unsigned int G2MotorDriver::getCurrentReading()
{
  return analogRead(_CS);
}

// Return motor current value in milliamps.
unsigned int G2MotorDriver::getCurrentMilliamps(int gain)
{
    /**
      * 5V / 1024 ADC counts / gain mV per A
      * The 24v14, 18v18 and 24v18 results in 244 mA per count.
      * The 18v22 results in 488 mA per count.
    **/
    unsigned int mAPerCount = 5000000/1024/gain;
    int reading = (getCurrentReading() - G2MotorDriver::_currentOffset);

    if (reading > 0)
    {
        return reading * mAPerCount;
    }

    return 0;
}

// Return error status for motor
unsigned char G2MotorDriver::getFault()
{
    return !digitalRead(_FLT);
}

// Put the motor driver to sleep
void G2MotorDriver::Sleep()
{
    digitalWrite(_SLP, LOW); // SLP must be driven logic low to disable the driver putting it to sleep.
}

// Wake up the motor driver
void G2MotorDriver::Wake()
{
    digitalWrite(_SLP, HIGH); // SLP must be driven logic High to enable the driver and waking it up.
}
