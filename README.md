# evdipp
[![Build Status](https://travis-ci.org/mlukaszek/evdipp.svg?branch=master)](https://travis-ci.org/mlukaszek/evdipp)

Simple C++ wrapper for [DisplayLink evdi](https://github.com/DisplayLink/evdi) library, with some example client code.
Currently undocumented, but hopefully simple enough to be useful even as-is.

## Contents
This project is a very simple example of how to write a complete client for DisplayLink's evdi/libevdi.

It includes:
- a very thin C++ wrapper for the library, called `libevdipp`, which also includes a simple terminal-based example,
- a simple Qt app which uses evdi to add an additional screen an show its contents in a window.

![twoscreens](https://cloud.githubusercontent.com/assets/4071821/20014459/52100d3a-a2b7-11e6-8b0d-64c4e77245ea.png)

## Building
You need cmake to build. EVDI is defined as external project which will be cloned from git as part of the build.

To build the example terminal app, you need libev/libev++ and Boost.
To build the Qt example, you obviously need Qt 5 dev packages installed.
If you're using recent Ubuntu, you can do it by executing

    sudo apt-get install -y libboost-filesystem-dev libev-dev qtbase5-dev

Then, you can build it by pretty standard chain of commands:

    mkdir build
    cd build
    export DEST=$HOME/target
    cmake -DCMAKE_INSTALL_PREFIX=$DEST -DCMAKE_INSTALL_RPATH=$DEST/lib ..
    make install

## Running
Any client app for EVDI at the moment must be run by root to be able to function - so, using `sudo` is required.

Both example app, and the Qt client require an input file with an EDID of a monitor that they should connect to evdi.
You can get one from any monitor you have (see `/sys/class/drm/*/edid`), or source it from the net - for example from
[Google's autotest project](https://chromium.googlesource.com/chromiumos/third_party/autotest/+/master/server/site_tests/display_Resolution/test_data/edids).

