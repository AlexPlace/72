#include "InputManager.h"
#include "PWMManager.h"

InputManager::InputManager(void){
    dir = 0;
    speed = 0;
    deltaDir = DELTA_DIR_INIT;
    deltaSpeed = DELTA_SPEED_INIT;
    
    dirMult = 1; 
    

    PWM = new PWMManager;
    
    return;
}

InputManager::~InputManager(void){
    delete PWM;
}

void InputManager::keyboardInput(void){
    char input;
    std::cout << "ic" << std::endl;
    std::cin >> input;
    switch(input){
        case 'a': 
            if(dir - deltaDir < -1){dir = -1; break;}
            dir -= deltaDir;
            break;
        case 'd':
            if(dir + deltaDir > 1){dir = 1; break;}
            dir += deltaDir;
            break;
        case 'w':
            if(speed + deltaSpeed > 1){speed = 1; break;}
            speed += deltaSpeed;
            break;
        case 'x':
            if(speed - deltaSpeed < -1){speed = -1; break;}
            speed -= deltaSpeed;
            break;
        case 's':
            speed = 0;
            dir = 0;
            break;
        case 'i':
            if(deltaDir + DELTA_DELTA_DIR > 1){deltaDir = 1; break;}
            deltaDir += DELTA_DELTA_DIR;
            break;
        case 'k':
            if(deltaDir - DELTA_DELTA_DIR < -1){deltaDir = -1; break;}
            deltaDir -= DELTA_DELTA_DIR;
            break;
        case 'o':
            if(deltaSpeed + DELTA_DELTA_SPEED > 1){deltaSpeed = 1; break;}
            deltaSpeed += DELTA_DELTA_SPEED;
            break;
        case 'l':
            if(deltaSpeed - DELTA_DELTA_SPEED < -1){deltaSpeed = -1; break;}
            deltaSpeed -= DELTA_DELTA_SPEED;
            break;
    }
    return;
}

void InputManager::PWMInput(void){
    // for some reason up is a more negative pwm value normally, so mult. by -1
    speed = -1 * PWM->readChannel(SPEED_CHANNEL_POSN); 
    dir = -1 * PWM->readChannel(DIR_CHANNEL_POSN);
    
    if (speed < ZERO_CUT && speed > -1 * ZERO_CUT){
        speed = 0;
    }
    
    if (dir < ZERO_CUT && dir > -1 * ZERO_CUT) {
        dir = 0;
    }
    
//    std::cout << PWM->readChannel(DIRMULT_CHANNEL_POSN) << std::endl;
    dirMult = PWM->readChannel(DIRMULT_CHANNEL_POSN);
    dirMult = (dirMult + 1) / 2;
    
/*    if(PWM->readChannel(DIRMULT_CHANNEL_POSN) > 0){
        dirMult = 1;
    }
    else {
        dirMult = -1;
    }
*/
    std::cout << "dirMult: " << std::endl;
    std::cout << dirMult << std::endl;
    
    return;
}

void InputManager::getInput(void){
    PWMInput();
}

float InputManager::getDir(void){
    return (dir);
}

float InputManager::getSpeed(void){
    return (dirMult * speed * -1);
}

void InputManager::sendMotorSpeed(int mot, float speed){
    PWM->writeChannel(mot, speed);
}