from communicate import Car

if __name__ == '__main__':
    car = Car()
    car.init_car()
    print('Car initialized')
    car.move_posz(300)
    car.move_posz(100)
    car.home_z()
