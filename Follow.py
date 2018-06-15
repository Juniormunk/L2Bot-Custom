
import cv2
import numpy as np
import PID
import RobotSerial
vid = cv2.VideoCapture(0)
vid.set(10,.05)

robot = RobotSerial.RobotSerial()

pidAngle = PID.PID(.25, 0, 0)
pidAngle.SetPoint=320.0
pidAngle.setSampleTime(0.01)


while(True):
    ret, frame = vid.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_green = np.array([50,70,50])
    upper_green = np.array([85,255,255])
    mask = cv2.inRange(hsv, lower_green, upper_green)
    res = cv2.bitwise_and(frame,frame,mask=mask)
    kernel = np.ones((3,3),np.uint8)
    res = cv2.morphologyEx(res,cv2.MORPH_OPEN, kernel)
    im2, contours, hierarchy = cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    for c in contours:
        (x,y,w,h) = cv2.boundingRect(c)
        area = cv2.contourArea(c)
        if area>0:
            hull = cv2.convexHull(c)
            hull_area = cv2.contourArea(hull)
            solidity = float(area)/hull_area
            if h/w >= .4 and h/w <=.6 and solidity>=.8 and h>10 and w>10:
                cv2.rectangle(res, (x,y), (x+w,y+h), (255, 0, 0), 2)
                pidAngle.update(x+(w/2))
                output = 0;
                if pidAngle.output >0:
                    output = pidAngle.output+64
                if pidAngle.output <0:
                    output = pidAngle.output-64
                print (output)
                robot.move(-output,output)
                #left gets neg output right gets output
                
    cv2.imshow('orig',frame)
    cv2.imshow('fff',res)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        arduinoSerialData.close()
        break
vid.release()
cv2.destroyAllWindows()
