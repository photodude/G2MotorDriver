/** 
 * Controlling 2 wire brushed DC motors via PWM
 * Hardware: 1 * Arduino or compatible board
 *         : 1 * G2 Motor Driver
 *         : 1 * Brushed DC Motor
 *         : 1 * 6.5 V to 30 V DC up to 40 Amp. external powersource for the brushed DC motor.
**/

#pragma once

#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || \
    defined(__AVR_ATmega328PB__) || defined (__AVR_ATmega32U4__)
  #define G2MOTORDRIVER_TIMER1_AVAILABLE
#endif

#include <Arduino.h>

/**
 * This is the base class used to represent a connection to a G2 motor driver.  This class
 * provides high-level functions for sending commands to the G2 motor driver and reading
 * data from it.
 **/
class G2MotorDriver
{
    public:
        // CONSTRUCTORS
        // Default pin selection.
        G2MotorDriver();

        // User-defined pin selection.
        G2MotorDriver(
            unsigned char DIRPin, // INA1,
            unsigned char PWMPin,
            unsigned char SLPPin, // Inverted sleep input: SLPPin must be driven logic high to enable the driver
            unsigned char FLTPin, // EN1DIAG1,
            unsigned char CSPin);

        // PUBLIC METHODS
        void init(); // Initialize TIMER 1, set the PWM to 20kHZ.
        void setSpeed(int speed); // Set speed for Motor.
        void setBrake(int brake); // Brake for Motor.
        unsigned int getCurrentMilliamps(int gain); // Get current reading for Motor.
        unsigned int getCurrentReading();
        void calibrateCurrentOffset();
        unsigned char getFault(); // Get fault reading from Motor.
        void flip(boolean flip); // Flip the direction of the speed for M1.
        void Sleep(); // Put the motor driver to sleep
        void Wake(); // Wake up the motor driver

    protected:
        unsigned int _currentOffset;
    
    private:
        unsigned char _DIRPin; //_INA1
        unsigned char _PWMPin;
        unsigned char _SLPPin;
        static const unsigned char _PWM_TIMER1_PIN = 9;
        unsigned char _FLTPin; //_EN1DIAG1
        unsigned char _CSPin;
        static boolean _flip;
};


// Additional prototypes for 18v17 version.
class G2MotorDriver18v17 : public G2MotorDriver
{
    public:
        using G2MotorDriver::G2MotorDriver;
        unsigned int getCurrentMilliamps(); // Get current reading for Motor with preset gain
};

// Additional prototypes for 18v25 version.
class G2MotorDriver18v25 : public G2MotorDriver
{
    public:
        using G2MotorDriver::G2MotorDriver;
        unsigned int getCurrentMilliamps(); // Get current reading for Motor with preset gain
};

// Additional prototypes for 24v13 version.
class G2MotorDriver24v13 : public G2MotorDriver
{
    public:
        using G2MotorDriver::G2MotorDriver;
        unsigned int getCurrentMilliamps(); // Get current reading for Motor with preset gain
};

// Additional prototypes for 24v21 version.
class G2MotorDriver24v21 : public G2MotorDriver
{
    public:
        using G2MotorDriver::G2MotorDriver;
        unsigned int getCurrentMilliamps(); // Get current reading for Motor with preset gain
};
