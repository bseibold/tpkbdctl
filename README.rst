What is this?
=============

It’s a tool that can be used to control features of the “Lenovo ThinkPad
USB Keyboard with TrackPoint”. The official User Guide can be found
here: `UserGuide`_ (PDF).

Requirements and Limitations
============================

This tool uses the ``hidraw`` driver and needs at least *Linux 2.6.39*
in order to work. Using ``hidraw`` it can only write settings, not read them,
so you have to set everything at the same time.
It also supports the ``hid-lenovo-tpkbd`` driver which was merged in Linux 3.6.
This driver allows changing the settings via sysfs files, so they can also be read.

On most systems ``/dev/hidraw*`` and ``/sys`` files are writeable only by root, so
you will probably have to run this as root.

Usage
=====

::

    Usage: tpkbdctl [options]

    Options:
      -h, --help            show this help message and exit
      -l, --list            List all available devices
      -d DEVICE, --device=DEVICE
                            Specify device. Format as printed with --list
      -s SENSITIVITY, --sensitivity=SENSITIVITY
                            Set trackpoint sensitivity. Range 1-255
      -S SPEED, --press-speed=SPEED
                            Set press-speed. Range 1-255
      -p ?, --press-to-select=?
                            Enable press-to-select? (y/n)
      -R ?, --press-right=?
                            Enable press-right? (y/n)
      -D ?, --dragging=?    Enable dragging? (y/n)
      -r ?, --release-to-select=?
                            Enable release-to-select? (y/n)

Sensitivity
~~~~~~~~~~~

The higher the sensitivity, the less force you need to move the mouse
cursor.

Press to select
~~~~~~~~~~~~~~~

If this is enabled, pressing down the trackpoint generates a click.

Press-speed
~~~~~~~~~~~

This setting determines how fast you have to press to generate a click
if *Press to select* is enabled.

Dragging
~~~~~~~~

By enabling this, you can not only click by pressing down the
trackpoint, but also drag. This depends on *Press to select* being
enabled.

Press right
~~~~~~~~~~~

This changes the *Press to select* feature to generate right-button
clicks instead of left-button ones.

Release to select
~~~~~~~~~~~~~~~~~

This inaptly named option (the name was adopted from the Windows driver)
enables double-clicking when using *Press to select*.

Building and Installing
=======================

Make sure you have ``pip`` installed. The package is usually called ``python-pip``.
Then run:

::

    sudo pip install tpkbdctl


Permanent Setup
===============

TODO

See also
========

The `kernel patch`_ provides additional functionality. It allows you to control the
LEDs in the mute buttons and makes the microphone mute button usable. It was
merged in Linux 3.6.

License
=======

This tool is licensed under the GNU GPL v2.

.. _UserGuide: http://download.lenovo.com/ibmdl/pub/pc/pccbbs/options_iso/45k1918_ug.pdf
.. _kernel patch: https://github.com/bseibold/linux/branches
