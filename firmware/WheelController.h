#ifndef _WHEEL_CONTROLLER_H_
#define _WHEEL_CONTROLLER_H_
#include "config.h"
#include <PIDController.h>
/*
    The speed controller takes desire speed and current speed of the motor
    then output proper pwm signal.
    The distance controller takes desire distance and current position of the encoders,
    then compute the desire speed for speed controller to regulate
*/
class WheelController{
    public:
        void begin();
        void update();
        void runSpeed(int32_t tps_l, int32_t tps_r);
        void moveTo(int32_t dist_l, int32_t dist_r);
        void stop();
    private:
        void encoder_ticks_l();
        void encoder_ticks_r();

        uint32_t distance_to_go = 0;
        uint32_t last_seen = 0;
        int32_t prev_pos_l = 0, prev_pos_r = 0;

        volatile int32_t encoder_pos_l = 0, encoder_pos_r = 0;

        PIDController speed_controller_l, \
                    speed_controller_r, \
                    distance_controller_l, \
                    distance_controller_r;
};

#endif