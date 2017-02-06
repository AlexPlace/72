#include <iostream>
#include <vector>

#define DELTA_DIR_INIT 0.1
#define DELTA_SPEED_INIT 0.1
#define DELTA_DELTA_DIR 0.1
#define DELTA_DELTA_SPEED 0.1

#define SPEED_CHANNEL_POSN 1
#define DIR_CHANNEL_POSN 0
#define DIRMULT_CHANNEL_POSN 2

#define ZERO_CUT 0.25

class PWMManager;

class InputManager{
    float dir; // -1 = farthest left possible, 1 = farthest right possible, 0 = straight
    float speed; // -1 = fastest backwards possible, 1 = fastest forwards possible, 0 = stopped
    float deltaDir;
    float deltaSpeed;
    float dirMult;
    PWMManager * PWM;
    
    public:
        InputManager(void);
        ~InputManager(void);
        void keyboardInput(void);
        void PWMInput(void);
        void getInput(void);
        float getDir(void);
        float getSpeed(void);
        int sendSerial(char);
        void sendMotorSpeed(int mot, float speed);
};