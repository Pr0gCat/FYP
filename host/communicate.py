
#TODO: Refactor this
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
        # Camera position
        SetLinefollowMode = 3
        SetPickupMode = 4
        SetDropOffMode = 5
        # Lifter
        MoveRelY = 6
        MoveRelZ = 7
        GotoPosY = 8
        GotoPosZ = 9
        StopY = 10
        StopZ = 11
        HomeY = 12
        HomeZ = 13
        # Wheels
        RunDistance = 14
        RunSpeed = 15
        Msg = 255

    def __init__(self, port='/dev/serial/by-id/usb-Arduino_LLC_Arduino_Micro-if00'):
        self.com = serial.Serial(port, baudrate=115200)
        # make sure the serial buffers are empty
        self.com.reset_output_buffer()
        self.com.reset_output_buffer()
        print(self.com)
        time.sleep(1)
        
        self.wait_flag = Lock()
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
        
        while True:
            # timeout when no data received for 3s while waiting for ack
            if (time.time()-t0) > 3 and flag != 0:
                flag = 0
                buff.clear()
            try:
                while self.com.in_waiting: # 若收到序列資料…
                    
                    data = self.com.read()[0] # 讀取一個字元
                    # print(f'[Receiver] flag: {flag}, data: {data}')
                    if flag < 2:
                        buff.append(data) # 將資料存入buff
                        t0 = time.time()
                        flag += 1
                        if flag == 2 and buff[1] == 0:
                            flag = 3

                    elif flag == 2:
                        buff.append(data)
                        t0 = time.time()
                        if len(buff[2:]) == buff[1]:
                            # print('[Receiver] payload received')
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
            # print('[Receiver] Confirm Code: %d' % payload[2])
            with self.wait_flag:
                self.latest_confirm = payload[2]
            
        elif cmd == self.CommandId.Msg:
            print(f'Car > "{"".join([chr(c) for c in payload[2:]])}"')
        else:
            print('[Receiver] Unknown command: %d' % cmd)

    def wait_ack(self, id, timeout=10):
        """
        Wait for ack from car
        timout: seconds - set to negative value to wait forever
        """
        print(f'[Communicate] Waiting for ack {repr(id)}')
        timeout = abs(timeout)
        t0 = time.time()
        while True:
            if self.latest_confirm == id:
                print(f'[Communicate] Ack {repr(id)} received')
                break
            if timeout > 0 and time.time() - t0 > timeout:
                print(f'\t- Timeout id: {repr(id)}')
                break
            time.sleep(0.1) # let receiver thread run
        self.latest_confirm = -1
        print(f'\t- Spend {round(time.time() - t0, 3)}s on this call')

    def send_pkt(self, cmd, payload=None, blocking=True, timeout=30):
        if payload:
            pkg = struct.pack('BB', cmd, len(payload)) + payload
        else:
            pkg = struct.pack('BB', cmd, 0)
        cs = 0xff & sum(pkg)
        self.com.write(pkg)
        self.com.write(struct.pack('B', cs))
        if blocking:
            self.wait_ack(cmd, timeout)


    def init_car(self, blocking=True, timeout=120):
        self.send_pkt(self.CommandId.Init, blocking=blocking, timeout=timeout)

    def run_speed(self, left, right):
        # this won't ack
        self.send_pkt(self.CommandId.RunSpeed, struct.pack('hh', left, right), blocking=False)

    def set_linefollow_mode(self, blocking=True, timeout=2):
        self.send_pkt(self.CommandId.SetLinefollowMode, blocking=blocking, timeout=timeout)

    def set_pickup_mode(self, blocking=True, timeout=2):
        self.send_pkt(self.CommandId.SetPickupMode, blocking=blocking, timeout=timeout)

    def set_dropoff_mode(self, blocking=True, timeout=2):
        self.send_pkt(self.CommandId.SetDropOffMode, blocking=blocking, timeout=timeout)

    def move_relY(self, posy, blocking=True, timeout=60):
        self.send_pkt(self.CommandId.MoveRelY, struct.pack('h', posy), blocking=blocking, timeout=timeout)

    def move_relZ(self,posz, blocking=True, timeout=60):
        self.send_pkt(self.CommandId.MoveRelZ, struct.pack('h', posz), blocking=blocking, timeout=timeout)

    def home_y(self, timeout=30):
        self.send_pkt(self.CommandId.HomeY, timeout=timeout)

    def home_z(self, timeout=30):
        self.send_pkt(self.CommandId.HomeZ, timeout=timeout)

    def run_distance(self,left,right, blocking=True, timeout=10):
        self.send_pkt(self.CommandId.RunDistance, struct.pack('hh', left, right), blocking=blocking, timeout=timeout)
