import cv2
import time

if __name__ == '__main__':
    cam1 = cv2.VideoCapture(0)
    cam2 = cv2.VideoCapture(1)

    cam1.set(cv2.CAP_PROP_FPS, 30)
    cam1.set(cv2.CAP_PROP_FRAME_WIDTH, 720)
    cam1.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    cam2.set(cv2.CAP_PROP_FPS, 30)
    cam2.set(cv2.CAP_PROP_FRAME_WIDTH, 720)
    cam2.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)

    time.sleep(1)

    while True:
        ret1, frame1 = cam1.read()
        ret2, frame2 = cam2.read()

        cv2.imshow('cam1', frame1)
        cv2.imshow('cam2', frame2)

        if cv2.waitKey(1) == 27:
            cam1.close()
            cam2.close()
            exit()
        