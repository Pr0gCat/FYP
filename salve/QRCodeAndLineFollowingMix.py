import cv2
import numpy as np
from pyzbar.pyzbar import decode


def decoder(image):
    string = None
    gray_img = cv2.cvtColor(image, cv2.COLOR_BGR2RGB)
    barcode = decode(gray_img)
    for obj in barcode:
        points = obj.polygon
        (x, y, w, h) = obj.rect
        pts = np.array(points, np.int32)
        pts = pts.reshape((-1, 1, 2))
        #  cv2.polylines(image, [pts], True, (0, 255, 0), 3)

        barcodeData = obj.data.decode("utf-8")

        string = str(barcodeData)

        #  cv2.putText(frame, string, (x, y), cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)
        #  print(barcodeData)
    return string


def line_following(image):
    # Crop the image
    crop_img = image[int(Screem_Height / 2):Screem_Height, 0:Screen_Weight]
    # Convert to grayscale
    gray = cv2.cvtColor(crop_img, cv2.COLOR_BGR2GRAY)
    # Gaussian blur
    blur = cv2.GaussianBlur(gray, (5, 5), 0)
    # Color thresholding
    ret, thresh = cv2.threshold(blur, 60, 255, cv2.THRESH_BINARY_INV)
    # Find the contours of the frame
    contours, hierarchy = cv2.findContours(thresh.copy(), 1, cv2.CHAIN_APPROX_NONE)
    # Find the biggest contour (if detected)
    if len(contours) > 0:
        c = max(contours, key=cv2.contourArea)
        M = cv2.moments(c)

        if M['m00'] == 0:
            return None
        cx = int(M['m10'] / M['m00'])
        cy = int(M['m01'] / M['m00'])

        '''
        cv2.line(crop_img, (cx, 0), (cx, Screem_Height), (255, 0, 0), 1)
        cv2.line(crop_img, (0, cy), (Screen_Weight, cy), (255, 0, 0), 1)
        cv2.drawContours(crop_img, contours, -1, (0, 255, 0), 1)
        '''

        deviation = -1 * (cx - Screen_Weight / 2) / (Screen_Weight / 2)
        #  print(deviation)
        return deviation
    else:
        #  print("I don't see the line")
        return None


cap = cv2.VideoCapture(0)
Screen_Weight = 720
Screem_Height = 480
cap.set(cv2.CAP_PROP_FPS, 30)
cap.set(3, Screen_Weight)
cap.set(4, Screem_Height)

while True:
    ret, frame = cap.read()
    #  ret, frame_test = cv2.threshold(frame, 1, 255, cv2.THRESH_BINARY)
    qrdata = decoder(frame)
    linedata = line_following(frame)

    #  Testing part
    #  print("QR: " + str(qrdata))
    #  print("Line: " + str(linedata))
    #  cv2.imshow('Image', frame)
    code = cv2.waitKey(10)
    if code == 27:
        break
