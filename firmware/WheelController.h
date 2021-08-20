#ifndef _WHEEL_CONTROLLER_H_
#define _WHEEL_CONTROLLER_H_
#include "config.h"
#include <PIDController.h>

class WheelController {
    public:
        void begin();
        void update();
        void move(int32_t dist_l, int32_t dist_r);
        void moveSpeed(int32_t speed_l, int32_t speed_r);

        template<uint8_t in1_pin, uint8_t in2_pin> void WheelController::setMotorPwm(double pwm);

    private:
        void encoder_ticks_l();
        void encoder_ticks_r();

        uint32_t dist2go_l = 0, dist2go_r = 0, last_seen = 0;
        int32_t prev_pos_l = 0, prev_pos_r = 0;

        volatile int32_t encoder_pos_l = 0, encoder_pos_r = 0;
        PIDController speed_controller_l, speed_controller_r;
};

#endif
