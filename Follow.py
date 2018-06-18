
import cv2
import numpy as np
import PID
import RobotSerial
import time
from scipy.interpolate import spline

import matplotlib.pyplot as plt
count=0

vid = cv2.VideoCapture(1)
vid.set(cv2.CAP_PROP_EXPOSURE,-8)



robot = RobotSerial.RobotSerial()

pidAngle = PID.PID(.7, 0,0.005)
pidAngle.SetPoint=320.0
pidAngle.setSampleTime(0.01)

pidDist = PID.PID(.75, 0,0.005)
pidDist.SetPoint=90.0
pidDist.setSampleTime(0.01)

time.sleep(.5)


while(True):
    ret, frame = vid.read()
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    lower_green = np.array([30,100,100])
    upper_green = np.array([90,255,255])
    mask = cv2.inRange(hsv, lower_green, upper_green)
    res = cv2.bitwise_and(frame,frame,mask=mask)
    kernel = np.ones((3,3),np.uint8)
    res = cv2.morphologyEx(res,cv2.MORPH_OPEN, kernel)
    im2, contours, hierarchy = cv2.findContours(mask,cv2.RETR_EXTERNAL,cv2.CHAIN_APPROX_SIMPLE)
    count = 0
    for c in contours:
        (x,y,w,h) = cv2.boundingRect(c)
        area = cv2.contourArea(c)
        if area>5:
            hull = cv2.convexHull(c)
            hull_area = cv2.contourArea(hull)
            solidity = float(area)/hull_area
            if h/w >= .0 and h/w <=.7 and solidity>=.8 and h>10 and w>10:
                count= count+1
                cv2.rectangle(res, (x,y), (x+w,y+h), (255, 0, 0), 2)
                pidAngle.update(x+(w/2))
                output = 0;
                if pidAngle.output>1 or pidAngle.output<-1:
                    if pidAngle.output >0:
                        output = pidAngle.output
                    if pidAngle.output <0:
                        output = pidAngle.output
                    if output<45.5 and output>0:
                        output=45.5
                    else:
                        if output>-45.5and output<0:
                            output=-45.5
                else:
                    output=0;

                pidDist.update(w)
                
                if pidDist.output>1 or pidDist.output<-1:
                    if pidDist.output >0:
                        distoutput = pidDist.output
                    if pidDist.output <0:
                        distoutput = pidDist.output
                    if distoutput<45.5 and distoutput>0:
                        distoutput=45.5
                    else:
                        if distoutput>-45.5 and distoutput<0:
                            distoutput=-45.5
                else:
                    distoutput=0;
                    
                

                
                
                print(output, distoutput)

                robot.move(distoutput-output, distoutput+output)                
#                #left gets neg output right gets output

    if count==0:
        robot.brake()
                
    cv2.imshow('orig',frame)
    cv2.imshow('fff',res)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        robot.brake()
        
        break
vid.release()
cv2.destroyAllWindows()
