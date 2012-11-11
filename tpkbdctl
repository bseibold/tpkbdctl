#!/usr/bin/python

# -*- coding: utf-8 -*-
# vim:set sw=4 ts=4 et:

from os import listdir, sep, access, W_OK
from os.path import isdir, isfile, islink, realpath, normpath, basename
from os.path import join as join_path
from struct import pack
from fcntl import ioctl
from optparse import OptionParser
import sys
import re


class HidrawDevice(object):

    can_get = False

    def __init__(self, hidraw_dev):
        #if not access(hidraw_dev, W_OK):
        #    raise RuntimeError('No write access. Maybe run as root?')
        self.hidraw_dev = hidraw_dev
        self._sensitivity = 180
        self._press_speed = 180
        self._press_to_select = False
        self._press_right = False
        self._dragging = False
        self._release_to_select = False

    def __repr__(self):
        return '<HidrawDevice "%s">' % self.hidraw_dev

    def __str__(self):
        return 'hidraw:%s (write-only)' % self.hidraw_dev

    def _write_settings(self):
        props = 0
        props |= 0x01 if self._press_to_select else 0x02
        props |= 0x04 if self._dragging else 0x08
        props |= 0x10 if self._release_to_select else 0x20
        props |= 0x80 if self._press_right else 0x40
        with open(self.hidraw_dev, 'w') as fd:
            ioctl(fd, 0xc0054806, pack('BBBBB', 4, props, 3, self._sensitivity, self._press_speed))


    def get_attr(self):
        raise RuntimeError('Cannot get, only set')

    def set_sensitivity(self, value):
        self._sensitivity = value
        self._write_settings()

    def set_press_speed(self, value):
        self._press_speed = value
        self._write_settings()

    def set_press_to_select(self, value):
        self._press_to_select = value
        self._write_settings()

    def set_press_right(self, value):
        self._press_right = value
        self._write_settings()

    def set_dragging(self, value):
        self._dragging = value
        self._write_settings()

    def set_release_to_select(self, value):
        self._release_to_select = value
        self._write_settings()

    sensitivity = property(get_attr, set_sensitivity)
    press_speed = property(get_attr, set_press_speed)
    press_to_select = property(get_attr, set_press_to_select)
    press_right = property(get_attr, set_press_right)
    dragging = property(get_attr, set_dragging)
    release_to_select = property(get_attr, set_release_to_select)



class TpkbdDevice(object):

    can_get = True

    def __init__(self, hid_path):
        #if not access(hid_path, W_OK):
        #    raise RuntimeError('No write access. Maybe run as root?')
        self.hid_path = hid_path

    def __repr__(self):
        return '<TpkbdDevice "%s">' % basename(self.hid_path)

    def __str__(self):
        [a,b,c,d] = re.split(':|\.', basename(self.hid_path))
        return 'tpkbd:%x:%x' % (int(a, 16), int(d, 16))

    def get_sensitivity(self):
        return int(open(join_path(self.hid_path, 'sensitivity')).readline())

    def set_sensitivity(self, value):
        with open(join_path(self.hid_path, 'sensitivity'), 'w') as fd:
            fd.write(str(value))

    def get_press_speed(self):
        return int(open(join_path(self.hid_path, 'press_speed')).readline())

    def set_press_speed(self, value):
        with open(join_path(self.hid_path, 'press_speed'), 'w') as fd:
            fd.write(str(value))

    def get_press_to_select(self):
        return int(open(join_path(self.hid_path, 'press_to_select')).readline()) == 1

    def set_press_to_select(self, value):
        with open(join_path(self.hid_path, 'press_to_select'), 'w') as fd:
            fd.write('1' if value else '0')

    def get_press_right(self):
        return int(open(join_path(self.hid_path, 'press_right')).readline()) == 1

    def set_press_right(self, value):
        with open(join_path(self.hid_path, 'press_right'), 'w') as fd:
            fd.write('1' if value else '0')

    def get_dragging(self):
        return int(open(join_path(self.hid_path, 'dragging')).readline()) == 1

    def set_dragging(self, value):
        with open(join_path(self.hid_path, 'dragging'), 'w') as fd:
            fd.write('1' if value else '0')

    def get_release_to_select(self):
        return int(open(join_path(self.hid_path, 'release_to_select')).readline()) == 1

    def set_release_to_select(self, value):
        with open(join_path(self.hid_path, 'release_to_select'), 'w') as fd:
            fd.write('1' if value else '0')

    sensitivity = property(get_sensitivity, set_sensitivity)
    press_speed = property(get_press_speed, set_press_speed)
    press_to_select = property(get_press_to_select, set_press_to_select)
    press_right = property(get_press_right, set_press_right)
    dragging = property(get_dragging, set_dragging)
    release_to_select = property(get_release_to_select, set_release_to_select)



class TpkbdCtl(object):

    __hid_path__ = '/sys/bus/hid/devices'
    __dev_path__ = '/dev'


    def __init__(self):
        self._check_prereqs()
        self.devices = []


    def _check_prereqs(self):
        pass


    def find_devices(self):
        self.devices = []
        devs = listdir(self.__hid_path__)
        for dev in devs:
            d = self.probe_device(dev)
            if d != None:
                self.devices.append(d)


    def _check_interface(self, dev):
        while not isfile(join_path(dev, 'bInterfaceClass')):
            if dev == '/sys/devices' or len(dev) < 2:
                raise RuntimeError('USB device info not found in sysfs')
            dev = realpath(join_path(dev, '..'))

        
        intclass = open(join_path(dev, 'bInterfaceClass')).readline()
        if not re.match(r'^03$', open(join_path(dev, 'bInterfaceClass')).readline()):
            return False
        if not re.match(r'^01$', open(join_path(dev, 'bInterfaceNumber')).readline()):
            return False

        return True


    def probe_device(self, dev):
        if not re.match(r'^....:17EF:6009\.....$', dev):
            return None

        hid_path = realpath(join_path(self.__hid_path__, dev))
        hidraw_path = join_path(hid_path, 'hidraw')
        if not isdir(hidraw_path):
            return None

        hidraw_name = listdir(hidraw_path)[0]
        if not isdir(join_path(hidraw_path, hidraw_name)):
            return None

        if not self._check_interface(hid_path):
            return None

        #print 'driver: %s' % basename(realpath(join_path(hid_path, 'driver/module')))
        if isfile(join_path(hid_path, 'sensitivity')):
            return TpkbdDevice(hid_path)
        else:
            hidraw_dev = join_path(self.__dev_path__, hidraw_name)
            return HidrawDevice(hidraw_dev)


    def __repr__(self):
        return '<Tpkbdctl>'


def parse_choice(val):
    v = val[0].lower()
    if v == 'y' or v == 't' or v == '1':
        return True
    return False


def tpkbdctl(argv):
    p = OptionParser(usage='usage: %prog [options]')
    p.add_option('-l', '--list', action='store_true', dest='list',
                help='List all available devices')
    p.add_option('-d', '--device', action='append', type='string', dest='device',
                help='Specify device. Format as printed with --list')
    p.add_option('-s', '--sensitivity', action='store', type='int', dest='sensitivity',
                help='Set trackpoint sensitivity. Range 1-255')
    p.add_option('-S', '--press-speed', action='store', type='int', dest='press_speed', metavar='SPEED',
                help='Set press-speed. Range 1-255')
    p.add_option('-p', '--press-to-select', type='string', action='store', dest='press_to_select', metavar='?',
                 help='Enable press-to-select? (y/n)')
    p.add_option('-R', '--press-right', type='string', action='store', dest='press_right', metavar='?',
                 help='Enable press-right? (y/n)')
    p.add_option('-D', '--dragging', type='string', action='store', dest='dragging', metavar='?',
                 help='Enable dragging? (y/n)')
    p.add_option('-r', '--release-to-select', type='string', action='store', dest='release_to_select', metavar='?',
                 help='Enable release-to-select? (y/n)')


    (options, args) = p.parse_args(argv)


    tpkbdctl = TpkbdCtl()

    if options.device:
        for d in options.device:
            m = re.match('tpkbd:([0-9a-fA-F]+):([0-9a-fA-F]+)', d)
            if m:
                tpkbdctl.probe_device('%04X:17EF:6009.%04X' % (int(m.groups(1), 16), int(m.groups(2), 16)))
            
            m = re.match('hidraw:(.*)', d)
            if m:
                tpkbdctl.devices.append(HidrawDevice(str(m.group(1)))) # no validation on purpose
    else:
        tpkbdctl.find_devices()

    if options.list:
        for d in tpkbdctl.devices:
            print str(d)
        exit(0)

    if options.sensitivity != None:
        s = options.sensitivity
        if 0 < s <= 255:
            for d in tpkbdctl.devices: d.sensitivity = s
        else:
            print >>sys.stderr, 'Sensitivity value out of range, ignored'

    if options.press_speed != None:
        s = options.press_speed
        if 0 < s <= 255:
            for d in tpkbdctl.devices: d.press_speed = s
        else:
            print >>sys.stderr, 'Press-speed value out of range, ignored'

    if options.press_to_select:
        v = parse_choice(options.press_to_select)
        for d in tpkbdctl.devices: d.press_to_select = v

    if options.press_right:
        v = parse_choice(options.press_right)
        for d in tpkbdctl.devices: d.press_right = v

    if options.dragging:
        v = parse_choice(options.dragging)
        for d in tpkbdctl.devices: d.dragging = v

    if options.release_to_select:
        v = parse_choice(options.release_to_select)
        for d in tpkbdctl.devices: d.release_to_select = v


if __name__ == '__main__':
    tpkbdctl(sys.argv)
