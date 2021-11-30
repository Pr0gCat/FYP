from threading import Thread, Lock
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
        self.wait_flag = Lock()
        print(self.com)
        time.sleep(1)
        self.receiver = Thread(target=self._receiver_main)
        self.receiver.setDaemon(True)
        self.receiver.start()
        self.latest_confirm = -1

        def __del__(self):
            self.com.close()

    def _receiver_main(self):
        import time
        print('[Receiver] Start receiving')
        buff = []
        flag = 0
        checksum = 0
        t0 = time.time()
        self.com.flushInput()
        self.com.flushOutput()
        while True:
            if (time.time()-t0) > 1 and flag != 0:
                flag = 0
                buff.clear()
            try:
                while self.com.in_waiting: # 若收到序列資料…
                    
                    data = self.com.read()[0] # 讀取一個字元
                    print(f'[Receiver] flag: {flag}, data: {data}')
                    if flag < 2:
                        buff.append(data) # 將資料存入buff
                        t0 = time.time()
                        flag += 1
                        if flag == 2 and buff[1] == 0:
                            flag = 3

                    elif flag == 2:
                        buff.append(data)
                        t0 = time.time()
                        # print(buff[1], buff[2:])
                        if len(buff[2:]) == buff[1]:
                            print('[Receiver] payload received')
                            flag = 3
                    elif flag == 3:
                        pkt_cs = data
                        checksum = 0xff & sum(buff)
                        if checksum == pkt_cs:
                            self.unpack_msg(buff)
                        else:
                            print('[Receiver] Checksum Error', buff, checksum, pkt_cs)
                        flag = 0
                        buff.clear()
                        
            except Exception as e:
                print(f'[Receiver] Exception: {e}')
            time.sleep(0.2)

    def unpack_msg(self, payload):
        cmd = payload[0]
        if cmd == self.CommandId.Confirm:
            print('[Receiver] Confirm Code: %d' % payload[2])
            with self.wait_flag:
                self.latest_confirm = payload[2]
            
        elif cmd == self.CommandId.Msg:
            print('[Receiver] Msg: ', ''.join([chr(c) for c in payload[2:]]))

    def wait_ack(self, id, timeout=10):
        """
        Wait for ack from car
        timout: seconds - set to negative value to wait forever
        """
        t0 = time.time()
        while True:
            with self.wait_flag:
                if self.latest_confirm == id:
                    break
            if time.time() - t0 > timeout:
                print('[Communicate] Timeout')
                break
            time.sleep(0.1)

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
        