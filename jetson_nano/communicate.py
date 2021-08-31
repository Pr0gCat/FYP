from threading import Thread
import serial
import time
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


class Car:
    def __init__(self, port='/dev/ttyACM0') -> None:
        self.com = serial.Serial(port, baudrate=115200)
        print(self.com)
        time.sleep(1)
        self.receiver = Thread(target=self._receiver_main)
        self.receiver.setDaemon(True)
        self.receiver.start()

        def __del__(self):
            self.com.close()

    def _receiver_main(self):
        print('[Receiver] Start receiving')
        while True:
            try:
                while self.com.in_waiting:          # 若收到序列資料…
                    data = self.com.read()
                    print(data)
            except Exception as e:
                print(f'[Receiver] Exception: {e}')
            time.sleep(0.2)

    def run_speed(self, left, right):
        pkg = struct.pack('BBHH', SET_SPEED, 4, left, right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))
