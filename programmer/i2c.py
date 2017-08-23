import serial
from config import port



def detect_arduino():

    result = []

    try:
        s = serial.Serial(port)
        s.close()
        result.append(port)
    except serial.SerialException:
        pass
    return result
