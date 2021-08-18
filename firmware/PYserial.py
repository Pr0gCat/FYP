import serial
import time

COFIRM = 1
GET_CRAWL_STATE = 2
GET_CRAWL_TRIGGER = 3
GET_POSY = 4
GET_POSZ = 5
GET_SPEED = 6
LIMIT_TRIGGERED = 7
SET_SPEED = 8
MOVE_Y = 9
MOVE_Z = 10
HOME_Y = 11
HOME_Z = 12
SET_CRAWL_ANGLE = 13

if __name__ == '__main__':
    port = serial.Serial('COM3', baudrate=115200)
    print(port)
    # write a byte
    time.sleep(0.2)
    port.write('2114'.encode("utf-8"))
    time.sleep(1)
    # read a byte
    try:
        while True:
            while port.in_waiting:          # 若收到序列資料…
                data= port.read()
                print(data)
    except KeyboardInterrupt:
        port.close()    # 清除序列通訊物件
        print('再見！')