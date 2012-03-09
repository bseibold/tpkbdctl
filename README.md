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
      -D, --dragging           enable dragging
      -r, --release-to-select  enable release-to-select
      -R, --select-with-right  select with right button instead of left


Building and Installing
=======================

    make
    sudo make install

You need Linux headers installed, minimum version 2.6.30.

See also
========

There's a [kernel patch][2] that provides more functionality than this tool.
Be aware though, the unstable branch will be rebased from time to time.

[2]: https://github.com/bseibold/linux/tree/tpkbd-unstable (Kernel patch)

License
=======

This tool is licensed under the GNU GPL v2.

