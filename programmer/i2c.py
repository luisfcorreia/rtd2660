# -*- coding: UTF-8 -*-

import serial

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
        """setup a serial port connection"""
        self.port = port
        self.speed = speed
        self.ser1 = serial.Serial(self.port, self.speed, timeout=0.5, xonxoff=False, rtscts=False, dsrdtr=False)
        if not self.ser1.is_open:
            self.ser1.open()

    def read_stuff(self):
        try:
            dataIn = port.read()
        except serial.SerialException as e:
            # There is no new data from serial port
            return None
        except TypeError as e:
            # Disconnect of USB->UART occured
            self.port.close()
            return None
        else:
            # Some data was received
            return dataIn

    def detect_arduino(self):
        result = ""

        try:
            if self.ser1.is_open:

                self.ser1.write((CMD_GET_IDENT).encode())

                blah = self.ser1.read()

                id = ""
                print(int.from_bytes(blah, byteorder='little', signed=False))
                for x in range(0, int.from_bytes(blah, byteorder='big', signed=False)):
                    id = id + self.ser1.read().decode()

                print(id)

                result = True
                # s.close()

        except serial.SerialException:
            pass
            result = False

        return result

    def send_address(self, addr):
        try:
            self.ser1.send(CMD_I2C_ADDRESS)
            self.ser1.send(addr)

        except serial.SerialException:
            pass
