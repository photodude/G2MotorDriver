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
    _PWM = 9;
    _SLP = 4;
    _FLT = 6;
    _CS  = A0;
}

G2MotorDriver::G2MotorDriver(
            unsigned char DIR, //INA1,
            unsigned char PWM,
            unsigned char SLP,
            unsigned char FLT, //EN1DIAG1,
            unsigned char CS)
{
    // Pin Map
    _DIR = DIR;
    _PWM = PWM;
    _SLP = SLP;
    _FLT = FLT;
    _CS  = CS;
}

// Public Methods
void G2MotorDriver::init()
{
    // Define pinMode for the pins
    pinMode(_DIR, OUTPUT);
    pinMode(_PWM, OUTPUT);
    pinMode(_SLP, OUTPUT);
    digitalWrite(_SLP, HIGH); // SLP must be driven logic high to enable the driver.
    pinMode(_FLT, INPUT_PULLUP); // Output is driven low when a fault has occurred INPUT_PULLUP where HIGH means the sensor is off, and LOW means the sensor is on
    pinMode(_CS, INPUT);

    // Set the frequency for timer1.
    #ifdef G2MOTORDRIVER_TIMER1_AVAILABLE
    if (_PWM == _PWM_TIMER1_PIN)
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
    if (_PWM == _PWM_TIMER1_PIN)
    {
        OCR1A = speed;
    }
    else
    {
        analogWrite(_PWM, speed * 51 / 80); // map 400 to 255
    }
    #else
        analogWrite(_PWM, speed * 51 / 80); // map 400 to 255
    #endif

    if (speed == 0)
    {
        digitalWrite(_DIR, LOW); // Make the motor coast no matter which direction it is spinning
                                // DIR is low, current will flow from OUTB to OUTA.
    }
    else if (reverse)
    {
        digitalWrite(_DIR, LOW); // DIR is low, current will flow from OUTB to OUTA.
    }
    else
    {
        digitalWrite(_DIR, HIGH); // DIR is high, current will flow from OUTA to OUTB
    }
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
    if (_PWM == _PWM_TIMER1_PIN)
    {
        OCR1A = brake;
    }
    else
    {
        analogWrite(_PWM, brake * 51 / 80); // map 400 to 255
    }
    #else
        analogWrite(_PWM, brake * 51 / 80); // map 400 to 255
    #endif
}

// Return motor current value in milliamps.
unsigned int G2MotorDriver::getCurrentMilliamps()
{
    // 5V / 1024 ADC counts / 144 mV per A = 34 mA per count
    return analogRead(_CS) * 34;
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
    digitalWrite(_SLP, High); // SLP must be driven logic High to enable the driver and waking it up.
}
