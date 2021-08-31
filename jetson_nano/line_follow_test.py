from communicate import Car, Commander
import time

if __name__ == '__main__':
    car = Car()
    car.set_speed(1000, 1000)
    while True:
        time.sleep(1)
        
