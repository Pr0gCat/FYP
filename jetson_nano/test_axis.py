from communicate import Car
import time

if __name__ == '__main__':
    car = Car()
    input()
    car.init_car()
    car.wait_ack()
    # print("Car initialized")
    car.move_posz(100)
    car.wait_ack()
    while True:
        pass