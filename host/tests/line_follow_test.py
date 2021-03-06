from communicate import Car
from aruco.find_aruco import findGround
# import cv2.aruco as aruco
import cv2
import time

#  Return float or None
def line_following(img):
    # img = img[int(Screem_Height / 2):Screem_Height, 0:Screen_Weight]
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    ret, thresh = cv2.threshold(blur, 60, 255, cv2.THRESH_BINARY_INV)
    _, contours, hierarchy = cv2.findContours(
        thresh, 1, cv2.CHAIN_APPROX_NONE)
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)
        if M['m00'] == 0:
            return None
        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])
        #cv2.line(crop_img, (cx, 0), (cx, Screem_Height), (255, 0, 0), 1)
        #cv2.line(crop_img, (0, cy), (Screen_Weight, cy), (255, 0, 0), 1)
        #cv2.drawContours(crop_img, contours, -1, (0, 255, 0), 1)
        deviation = -1 * (cx - Screen_Weight / 2) / (Screen_Weight / 2)
        return deviation
    else:
        return None


if __name__ == '__main__':
    car = Car()
    # input()
    # car.init_car()
    # car.wait_ack()
    # print("Car initialized")
    car.set_linefollow_mode()
    car.wait_ack()
    input('ready')
    cap = cv2.VideoCapture(0)
    Screen_Weight = 720
    Screem_Height = 480
    cap.set(cv2.CAP_PROP_FPS, 30)
    cap.set(3, Screen_Weight)
    cap.set(4, Screem_Height)
    retutn_data = []
    flag = True
    while True:
        ret, frame = cap.read()
        MAX_SPPED = 400
        speed = 200
        factor = 300
        if ret:
            if(flag):
                flag = not findGround(frame)
                offset = line_following(frame)
                # print(offset)
                if offset is None:
                    # print('no line')
                    continue
                compan = int(offset * factor)
                speed_l = min(MAX_SPPED, speed - compan)
                speed_r = min(MAX_SPPED, speed + compan)
                print(offset, speed_l, speed_r)
                if flag:
                    car.run_speed(speed_l, speed_r)
                else:
                    print(flag)
                    car.run_speed(0, 0)
        # time.sleep(0.1)
