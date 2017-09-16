# -*- coding: UTF-8 -*-

import serial1

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


class I2C(object):
    def __init__(self, port, speed):


