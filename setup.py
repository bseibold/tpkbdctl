#!/usr/bin/env python
# -*- coding: utf-8 -*-
# vim:set sw=4 ts=4 et:

from setuptools import setup

setup(name='tpkbdctl',
      version='0.2.3',
      description='Thinkpad Keyboard Control Tool',
      long_description=open('README.rst').read(),
      url='http://github.com/bseibold/tpkbdctl',
      author='Bernhard Seibold',
      author_email='bernhard.seibold@gmail.com',
      license='GPLv2',
      packages=['tpkbdctl'],
      entry_points = {'console_scripts': ['tpkbdctl = tpkbdctl.cmd:main']},
      zip_safe=False)
