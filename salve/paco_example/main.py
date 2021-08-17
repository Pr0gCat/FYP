import serial

if __name__ == '__main__':
    port = serial.Serial('/dev/ttyUSB0', baudrate=115200, xonxoff=False, rtscts=False)
    # write a byte
    port.write(0x80)
    # read a byte
    port.read()