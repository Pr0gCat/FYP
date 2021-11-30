from communicate import Car

if __name__ == '__main__':
    car = Car()
    car.init_car()
    car.wait_ack(120)
    print('Car initialized')
