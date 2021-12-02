from dis import dis
from communicate import Car
from aruco.find_aruco import findGround, findArucoMarkers
import cv2
import time

#  Return float or None
def line_following(img):
    img = img[int(Screem_Height / 2):Screem_Height, 0:Screen_Weight]
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
    car.init_car()
    print("Car initialized")
    car.set_linefollow_mode()
    print('ready')
    input('press to start')

    cap = cv2.VideoCapture(0)
    Screen_Weight = 720
    Screem_Height = 480
    cap.set(cv2.CAP_PROP_FPS, 30)
    cap.set(3, Screen_Weight)
    cap.set(4, Screem_Height)
    # cap.set(cv2.CV_CAP_PROP_BUFFERSIZE, 0);
    retutn_data = []
    flag = False
    flag2 = False
    flag3 = False
    aruco_count = 0
    t0 = time.time()
    while True:
        ret, frame = cap.read()
        MAX_SPPED = 400
        speed = 200
        factor = 300
        if not ret:
            continue
        if not flag2:
            found, id, rot = findGround(frame)
            offset = line_following(frame)
            # print(offset)
            if offset is None:
                # print('no line')
                continue
            compan = int(offset * factor)
            speed_l = min(MAX_SPPED, speed - compan)
            speed_r = min(MAX_SPPED, speed + compan)
            # print(offset, speed_l, speed_r)
            if found: 
                print(f'id: {id}')
            if not found and not flag2:
                car.run_speed(speed_l, speed_r)
            elif id[0] == 0 and not flag:
                print('count: ', aruco_count)
                car.run_speed(0, 0)
                # time.sleep(2)
                car.run_distance(800, 800)
                # print('doing', time.time())
                if aruco_count > 1:
                    print('turn left')
                    car.run_distance(-470, 470)
                    flag = True
                else:
                    print('turn right')
                    car.run_distance(470, -470)
                aruco_count += 1
                ret, frame = cap.read()
                while True:
                    if not ret:
                        continue
                    found, id, rot = findGround(frame)
                    if not found:
                        break
                    ret, frame = cap.read()
            elif id[0] == 1:
                car.run_speed(0,0)
                flag2 = True
        elif not flag3:
            print('move y')
            car.move_relY(200)
            car.set_pickup_mode()
            flag3 = True
        if flag3:
            arucoFound = findArucoMarkers(frame)
            if len(arucoFound) > 0:
                # if time.time() - t0 < 0.1:
                #     continue
                # print(arucoFound)
                id, cx, cy, _ = arucoFound[0]
                distance = cy - 240
                print(distance)
                print('tag found')
                if distance > 5:
                    print('go down')
                    car.move_relZ(-5)
                elif distance < -5:
                    print('go up')
                    car.move_relZ(5)
                else:
                    print('Conpensate offset betweewn thr fork and camera...')
                    car.move_relZ(90-28)
                    print('Inserting fork...')
                    car.move_relY(-200)
                    print('Lifting cargo...')
                    car.move_relZ(30)
                    print('Pulling back...')
                    car.move_relY(270)
                    print('Placing cargo on the platform')
                    car.home_z()
                    print('Getting clearance...')
                    car.run_distance(-800, -800)
                    print('Pulling out the fork...')
                    car.move_relY(240)
                    # avoid collision by lifting the fork
                    car.move_relZ(600)
                    print('Homing Y...')
                    car.home_y()
                    print('Homing Z...')
                    car.home_z()
                    print('OP done')
                    break
                # t0 = time.time()
            else:
                car.move_relZ(30, blocking=False)

        # time.sleep(0.1)
