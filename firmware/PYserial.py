import serial
import time
from _thread import *

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

def receive(port):
    # read a byte
    try:
        while True:
            while port.in_waiting:          # 若收到序列資料…
                data= port.read()
                print(data)
    except KeyboardInterrupt:
        port.close()    # 清除序列通訊物件
        print("再見！")

if __name__ == '__main__':
    port = serial.Serial('COM3', baudrate=115200)
    start_new_thread(receive,(port,))
    print("Receive thread strat")
    print(port)
    # write a byte
    time.sleep(2)
    a = str(0xff & 4)
    port.write(b'211')
    port.write(a.encode("utf-8"))
    
    while True:
        pass

def comfirm(cmd):
  if(cmd != 1):
    port.write(1)
    port.write(1)
    port.write(cmd)
  

def get_crawl_state():
  comfirm(GET_CRAWL_STATE)


def get_crawl_trigger():
  comfirm(GET_CRAWL_TRIGGER)


def get_posy():
  comfirm(GET_POSY)


def get_posz():
  comfirm(GET_POSZ)


def get_speed():
  comfirm(GET_SPEED)


def limit_trigger():
  comfirm(LIMIT_TRIGGERED)


def set_speed():
    comfirm(SET_SPEED)


def move_y():
    comfirm(MOVE_Y)


def move_z():
    comfirm(MOVE_Z)


def home_y():
    comfirm(HOME_Y)


def home_z():
    comfirm(HOME_Z)


def set_crawl_angle():
    comfirm(SET_CRAWL_ANGLE)
