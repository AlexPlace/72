#include "RobotCar.h"
#include "InputManager.h"


RobotCar::RobotCar(void){
    IM = new InputManager;
}

RobotCar::~RobotCar(){
    delete IM;
}

void RobotCar::adjustSAndD(void){
    float dir = IM->getDir();
    float speed = IM->getSpeed();
    float lSpeed = TURN_SHARPNESS * dir + speed;
    float rSpeed = TURN_SHARPNESS * (-1. * dir) + speed;
    
/*    if (lSpeed > 1){
        lSpeed = 1;
    }
    else if (lSpeed > 0.5){
        lSpeed = 0.6;
    }
    else if (lSpeed > 0){
        lSpeed = 0.2;
    }   
    else if (lSpeed == 0){
        lSpeed = 0;
    }
    else if (lSpeed > -0.5){
        lSpeed = -0.2;
    }
    else if (lSpeed > -1){
        lSpeed = -0.6;
    }
    else if (lSpeed < -1){
        lSpeed = -1;
    }
    
    
    
    if (rSpeed > 1){
        rSpeed = 1;
    }
    else if (rSpeed > 0.5){
        rSpeed = 0.6;
    }
    else if (rSpeed > 0){
        rSpeed = 0.2;
    }  
    else if (rSpeed == 0){
        rSpeed = 0;
    } 
    else if (rSpeed > -0.5){
        rSpeed = -0.2;
    }
    else if (rSpeed > -1){
        rSpeed = -0.6;
    }
    else if (rSpeed < -1){
        rSpeed = -1;
    }
*/

    if (rSpeed > 1){
        rSpeed = 1;
    }
    else if (rSpeed < -1){
        rSpeed = -1;
    }

    if (lSpeed > 1){
        lSpeed = 1;
    }
    else if (lSpeed < -1){
        lSpeed = -1;
    }

    
    std::cout << "lSpeed: " << lSpeed << std::endl;
    std::cout << "rSpeed: " << rSpeed << std::endl;
    
    for(int mot = 0; mot <= 1; mot++){
        IM->sendMotorSpeed(mot, -1 * lSpeed); // Added -1 to adjust for wire orientation
    }
    for(int mot = 2; mot <= 3; mot++){
        IM->sendMotorSpeed(mot, -1 * rSpeed);
    }    
}

void RobotCar::updateDirection(){
    IM->getInput();
}