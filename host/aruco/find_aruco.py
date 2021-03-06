import cv2
import cv2.aruco as aruco
import numpy as np
import math

with open('camera_cal.npy', 'rb') as f:
    camera_matrix = np.load(f)
    camera_distortion = np.load(f)

key = getattr(aruco, 'DICT_6X6_250')
arucoDict = aruco.Dictionary_get(key)
arucoParam = aruco.DetectorParameters_create()


def isRotationMatrix(R):
    Rt = np.transpose(R)
    shouldBeIdentity = np.dot(Rt, R)
    I = np.identity(3, dtype=R.dtype)
    n = np.linalg.norm(I-shouldBeIdentity)
    return n < 1e-6


def rotationMatrixToEulerAngles(R):
    assert(isRotationMatrix(R))

    sy = math.sqrt(R[0, 0] * R[0, 0] + R[1, 0] * R[1, 0])

    singular = sy < 1e-6

    if not singular:
        x = math.atan2(R[2, 1], R[2, 2])
        y = math.atan2(-R[2, 0], sy)
        z = math.atan2(R[1, 0], R[0, 0])
    else:
        x = math.atan2(-R[1, 2], R[1, 1])
        y = math.atan2(-R[2, 0], sy)
        z = 0

    return np.array([x, y, z])


def findArucoMarkers(img, markerSize=7, totalMarkers=250):
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
            # cv2.circle(img, (cX, cY), 4, (0, 0, 255), -1)
            marker.append(ids[count][0])
            marker.append(cX)
            marker.append(cY)
            # distance between mid-point
            marker.append(round((((((cX-320)**2)+((cY-240)**2))**(0.5))),2))
            data.append(marker)
            count=count+1
        # print(data)
        aruco.drawDetectedMarkers(img,corners)

    return data


def findGround(img, markerSize=6):
    img = img[int(480 / 3):480, :]
    imgGray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    corners, ids, rejected = aruco.detectMarkers(
        imgGray, arucoDict, parameters=arucoParam)
    ID = -1
    if ids is not None:
        for x in ids:
            rvec_list_all, tvec_list_all = aruco.estimatePoseSingleMarkers(
                corners, markerSize, camera_matrix, camera_distortion)

            rvec = rvec_list_all[0][0]

            rvec_flipped = rvec * -1
            rotation_matrix, jacobian = cv2.Rodrigues(rvec_flipped)

            pitch, roll, yaw = rotationMatrixToEulerAngles(rotation_matrix)

            ID = x
            rotation = math.degrees(yaw)

        return True, ID, rotation
    return False, None, None


def main():

    cap = cv2.VideoCapture(-1)

    while True:
        success, img = cap.read()
        if not success:
            continue
        found, id, _ = findGround(img)
        if found:
            print(id)

        cv2.imshow("Image", img)
        key = cv2.waitKey(1) & 0xFF
        if key == ord("q"):
            break

    cap.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
