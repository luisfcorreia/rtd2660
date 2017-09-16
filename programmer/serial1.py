# Serial1 Class
from threading import Thread

class Serial1(Thread):
    def __init__(self, serialPort, serialBaud):
        Thread.__init__(self)
        self.daemon = True
        self.running = True
        self.name = 'Serial Thread'
        self.start()

    def kill(self):
        self.running = False

    def write(self,data):
        try:
            self.port.write(data)

        except:
            pass

    def readline(self):
        buf = ''
        while True:
            char = self.port.read()
            if char == 'B':
                buf = char
            else:
                buf += char

            if char == '\r' or char == '' and buf[:3] == 'BEG' and buf[-4:] == 'END\r':
                return buf

    def run(self):
        try:
            self.port = serial.Serial(serialPort, serialBaud)
            logging.debug('starting')
        except:
            pass

        while True:
            try:
                data = self.readline()
                logging.debug(data)

            except:
                pass

            if not self.running:
                break
