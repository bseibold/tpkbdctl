What is this?
=============

It's a tool that can be used to control features of the
"Lenovo ThinkPad USB Keyboard with TrackPoint".
The official User Guide can be found [here][1] (PDF).

[1]: http://download.lenovo.com/ibmdl/pub/pc/pccbbs/options_iso/45k1918_ug.pdf "User Guide"

Requirements and Limitations
============================

This tool uses the `hidraw` driver and needs at least *Linux 2.6.30* in order
to work. It can only write settings, not read them, so you have to set
everything at the same same.

On most systems the hidraw device files are readable only by root, so you
might need to run this as root.

Usage
=====

    Usage: tpkbdctl [OPTIONS]
    
      -h, --help               show this
      -V, --version            show version
      -d, --device=?           set device. Autodiscovered if unset
      -v                       increase verbosity
      -s, --sensitivity=?      set sensitivity, range 1-255
      -S, --press-speed=?      set press-speed, range 1-255
      -p, --press-to-select    enable press-to-select
      -R, --press-right        select with right button instead of left
      -D, --dragging           enable dragging
      -r, --release-to-select  enable release-to-select


### Sensitivity
The higher the sensitivity, the less force you need to move the mouse cursor.

### Press to select
If this is enabled, pressing down the trackpoint generates a click.

### Press-speed
This setting determines how fast you have to press to generate a click if
*Press to select* is enabled.

### Dragging
By enabling this, you can not only click by pressing down the trackpoint, but
also drag. This depends on *Press to select* being enabled.

### Press right
This changes to *Press to select* feature to generate right-button clicks
instead of left-button ones.

### Release to select
This inaptly named option (the name was adopted from the Windows driver)
enables double-clicking when using *Press to select*.


Building and Installing
=======================

Debian/Ubuntu
-------------

    fakeroot debian/rules binary
    sudo dpkg -i ../tpkbdctl_*.deb

Others
------

    make
    sudo make install

You need Linux headers installed, minimum version 2.6.39.

Permanent Setup
===============

Debian/Ubuntu
-------------

Just customize `/etc/default/tpkbdctl`.

Others
------

To have your perferred configuration set automatically, a udev rule can be
used. Place the following code in  `/etc/udev/rules.d/10-tpkbdctl.rules`.

    SUBSYSTEM=="hidraw", ATTRS{idVendor}=="17ef", ATTRS{idProduct}=="6009", ACTION=="add", RUN+="/etc/udev/tpkbdctl_runner"

Customize this according to you needs and save it as `/etc/udev/tpkbdctl_runner`.
Don't forget to make it executable.

    #!/bin/sh
    
    /usr/bin/tpkbdctl -d ${DEVNAME} -s 192 # your settings here

See also
========

There's a [kernel patch][2] that provides more functionality than this tool.
Be aware though, the unstable branch will be rebased from time to time.

[2]: https://github.com/bseibold/linux/tree/tpkbd-unstable (Kernel patch)

License
=======

This tool is licensed under the GNU GPL v2.

