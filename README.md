# evdipp
[![Build Status](https://travis-ci.org/mlukaszek/evdipp.svg?branch=master)](https://travis-ci.org/mlukaszek/evdipp)

Simple C++ wrapper for [DisplayLink evdi](https://github.com/DisplayLink/evdi) library, with some example client code.
Currently undocumented, but hopefully simple enough to be useful even as-is.

## Contents
This project is a very simple example of how to write a complete client for DisplayLink's evdi/libevdi.

It includes:
- a very thin C++ wrapper for the library, called `libevdipp`,
- a simple terminal-based example which registers a virtual screen and logs to console,
- a more complete `monitorsim` Qt sample app which uses evdi to add an additional screen an show its contents in a window. This application uses more features of the library than the console client - for example, enables client-side mouse compositing.

![screenshot2021](https://user-images.githubusercontent.com/4071821/103447188-18c8ad00-4c88-11eb-9da2-d9b544018f1a.png)

## Building
You need cmake to build. EVDI is defined as external project which will be cloned from git as part of the build. As with libevdi, you need libdrm-dev package to be installed to be able to compile the library.

Additionally:
- to build the example terminal app, you need libev/libev++.
- to build the Qt example, you obviously need Qt 5 dev packages installed.

If you're using recent Ubuntu, you can do it by executing

    sudo apt-get install -y libev-dev qtbase5-dev libdrm-dev

Then, you can build it by pretty standard chain of commands:

    mkdir build
    cd build
    mkdir stage
    cmake -DCMAKE_INSTALL_PREFIX=stage -DCMAKE_INSTALL_RPATH=stage/lib ..
    make install

## Running
This project assumes you are familiar with what [evdi](https://github.com/DisplayLink/evdi) is, and how to build it for your system. Before attempting to launch any of the samples, make sure `evdi` kernel module is loaded. If you don't see it in the output of `lsmod`, run `modprobe evdi` first.

Any client app for EVDI at the moment must be run by root to be able to function - so, using `sudo` is required. For example, to run the `monitorsim` binary built in the `stage/bin` subdirectory of your `build` folder, use:

    cd stage/bin
    sudo monitorsim

Note that both the `example` app, and the Qt `monitorsim` client expect an input file with an EDID of a monitor that they should connect to evdi (which acts like a virtual graphics adapter that you need to connect to it). They will still run without it, but will use a hard-coded, single mode (800x600) EDID, which will reduce your possibilities - for example, you will not be able to change display modes for the virtual screen as it will only have one mode available.

You can get an EDID from any monitor you have (see `ls -la /sys/class/drm/*/edid`), or source it from the net - for example from
[Google's autotest project](https://chromium.googlesource.com/chromiumos/third_party/autotest/+/master/server/site_tests/display_Resolution/test_data/edids).
Note however that the EDIDs you use must be valid - and many are not. EVDI refuses EDIDs with invalid checksums, connection attempts will fail with `Invalid argument` error reported by the libevdi library.


