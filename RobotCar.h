#include <iostream>
#include <stdlib.h>

class InputManager;

#define TURN_SHARPNESS 1

class RobotCar{
    InputManager * IM;
    
    public:
        RobotCar(void);
        ~RobotCar();
        void adjustSAndD();
        void updateDirection();
};