#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <wiringPi.h>
#include <softPwm.h>
#include <wiringPiI2C.h>
#include <math.h>
#include "pca9685.h"



#define NUM_INPUT_CHANNELS 4
#define NUM_OUTPUT_CHANNELS 4
#define NUM_SAMPLES_TO_AV 7

#define MIN_LENGTH_PWM 18150
#define MAX_LENGTH_PWM 19200
#define AV_LENGTH_PWM ((MIN_LENGTH_PWM + MAX_LENGTH_PWM) / 2)
#define MAX_POSSIBLE_INTERVAL_PWM (MAX_LENGTH_PWM - MIN_LENGTH_PWM)


#define ZERO_THRESHOLD_PWM 200 // if the abs value of the input - AV_LENGTH_PWM < ZERO_THRESHOLD_PWM 0 will be output by readChannel

#define PIN_BASE 350

#define MAX_OUT_PWM 4096
#define MIN_DUTY_CYCLE 0.0625
#define MAX_DUTY_CYCLE 0.0875

#define OUTLIER_CUTOFF_LEVEL 50

class PWMManager{
    int * inputChannels;
    int * outputChannels;
    int currInPin;
    int fd; // for output file to breakout board

    public:

        float readBuff [NUM_SAMPLES_TO_AV];
        float goodReadBuff [NUM_SAMPLES_TO_AV];
        float t;
        int readBuffIt; // This will store what part of the read buffer contains updated data
        int numSamplesRead;


        PWMManager();
        float readChannel(int chan);
        int writeChannel(int channelIt, float normSpeed);
        static void PWMInHandler(void);
        void setInt(int channel);
        void stopInt(int channel);
        int minLengthPWM(int channel);
        int maxLengthPWM(int channel);
};

