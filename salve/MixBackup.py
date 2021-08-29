import cv2
import numpy as np
from pyzbar.pyzbar import decode


#  Return list of None
def cross_point(line1, line2):  # 计算交点函数
    x1 = line1[1]  # 取四点坐标
    y1 = line1[2]
    x2 = line1[3]
    y2 = line1[4]

    x3 = line2[1]
    y3 = line2[2]
    x4 = line2[3]
    y4 = line2[4]
    if (x4 - x3) == 0 and (x2 - x1) == 0:
        return False
    elif (x4 - x3) == 0:  # L2直线斜率不存在操作
        k2 = None
        b2 = 0
        x = x3
        k1 = (y2 - y1) * 1.0 / (x2 - x1)  # 计算k1,由于点均为整数，需要进行浮点数转化
        b1 = y1 * 1.0 - x1 * k1 * 1.0  # 整型转浮点型是关键
        y = k1 * x * 1.0 + b1 * 1.0
    elif (x2 - x1) == 0:
        k1 = None
        b1 = 0
        x = x1
        k2 = (y4 - y3) * 1.0 / (x4 - x3)
        b2 = y3 * 1.0 - x3 * k2 * 1.0
        y = k2 * x * 1.0 + b2 * 1.0
    else:
        k1 = (y2 - y1) * 1.0 / (x2 - x1)  # 计算k1,由于点均为整数，需要进行浮点数转化
        k2 = (y4 - y3) * 1.0 / (x4 - x3)  # 斜率存在操作
        b1 = y1 * 1.0 - x1 * k1 * 1.0  # 整型转浮点型是关键
        b2 = y3 * 1.0 - x3 * k2 * 1.0
        if k1 - k2 == 0:
            return False
        x = (b2 - b1) * 1.0 / (k1 - k2)
        y = k1 * x * 1.0 + b1 * 1.0
    return [x, y]


#  Return true or false
def cross_detect(image):
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    edges = cv2.Canny(gray, 60, 150, apertureSize=3)

    lines = cv2.HoughLines(edges, 1, np.pi / 180, 200)
    if lines is None:
        return
    line1 = [False, 0, 0, 0, 0]
    line2 = [False, 0, 0, 0, 0]
    for line in lines:
        for rho, theta in line:
            a = np.cos(theta)
            b = np.sin(theta)
            x0 = a * rho
            y0 = b * rho
            x1 = int(x0 + 3000 * -b)
            y1 = int(y0 + 3000 * a)
            x2 = int(x0 - 3000 * -b)
            y2 = int(y0 - 3000 * a)
            if not line1[0]:
                line1[0] = True
                line1[1] = x1
                line1[2] = y1
                line1[3] = x2
                line1[4] = y2
                cv2.line(image, (x1, y1), (x2, y2), (0, 255, 0), 10)
            elif not line2[0]:
                if not (abs(line1[1] - x1) < 100 and abs(line1[2] - y1) < 100):
                    if not (abs(line1[3] - x2) < 100 and abs(line1[4] - y2) < 100):
                        line2[0] = True
                        line2[1] = x1
                        line2[2] = y1
                        line2[3] = x2
                        line2[4] = y2
                        cv2.line(image, (x1, y1), (x2, y2), (255, 0, 0), 10)
            else:
                cv2.line(image, (x1, y1), (x2, y2), (0, 0, 255), 5)
    cross = cross_point(line1, line2)
    #  print(cross)
    #  print(line1[0])
    #  print(line2[0])
    if cross:
        if cross[0] < Screen_Weight and cross[1] < Screem_Height:
            return True
        else:
            return False
    else:
        return False


#  Return string or None
def decoder(image):
    string = None
    gray_img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    barcode = decode(gray_img)
    for obj in barcode:
        points = obj.polygon
        (x, y, w, h) = obj.rect
        pts = np.array(points, np.int32)
        pts = pts.reshape((-1, 1, 2))
        barcodedata = obj.data.decode("utf-8")
        string = str(barcodedata)
    return string


#  Return float or None
def line_following(image):
    crop_img = image[int(Screem_Height / 2):Screem_Height, 0:Screen_Weight]
    gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.threshold(blur, 60, 255, cv2.THRESH_BINARY_INV)
    contours, hierarchy = cv2.findContours(thresh.copy(), 1, cv2.CHAIN_APPROX_NONE)
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        m = cv2.moments(c)
        if m['m00'] == 0:
            return None
        cx = int(m['m10'] / m['m00'])
        cy = int(m['m01'] / m['m00'])
        deviation = -1 * (cx - Screen_Weight / 2) / (Screen_Weight / 2)
        return deviation
    else:
        return None


cap = cv2.VideoCapture(0)
Screen_Weight = 720
Screem_Height = 480
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(3, Screen_Weight)
cap.set(4, Screem_Height)

while True:
    ret, frame = cap.read()
    if ret:
        qrdata = decoder(frame)
        linedata = line_following(frame)
        detect = cross_detect(frame)
    #  Testing part
    '''
    print("QR: " + str(qrdata))
    print("Line: " + str(linedata))
    cv2.imshow('Image', frame)
    code = cv2.waitKey(10)
    if code == 27:
        break
    '''