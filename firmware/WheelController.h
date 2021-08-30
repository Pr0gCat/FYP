#ifndef _WHEEL_CONTROLLER_H_
#define _WHEEL_CONTROLLER_H_

void wheel_setup();
void wheel_update();
void set_speed_r(int speed_r);
void set_speed_l(int speed_l);
void set_distance_r(int distance_r);
void set_distance_l(int distance_l);
void stop_motor_l();
void stop_motor_r();


#endif