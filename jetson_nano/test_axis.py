from communicate import Car
import time

if __name__ == '__main__':
    car = Car()
    car.init_car()
    print("Car initialized")
    input()
    print('move')
    car.move_posz(100)
    # car.wait_ack()
    print('done')
    while True:
        pass