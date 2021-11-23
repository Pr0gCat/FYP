from communicate import Car
import cv2
import numpy as np
import time
from aruco.find_aruco import findGround

def main():
    # car = Car()
    # car.init_car()
    # car.set_linefollow_mode()  
    # car.wait_ack()                                                   
    print('Car initialized')

    cap = cv2.VideoCapture(-1)
    TOL = 20
    while True:
        success, img = cap.read()
        if not success:
            continue
        found, id, rot = findGround(img)
        if not found:
            # car.run_speed(100,-100)
            continue
        print(rot)
        # if rot < -10 :
        #     # car.run_speed(100,-100)
        # elif rot > 10:
        #     # car.run_speed(-100,100)
        # else:
        #     car.run_speed(0,0)


    cap.release()

if __name__ == '__main__':
    main()