from communicate import Car

if __name__ == '__main__':
    car = Car()
    # input()
    # car.init_car()
    print("Car initialized")
    input()
    car.set_linefollow_mode()
    print('send')
    car.run_distance(100, -100)
    while True:
        pass