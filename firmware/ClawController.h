#ifndef _CLAW_CONTROLLER_H_
#define _CLAW_CONTROLLER_H_

#include "config.h"
#include <AccelStepper.h>

class ClawController {
    public:
        ClawController(){
            y_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Y_STEP_PIN, STEPPER_Y_DIR_PIN);
            z_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Z_STEP_PIN, STEPPER_Z_DIR_PIN);
        }

        typedef enum {
            Y = 0,
            Z,
        } AXIS;

        void begin();
        void update();
        void homeY();
        void homeZ();
        void calibrateY();
        void calibrateZ();
        template<ClawController::AXIS axis> void moveTo(uint32_t mm);
        uint32_t getYMax(){ return y_max; }
        uint32_t getZMax(){ return z_max; }
    private:
        AccelStepper y_axis;
        AccelStepper z_axis;
        uint32_t y_max = 0;
        uint32_t z_max = 0;
};

#endif