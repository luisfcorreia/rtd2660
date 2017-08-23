# -*- coding: UTF-8 -*-
from collections import namedtuple

chip = namedtuple("flash", "model jedec size page block")
chips = []
chips.append(chip("AT25DF041A", 0x1F4401, 512, 256, 64))
chips.append(chip("AT25DF161", 0x1F4602, 2 * 1024, 256, 64))
chips.append(chip("AT26DF081A", 0x1F4501, 1 * 1024, 256, 64))
chips.append(chip("AT26DF0161", 0x1F4600, 2 * 1024, 256, 64))
chips.append(chip("AT26DF161A", 0x1F4601, 2 * 1024, 256, 64))
chips.append(chip("AT25DF321", 0x1F4701, 4 * 1024, 256, 64))
chips.append(chip("AT25DF512B", 0x1F6501, 64, 256, 32))
chips.append(chip("AT25DF512B", 0x1F6500, 64, 256, 32))
chips.append(chip("AT25DF021", 0x1F3200, 256, 256, 64))
chips.append(chip("AT26DF641", 0x1F4800, 8 * 1024, 256, 64))
# ST
chips.append(chip("M25P05", 0x202010, 64, 256, 32))
chips.append(chip("M25P10", 0x202011, 128, 256, 32))
chips.append(chip("M25P20", 0x202012, 256, 256, 64))
chips.append(chip("M25P40", 0x202013, 512, 256, 64))
chips.append(chip("M25P80", 0x202014, 1 * 1024, 256, 64))
chips.append(chip("M25P16", 0x202015, 2 * 1024, 256, 64))
chips.append(chip("M25P32", 0x202016, 4 * 1024, 256, 64))
chips.append(chip("M25P64", 0x202017, 8 * 1024, 256, 64))
# Windbond
chips.append(chip("W25X10", 0xEF3011, 128, 256, 64))
chips.append(chip("W25X20", 0xEF3012, 256, 256, 64))
chips.append(chip("W25X40", 0xEF3013, 512, 256, 64))
chips.append(chip("W25X80", 0xEF3014, 1 * 1024, 256, 64))
# Macronix
chips.append(chip("MX25L512", 0xC22010, 64, 256, 64))
chips.append(chip("MX25L3205", 0xC22016, 4 * 1024, 256, 64))
chips.append(chip("MX25L6405", 0xC22017, 8 * 1024, 256, 64))
chips.append(chip("MX25L8005", 0xC22014, 1024, 256, 64))
# Microchip
chips.append(chip("SST25VF512", 0xBF4800, 64, 256, 32))
chips.append(chip("SST25VF032", 0xBF4A00, 4 * 1024, 256, 32))

"""
for x in chips:
    if x.jedec == 0xEF3014:
        print(x)
"""
