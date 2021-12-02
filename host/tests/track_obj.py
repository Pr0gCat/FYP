from communicate import Car
from aruco.find_aruco import findArucoMarkers
import cv2
import numpy as np
import time

def main():
    car = Car()
    input()
    car.init_car()
    car.set_pickup_mode()                                                     
    print('Car initialized')

    cap = cv2.VideoCapture(-1)
    t0 = time.time()
    while True:
        success, img = cap.read()
        if not success:
            continue
        arucoFound = findArucoMarkers(img)
        if len(arucoFound) > 0 and time.time() - t0 > 1:
            id, cx, cy, _ = arucoFound[0]
            dist = cy - 240
            print('tag found')
            if dist < -30:
                print('go up')
                car.move_posz(5)
            elif dist > 30:
                print('go down')
                car.move_posz(-5)
            t0 = time.time()
        cv2.imshow("Image", img)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"): break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()