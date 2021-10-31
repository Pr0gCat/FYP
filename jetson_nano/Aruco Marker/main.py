import cv2
import cv2.aruco as aruco
import numpy as np

def findAriucoMarkers(img, markerSize=6, totalMarkers=250):
    data=[]
    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    key = getattr(aruco,f'DICT_{markerSize}X{markerSize}_{totalMarkers}')
    arucoDict = aruco.Dictionary_get(key)
    arucoParam = aruco.DetectorParameters_create()
    corners, ids, rejected = aruco.detectMarkers(imgGray, arucoDict, parameters=arucoParam)
    if ids is not None:
        count = 0
        for x in ids:
            marker=[]
            cX = int((corners[count][0][0][0] + corners[count][0][2][0]) / 2.0)
            cY = int((corners[count][0][0][1] + corners[count][0][2][1]) / 2.0)
            cv2.circle(img, (cX, cY), 4, (0, 0, 255), -1)
            marker.append(ids[count][0])
            marker.append(cX)
            marker.append(cY)
            marker.append(round((((((cX-320)**2)+((cY-240)**2))**(0.5))),2))
            data.append(marker)
            count=count+1
        print(data)
        aruco.drawDetectedMarkers(img,corners)
    
    return data


def main():
    cap = cv2.VideoCapture(1)

    while True:
        sccuess, img = cap.read()
        arucoFound = findAriucoMarkers(img)
        cv2.circle(img, (320, 240), 4, (0, 0, 255), -1)
        cv2.imshow("Image", img)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"): break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()