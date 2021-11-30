from communicate import Car

if __name__ == '__main__':
    car = Car()
    car.set_pickup_mode()
    car.wait_ack(2)
    print('Car initialized')
    input()
    car.set_linefollow_mode()
    car.wait_ack(2)
