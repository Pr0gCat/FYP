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
        time.sleep(0.1)
