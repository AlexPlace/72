#include "PWMManager.h"
#include "InputManager.h"
#include "RobotCar.h"

int main(void){


//int inPins [4] = {7, 3, 0, 1};
//int outPins [4] = {1, 2, 3, 4};

//InputManager IM;
// std::cout << PWM.readChannel(7) << std::endl;






/* while (1){
    IM.keyboardInput();
    std::cout << IM.getDir() << std::endl;
    PWM.writeChannel(0, IM.getDir());
}
*/


RobotCar RC;
while (1){
    RC.updateDirection();
    RC.adjustSAndD();
}


/*
PWMManager PWM;
std::cout << PWM.readChannel(0) << std::endl;
std::cout << PWM.readChannel(1) << std::endl;
std::cout << PWM.readChannel(2) << std::endl;
*/

return(0);
}

