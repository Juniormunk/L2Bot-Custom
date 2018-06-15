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

    def Move(self, left, right):
        self.arduinoSerialData.write(b"."+str(left)+":"+str(right)+"\n")
    def Brake(self):
        arduinoSerialData.write(b".0:0\n")
    def BrakeR(self):
        arduinoSerialData.write(b".BRAKER\n")
    def BrakeL(self):
        arduinoSerialData.write(b".BRAKEL\n")
    #def WatchDog(self):
        
