from threading import Thread
import serial
import time
import struct
from enum import IntEnum, unique

class Car:

    @unique
    class CommandId(IntEnum):
        Confirm = 1
        Sync = 2
        Init = 3
        GetClawState = 4
        SetClawState = 5
        SetLinefollowMode = 6
        SetPickupMode = 7
        SetDockingMode = 8
        GetPosY = 9
        GetPosZ = 10
        MovePosY = 11
        MovePosZ = 12
        HomeY = 13
        HomeZ = 14
        RunDistance = 15
        GetMotorSpeed = 16
        SetMotorSpeed = 17
        Msg = 255

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
        pkg = struct.pack('BBhh', self.CommandId.SetMotorSpeed, 4, left, right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))
<<<<<<< HEAD

    def car_sync(self):
        pkg = struct.pack('BB', self.CommandId.Sync, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def car_init(self):
        pkg = struct.pack('BB', self.CommandId.Init, 0)
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

    def CMD_SET_LINEFOLLOW_MODE(self):
        pkg = struct.pack('BB', self.CommandId.SetLinefollowMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_SET_PICKUP_MODE(self):
        pkg = struct.pack('BB', self.CommandId.SetPickupMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_SET_DOCKING_MODE(self):
        pkg = struct.pack('BB', self.CommandId.SetDockingMode, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))
    
    def CMD_GET_POSY(self):
        pkg = struct.pack('BB', self.CommandId.GetPosY, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_GET_POSZ(self):
        pkg = struct.pack('BB', self.CommandId.GetPosZ, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_MOVE_POSY(self,posy):
        pkg = struct.pack('BBI', self.CommandId.MovePosY, 4,posy)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_MOVE_POSZ(self,posz):
        pkg = struct.pack('BBI', self.CommandId.MovePosZ, 4,posz)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_HOME_Y(self):
        pkg = struct.pack('BB', self.CommandId.HomeY, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_HOME_Z(self):
        pkg = struct.pack('BB', self.CommandId.HomeZ, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_RUN_DISTANCE(self,left,right):
        pkg = struct.pack('BBII', self.CommandId.RunDistance, 8,left,right)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))

    def CMD_GET_MOTOR_SPEED(self):
        pkg = struct.pack('BB', self.CommandId.GetMotorSpeed, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))


=======
        # time.sleep(0.1)
>>>>>>> 8e11a560106cd0fbf3664293b5c864d8e1fafc76
