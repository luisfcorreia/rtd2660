# -*- coding: UTF-8 -*-
import os
import sys
#  arduino port and speed
port = "/dev/ttyUSB0"
speed = 19200

def sanity_check():
    if sys.version_info < (3, 0):
        print("you must use python3")
        sys.exit()

    if os.uname()[1] != 'raspberrypi':
        print("we're sorry, you seem NOT be using a raspberrypi")

"""
    if len(sys.argv) != 2:
        print("two arguments required")
        print("read to file:    'r to_file.bin'")
        print("write from file: 'w from_file.bin'")
"""
