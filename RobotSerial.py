import serial
import time

class RobotSerial:
    def __init__(self, port="com3"):
        self.port=port
        self.arduinoSerialData = serial.Serial()
        self.arduinoSerialData.port = "COM3"
        self.arduinoSerialData.baudrate = 115200
        self.arduinoSerialData.timeout = 1
        self.arduinoSerialData.setDTR(False)
        #self.arduinoSerialData.setRTS(False)
        self.arduinoSerialData.open()

    def move(self, left, right):

        self.arduinoSerialData.write(("."+str(left)+":"+str(right)+"\n").encode())
    def brake(self):
        self.arduinoSerialData.write(b".0:0\n")
    def brakeR(self):
        self.arduinoSerialData.write(b".BRAKER\n")
    def brakeL(self):
        self.arduinoSerialData.write(b".BRAKEL\n")
    def arcadeDrive(self, throttle, turn):
        self.arduinoSerialData.write(("."+str(-throttle-turn)+":"+str(throttle+turn)+"\n").encode())
    #def WatchDog(self):
