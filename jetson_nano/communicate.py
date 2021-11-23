from threading import Thread, Event
import serial
import time
import struct
from enum import IntEnum, unique

class Car:

    @unique
    class CommandId(IntEnum):
        Confirm = 1
        Init = 2
        GetClawState = 3
        SetClawState = 4
        SetLinefollowMode = 5
        SetPickupMode = 6
        SetDropOffMode = 7
        MovePosY = 8
        MovePosZ = 9
        GotoPosY = 10
        GotoPosZ = 11
        HomeY = 12
        HomeZ = 13
        RunDistance = 14
        GetMotorSpeed = 15
        SetMotorSpeed = 16
        Msg = 255

    def __init__(self, port='/dev/serial/by-id/usb-Arduino_LLC_Arduino_Micro-if00'):
        self.com = serial.Serial(port, baudrate=115200)
        self.wait_flag = Event()
        print(self.com)
        time.sleep(1)
        self.receiver = Thread(target=self._receiver_main)
        self.receiver.setDaemon(True)
        self.receiver.start()

        def __del__(self):
            self.com.close()

    def _receiver_main(self):
        print('[Receiver] Start receiving')
        len1 = 0
        flag = 0
        data_len = 0
        msg=""
        while True:
            try:
                while self.com.in_waiting:          # 若收到序列資料…
                    data = self.com.read()
                    print(f'CAR: {data}')
                    
                    if flag == 0:
                        if ord(data) == 255:
                            flag =1
                        elif ord(data) == 1:
                            self.wait_flag.set()
                    elif flag == 1:
                        len1 = ord(data)
                        flag = 2
                    elif flag == 2:
                        msg += str(data.decode("utf-8"))
                        data_len = data_len+1
                        if data_len == len1:
                            flag = 3
                    elif flag == 3:
                        cs = data
                        print(f'CAR say: {msg}')
                        len1 = 0
                        flag = 0
                        data_len = 0
                        msg=""
                        
            except Exception as e:
                print(f'[Receiver] Exception: {e}')
            time.sleep(0.2)

    def wait_ack(self):
        while not self.wait_flag.is_set():
            pass

    def init_car(self):
        pkg = struct.pack('BB', self.CommandId.Init, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def run_speed(self, left, right):
        pkg = struct.pack('BBhh', self.CommandId.SetMotorSpeed, 4, left, right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def get_car_claw(self):
        pkg = struct.pack('BB', self.CommandId.GetClawState, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def set_car_claw(self,left,right):
        pkg = struct.pack('BBbb', self.CommandId.SetClawState,2,left,right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def set_linefollow_mode(self):
        pkg = struct.pack('BB', self.CommandId.SetLinefollowMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def set_pickup_mode(self):
        pkg = struct.pack('BB', self.CommandId.SetPickupMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def set_docking_mode(self):
        pkg = struct.pack('BB', self.CommandId.SetDockingMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    #TODO: 待測試

    # def goto_posy(self, posy):
    #     pkg = struct.pack('BBI', self.CommandId.GetPosY, 0)
    #     cs = 0xff & sum(pkg)
    #     self.com.write(pkg)
    #     self.com.write(struct.pack('B', cs))

    # def goto_posz(self, posz):
    #     pkg = struct.pack('BBI', self.CommandId.GetPosZ, 0)
    #     cs = 0xff & sum(pkg)
    #     self.com.write(pkg)
    #     self.com.write(struct.pack('B', cs))

    def move_posy(self, posy):
        pkg = struct.pack('BBh', self.CommandId.MovePosY, 2,posy)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def move_posz(self,posz):
        pkg = struct.pack('BBh', self.CommandId.MovePosZ, 2, posz)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def home_y(self):
        pkg = struct.pack('BB', self.CommandId.HomeY, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def home_z(self):
        pkg = struct.pack('BB', self.CommandId.HomeZ, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def run_distance(self,left,right):
        pkg = struct.pack('BBhh', self.CommandId.RunDistance, 4,left,right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def get_run_speed(self):
        pkg = struct.pack('BB', self.CommandId.GetMotorSpeed, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))
        