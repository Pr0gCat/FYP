from communicate import Car
import time

if __name__ == '__main__':
    car = Car()
    car.init_car()
    car.wait_ack()
    print("Car initialized")
    input()
    car.move_posz(100)
    # car.wait_ack()
    print('done')
    while True:
        pass