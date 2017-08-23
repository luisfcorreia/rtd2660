# -*- coding: UTF-8 -*-

import platform
import sys
from flashchips import chips
from i2c import *
from config import port


def sanity_check():
    if sys.version_info < (3, 0):
        print("you must use python3")
        sys.exit()

    if "linux" != platform.system().lower():
        print("we're sorry, you seem NOT be using linux")
        print("there are other tools out there")


"""
    if len(sys.argv) != 2:
        print("two arguments required")
        print("read to file:    'r to_file.bin'")
        print("write from file: 'w from_file.bin'")
"""


def main():
    print("\nrtd2660 alternative programmer\n")

    # test if we're running in a sane system
    sanity_check()

    # the name says it all, detect an arduino for translation to I2C
    if detect_arduino() == "":
        print("no arduino found at " + port)


        # and we're done
    print("\n")


if __name__ == "__main__":
    main()
