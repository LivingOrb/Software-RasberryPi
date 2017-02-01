# Living Orb - Software for Raspberry Pi

Contains three parts:

1. `server`: a Node.js app, which runs an "admin" web interface controlling the device;
2. `game`: a C++ app, launched by the web interface, which runs a mode based on sensor inputs and outputs on LEDs;
3. `rotation-calibration`: a C++ app, which sets the sensor-to-device transformation matrix.

## Server

The web interface features an editor ([ACE](https://ace.c9.io/#nav=about)) and remotely controls the device.

The modes are saved and loaded from the `modes` directory. In order to be able

### Installation

Install [Node.js](https://nodejs.org/en/), then in the `server` directory, install dependencies with `npm install`.

Run the server with `node app.js`.

Documentation in progress:

- Setting Wi-Fi
- Securing the web interface

## Game

The app is responsible for taking inputs from sensors and outputting colors on LEDs.

A *mode* binds the inputs to the outputs. This is a [Lua](https://www.lua.org/) script, which can be edited on the web interface.

Documentation in progress:

- API
- Tutorial

### Build

Build system: [scons](http://scons.org/)

Libraries:

1. [adxl345.c](https://github.com/Bloutiouf/adxl345.c)
1. [hmc5883l](https://github.com/Bloutiouf/hmc5883l.c)
1. [lua](https://www.lua.org/)
1. [rpi_ws281x](https://github.com/jgarff/rpi_ws281x)

## License

Copyright 2016 Jonathan Giroux

[CC BY-NC-SA 4.0](http://creativecommons.org/licenses/by-nc-sa/4.0/)
