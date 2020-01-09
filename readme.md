## OpenHDMicroservice

This code is intended to act as a Mavlink "microservice", using very little system resources
and depending only on external libraries it actually needs to function.

**Note**: currently the power microservice is the only one ready to be used, and only for 
lifepowered/pi power sensors (there's no configuration for in219 yet).

----

## Dependencies

#### For testing on Open.HD images:

Only install the Raspbian packages listed below, the lifepoweredpi library and mavlink-routerd are already included in 
recent Open.HD releases.

#### For Raspbian:

    apt install build-essential git python libboost-dev libboost-program-options-dev libboost-system-dev libasio-dev

    git clone https://github.com/xorbit/LiFePO4wered-Pi.git
    
    cd LiFePO4wered-Pi
    
    ./build.py
    
    sudo ./INSTALL.sh
    
    sudo cp lifepowered-data.h /usr/local/include/
    
    git clone https://github.com/intel/mavlink-router.git
    
    cd mavlink-router
    
    git submodule update --init --recursive
    
    sudo ./autogen.sh && sudo ./configure CFLAGS='-g -O2' \
        --sysconfdir=/etc --localstatedir=/var --libdir=/usr/local/lib64 \
        --prefix=/usr/local 
    
    sudo make install


The `ina219` driver is included in this repo and requires no setup or installation.

A fork of the Mavlink library with custom messages and commands for OpenHD (not finalized, they're subject to change 
at any time) is included in this repo as a submodule, however the generated Mavlink headers are also included directly
in the repo so you don't need to rebuild them.

----

## Build

    git clone https://github.com/OpenHD/OpenHDMicroservice.git
    cd OpenHDMicroservice
    git submodule update --init
    sudo make install

-----

## Use

Make sure mavlink-routerd is running first.

There's a systemd service file for starting the various microservices, which will use the system ID specified
in `/etc/openhd/microservice.conf`:

    systemctl start openhd_microservice@power

Or:

    systemctl start openhd_microservice@gpio

Or:

    systemctl start openhd_microservice@camera


You can also run them directly for testing:

    openhd_microservice --power --sysid 253

That will start the power sensor microservice using system ID 253 (should be different on ground/air/GCS/autopilot).

-----

## Available microservices

Currently there are three: power sensor, camera, and GPIO microservices.

Only the power sensor microservice is fully functional

### Power sensor microservice

Provides power sensor readings, intended to be used to monitor ground station power supply.

Currently LifePO4wered/Pi support is the default, but ina219 support is already in the source code and will
be enabled soon.

### Camera microservice

Currently just used as a demo of custom Mavlink commands (changing brightness and other settings live), but 
will be expanded to support retrieving h264 bytestream from the camera and sending the data to stdout for 
tx_rawsock to use.

### GPIO microservice

Code for this isn't finished yet, but will allow reading high/low state and toggling GPIOs on the ground or 
air pi, for whatever purpose you can think of.
