from communicate import Car

if __name__ == '__main__':
    car = Car()
    car.init_car()
    print('Car initialized')
    car.move_posy(300)
    car.move_posy(100)
    car.home_y()
