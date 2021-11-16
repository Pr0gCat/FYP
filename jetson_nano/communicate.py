from threading import Thread, Event
import serial
import time
import struct
from enum import IntEnum, unique

event = Event()
communicat_id = 0

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
        GotoPosY = 8
        GotoPosZ = 9
        MovePosY = 10
        MovePosZ = 11
        HomeY = 12
        HomeZ = 13
        RunDistance = 14
        GetMotorSpeed = 15
        SetMotorSpeed = 16
        Msg = 255

    def __init__(self, port='/dev/serial/by-id/usb-Arduino_LLC_Arduino_Micro-if00') -> None:
        self.com = serial.Serial(port, baudrate=115200)
        print(self.com)
        time.sleep(1)
        self.receiver = Thread(target=self._receiver_main, args=(communicat_id, ))
        self.receiver.setDaemon(True)
        self.receiver.start()

        def __del__(self):
            self.com.close()

    def _receiver_main(self, id):
        print('[Receiver] Start receiving')
        count = 0
        while True:
            try:
                while self.com.in_waiting:          # 若收到序列資料…
                    data = self.com.read()
                    if data == id  and count == 2:
                        event.set
                    if data == 1:
                        count = count + 1
                    else :
                        count = 0
                    print(data)
            except Exception as e:
                print(f'[Receiver] Exception: {e}')
            time.sleep(0.2)

    def wait_ack(self, id, blocking):
        communicat_id = id
        while blocking:
            if event.is_set():
                break
        
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
        pkg = struct.pack('BBI', self.CommandId.MovePosY, 4,posy)
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
        pkg = struct.pack('BBII', self.CommandId.RunDistance, 8,left,right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def get_run_speed(self):
        pkg = struct.pack('BB', self.CommandId.GetMotorSpeed, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))


