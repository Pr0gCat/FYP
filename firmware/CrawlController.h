#ifndef _CRAWL_CONTROLLER_H_
#define _CRAWL_CONTROLLER_H_

#include "config.h"
#include <AccelStepper.h>

class CrawlController {
    public:
        CrawlController(){
            y_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Y_STEP_PIN, STEPPER_Y_DIR_PIN);
            z_axis = AccelStepper(AccelStepper::DRIVER, STEPPER_Z_STEP_PIN, STEPPER_Z_DIR_PIN);
        };
        void begin();
        void update();
        void homeY();
        void homeZ();
        void calibrateY();
        void calibrateZ();
    private:
        AccelStepper y_axis;
        AccelStepper z_axis;
        uint32_t y_max = 0;
        uint32_t z_max = 0;
};

#endif