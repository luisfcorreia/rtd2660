# -*- coding: UTF-8 -*-

import serial
from config import port, speed

CMD_I2C_ADDRESS = 'A'
CMD_I2C_LENGTH = 'L'
CMD_I2C_WRITE_RESTART = 'w'
CMD_I2C_WRITE = 'W'
CMD_I2C_READ_RESTART = 'r'
CMD_I2C_READ = 'R'
CMD_GET_STATE = 'S'
CMD_GET_ERROR = 'E'
CMD_GET_IDENT = 'I'
CMD_GET_ADDRESS = 'a'
CMD_GET_LENGTH = 'l'

STATE_INIT = 0x00
STATE_ERROR = 0x01
STATE_ADDRESS = 0x02
STATE_LENGTH = 0x03
STATE_WRITE = 0x05

# It is somehow misleading that <ESC> is used for RESET
CHAR_RESET = 0x1B

# And "\" is the escape character.
CHAR_ESCAPE = 0x5C

CHAR_ESCAPED_RESET = 0xB1
CHAR_ESCAPED_ESCAPE = 0xC5

ERROR_NONE = 'N'
ERROR_UNESCAPE = 'U'
ERROR_LENGTH = 'L'
ERROR_READ = 'R'
ERROR_WRITEDATA = 'W'
ERROR_SENDDATA = 'S'


def detect_arduino():
    result = ""

    try:
        s = serial.Serial(port, speed, timeout=0.5)
        if s.is_open:
            # s.write(str(CHAR_RESET).encode())

            s.write((CMD_GET_IDENT + "\n").encode())

            blah = s.read()
            print(s.readline())
            for x in range(1, blah):
                print(s.read())

            result = port
            s.close()

    except serial.SerialException:
        pass

    return result


def send_address(addr):
    try:
        s = serial.Serial(port, speed, timeout=0.5)
        s.send(CMD_I2C_ADDRESS)
        s.send(addr)

        result = port
    except serial.SerialException:
        pass
    return result
