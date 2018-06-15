import serial
import time


arduinoSerialData = serial.Serial()
arduinoSerialData.port = "COM3"
arduinoSerialData.baudrate = 115200
arduinoSerialData.timeout = 1
arduinoSerialData.setDTR(False)
#arduinoSerialData.setRTS(False)
arduinoSerialData.open()

arduinoSerialData.write(b".100:100\n")
time.sleep(3)
arduinoSerialData.write(b".0:0\n")
arduinoSerialData.close()
