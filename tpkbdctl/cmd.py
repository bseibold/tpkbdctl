#!/usr/bin/python
# -*- coding: utf-8 -*-
# vim:set sw=4 ts=4 et:

from __future__ import print_function

import sys
import re
from optparse import OptionParser
from tpkbdctl import TpkbdCtl, HidrawDevice

def parse_choice(val):
    v = val[0].lower()
    if v == 'y' or v == 't' or v == '1':
        return True
    return False


def main():
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


    (options, args) = p.parse_args(sys.argv)


    tpkbdctl = TpkbdCtl()

    if options.device:
        for d in options.device:
            m = re.match('/devices/.*/([0-9a-fA-F]+):17EF:6009\.([0-9a-fA-F]+)$', d)
            if m:
                tpkbdctl.probe_device('%04X:17EF:6009.%04X' % (int(m.group(1), 16), int(m.group(2), 16)))
                continue

            m = re.match('tpkbd:([0-9a-fA-F]+):([0-9a-fA-F]+)', d)
            if m:
                tpkbdctl.probe_device('%04X:17EF:6009.%04X' % (int(m.group(1), 16), int(m.group(2), 16)))
                continue

            m = re.match('hidraw:(.*)', d)
            if m:
                tpkbdctl.devices.append(HidrawDevice('/dev/hidraw{0:s}'.format(m.group(1)))) # no validation on purpose
                continue

            m = re.match('/dev/hidraw([0-9]+)', d)
            if m:
                tpkbdctl.devices.append(HidrawDevice('/dev/hidraw{0:s}'.format(m.group(1)))) # no validation on purpose
                continue

    else:
        tpkbdctl.find_devices()

    if options.list:
        for d in tpkbdctl.devices:
            print(str(d))
        exit(0)

    if options.sensitivity != None:
        s = options.sensitivity
        if 0 < s <= 255:
            for d in tpkbdctl.devices: d.sensitivity = s
        else:
            print('Sensitivity value out of range, ignored', file=sys.stderr)

    if options.press_speed != None:
        s = options.press_speed
        if 0 < s <= 255:
            for d in tpkbdctl.devices: d.press_speed = s
        else:
            print('Press-speed value out of range, ignored', file=sys.stderr)

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

