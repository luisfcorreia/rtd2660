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

  def SPICommonCommand(self, cmd_type, cmd_code, num_reads, num_writes, write_value):

    num_reads &= 3;
    num_writes &= 3;
    write_value &= 0xFFFFFF;
    reg_value = (cmd_type << 5) | (num_writes << 3) | (num_reads << 1);

    WriteReg(0x60, reg_value);
    WriteReg(0x61, cmd_code);

    if numwrites == 3:
      WriteReg(0x64, write_value >> 16);
      WriteReg(0x65, write_value >> 8);
      WriteReg(0x66, write_value);

    if numwrites == 2:
      WriteReg(0x64, write_value >> 8);
      WriteReg(0x65, write_value);

    if numwrites == 1:
      WriteReg(0x64, write_value);

    WriteReg(0x60, reg_value | 1);
    #Execute the command
    uint8_t  b;
    do
    {
      b = ReadReg(0x60);
    } while (b & 1); #
    #TODO: add timeout and reset the controller
    switch(num_reads)
    {
      case
    0:
    return 0;
    case
    1:
    return ReadReg(0x67);
    case
    2:
    return (ReadReg(0x67) << 8) | ReadReg(0x68);
    case
    3:
    return (ReadReg(0x67) << 16) | (ReadReg(0x68) << 8) | ReadReg(0x69);
    }
    return 0;