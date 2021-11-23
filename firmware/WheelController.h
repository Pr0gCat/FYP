#ifndef _WHEEL_CONTROLLER_H_
#define _WHEEL_CONTROLLER_H_

void wheel_setup();
void wheel_update();
void run_speed(int speed_l, int speed_r);
void run_distance(int dist_l, int dis_r, int speed);
void stop_motor_l();
void stop_motor_r();

#endif