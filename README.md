# Arduino library for the Pololu G2 High-Power Motor Driver breakout board

Version: 0.0.1<br>
Release Date: 2019-23-03<br>
[![Build Status](https://travis-ci.org/photodude/G2MotorDriver.svg?branch=master)](https://travis-ci.org/photodude/G2MotorDriver)<br>
Pololu G2 High-Power Motor Driver breakout board https://www.pololu.com/product/2991<br>
[www.pololu.com](https://www.pololu.com/)

## Summary

This is a library for the Arduino IDE that interfaces with the [Pololu G2 High-Power Motor Driver breakout board](https://www.pololu.com/product/2991).
It makes it simple to drive one brushed DC motor.

The library is highly influenced by the [Pololu Dual VNH5019 Motor Driver Shield](https://github.com/pololu/dual-vnh5019-motor-shield) with much of the code base and readme originating from that library. With that said extensive modifications have been made and many variable renamed to reflect use with the G2 High-Power Motor Driver breakout board

## Supported platforms

This library is designed to work with the Arduino IDE versions 1.6.x or later; we have not tested it with earlier versions.  This library should support any Arduino-compatible board, including the [Pololu A-Star 32U4 controllers](https://www.pololu.com/category/149/a-star-programmable-controllers).

## Getting started

### Hardware

The [Pololu G2 High-Power Motor Driver breakout board](https://www.pololu.com/product/2991) can be purchased from Pololu's website.  Before continuing, careful reading of the product page is recommended.

### Software

If you are using version 1.6.2 or later of the [Arduino software (IDE)](https://www.arduino.cc/en/Main/Software), you can use the Library Manager to install this library:

1. In the Arduino IDE, open the "Sketch" menu, select "Include Library", then "Manage Libraries...".
2. Search for "G2MotorDriver".
3. Click the G2MotorDriver entry in the list.
4. Click "Install".

If this does not work, you can manually install the library:

1. Download the [latest release archive from GitHub](https://github.com/photodude/G2MotorDriver/releases) and decompress it.
2. Rename the folder "G2MotorDriver-xxxx" to "G2MotorDriver".
3. Drag the "G2MotorDriver" folder into the "libraries" directory inside your Arduino sketchbook directory.  You can view your sketchbook location by opening the "File" menu and selecting "Preferences" in the Arduino IDE.  If there is not already a "libraries" folder in that location, you should make the folder yourself.
4. After installing the library, restart the Arduino IDE.

## Example

An example sketch is available that shows how to use the library.  You can access it from the Arduino IDE by opening the "File" menu, selecting "Examples", and then selecting "G2MotorDriver".  If
you cannot find these examples, the library was probably installed incorrectly and you should retry the installation instructions above.

### Demo

The demo sketch example ramps the motor from stopped to full speed forward, ramps down
to full speed reverse, and back to stopped. Current readings for the motor are sent over serial and can be seen with the serial monitor.  If a fault is detected, a message is sent over serial.

## Documentation

- `G2MotorDriver()`<br> Default constructor, selects the default pins for the motor driver.
- `G2MotorDriver(
    unsigned char DIR,
    unsigned char PWM,
    unsigned char SLP,
    unsigned char FLT,
    unsigned char CS,
    )` <br>
Alternate constructor for G2 motor driver connections remapped by user. If PWM are remapped, it will try to use analogWrite instead of timer1.
- `void init()` <br> Initialize pinModes and timer1.
- `void setSpeed(int speed)` <br> Set speed and direction for motor.
  Speed should be between -400 and 400.  400 corresponds to motor current flowing from M1A to M1B.  -400 corresponds to motor current flowing from OUTB to OUTA.  0 corresponds to full coast.
- `void setBrake(int brake)` <br> Set brake for motor.  Brake should be between 0 and 400.  0 corresponds to full coast, and 400 corresponds to full brake.
- `unsigned int getCurrentMilliamps()` <br> Returns current reading from motor in milliamps.  See the notes in the "Current readings" section below.
- `unsigned char getFault()` <br> Returns 1 if there is a fault on the G2 motor driver, 0 if no fault.

### Current readings

The current readings returned by `getCurrentMilliamps` will be noisy and unreliable if you are using
a PWM frequency below about 5&nbsp;kHz.  We expect these readings to work fine if you haven't remapped the PWM pins and you are using a board based on the ATmega168, ATmega328P, ATmega328PB, or ATmega32U4, since this library uses 20&nbsp;kHz hardware PWM on those boards.

On other boards, this library uses `analogWrite` to generate PWM signals, which usually means that the PWM frequency will be too low to get reliable current measurements.  If `analogWrite` uses a frequency of 490&nbsp;Hz or more on your board, you can add a 1&nbsp;&micro;F
(or larger) capacitor between each current sense line you are using and GND.  To make `getCurrentMilliamps` work well, you would add the capacitor between CS and GND.

## Version history

* 0.0.1 (2019-23-03): untested alpha release.