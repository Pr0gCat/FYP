from communicate import Car
from aruco.main import findArucoMarkers
import cv2
import numpy as np
import time

def main():
    car = Car()
    input()
    car.set_pickup_mode()
    car.init_car()                                                     
    print('Car initialized')
    with open('camera_cal.npy', 'rb') as f:
        camera_matrix = np.load(f)
        camera_distortion = np.load(f)

    cap = cv2.VideoCapture(-1)
    t0 = time.time()
    while True:
        success, img = cap.read()
        if not success:
            continue
        arucoFound = findArucoMarkers(img,camera_matrix,camera_distortion)
        if len(arucoFound) > 0 and time.time() - t0 > 100:
            id, cx, cy, dx, dy = arucoFound[0]
            print(dx, dy)
            # print('tag found')
            t0 = time.time()
        cv2.imshow("Image", img)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"): break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    main()