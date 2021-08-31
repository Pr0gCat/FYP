import serial
import time
from _thread import *
import struct

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
  time.sleep(0.2)

def get_crawl_state():
  pass


def get_crawl_trigger():
  pass


def get_posy():
  pass


def get_posz():
  pass


def get_speed():
  pass


def limit_trigger():
  pass


def move_y():
  pass    

def move_z():
  pass


def home_y():
  pass


def home_z():
  pass


def set_crawl_angle():
  pass

def set_speed(port, left,right):
      pkg = struct.pack('BBHH', SET_SPEED, 4, left, right)
      port.write(pkg)
      cs = 0xff & sum(pkg)
      pkg = struct.pack('B', cs)
      port.write(pkg)
    

if __name__ == '__main__':
    port = serial.Serial('/dev/cu.usbmodem138', baudrate=115200)
    start_new_thread(receive,(port,))
    print("Receive thread strat")
    # write a byte
    time.sleep(2)
    set_speed(port, 1000, 1000)
    # def set_speed(port, left,right):
    #   pkg = struct.pack('BBhh', SET_SPEED, 4, left, right)
    #   # print(pkg)
    #   port.write(pkg)
    #   cs = str(0xff & (SET_SPEED+2+left+right))
    #   port.write(cs.encode("utf-8"))
    
    while True:
        pass