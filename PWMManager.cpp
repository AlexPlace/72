#include "PWMManager.h"
#include <pca9685.h>

/*
float PWMManager::readBuff [NUM_SAMPLES_TO_AV] = {0, 0, 0, 0};
float PWMManager::t = 0;
int PWMManager::readBuffIt = 0; // This will store what part of the read buffer contains updated data
int PWMManager::numSamplesRead = 0;
*/



using namespace std;


PWMManager::PWMManager(void){

/*    int inPins [4] = {7, 3, 0, 1};
    int outPins [4] = {1, 2, 3, 4};
*/
    
    inputChannels = new int [NUM_INPUT_CHANNELS];
    outputChannels = new int [NUM_OUTPUT_CHANNELS];
    
    inputChannels[0] = 7;
    inputChannels[1] = 1;
    inputChannels[2] = 0;
    inputChannels[3] = 1;
    
    outputChannels[0] = 1; 
    outputChannels[1] = 2; 
    outputChannels[2] = 3; 
    outputChannels[3] = 4; 

    wiringPiSetup();
    for(int i = 0; i < NUM_INPUT_CHANNELS; i++){
        pinMode(inputChannels[i], INPUT);
//        wiringPiISR(inputChannels[i], INT_EDGE_BOTH, PWMInHandler);
    }

//    system("/usr/bin/gpio edge 7 none");
//    system("echo 7 > /sys/class/gpio/unexport");
//    system("gpio unexport 7");
//    system("gpio unexport 11");
//    system("gpio unexport 15");

 /*   for(int i = 0; i < NUM_OUTPUT_CHANNELS; i++){
        softPwmCreate(outputChannels[i], 50, 100);
    } */
    
    fd = pca9685Setup(PIN_BASE, 0x40, 50);
    cout << "I2C connection status: " << fd << endl;
    pca9685PWMReset(fd);
    
}

/*
void PWMManager::PWMInHandler(void){
    cout << "Handler Called" << endl;
    if (readBuffIt >= NUM_SAMPLES_TO_AV){
        return;
    }
    float dt;
    dt = micros() - PWMManager::t;
//    cout << dt << endl;
    if(dt > MIN_LENGTH_PWM && dt < MAX_LENGTH_PWM){
        readBuff[PWMManager::readBuffIt]=dt;
        numSamplesRead++;
        readBuffIt++;
//        cout << "used" << endl;
    }
    PWMManager::t = micros();
    return;
}
*/


float average(float * arr, int length){
    float av = 0;
    for(int i = 0; i < length; i++){
        av += arr[i];
    }
    return av/length;
}

float findOutlier(float * arr, int length){
    // returns index of max outlier if outlier further from av than OUTLIER_CUTOFF_LEVEL
    // otherwise will return -1
    float maxDis = 0;
    float dis = 0;
    int pos = -1;
    float av = average(arr, length);
    for(int i = 0; i < length; i++){
        dis = abs(arr[i] - av);
        if (dis > maxDis && dis > OUTLIER_CUTOFF_LEVEL){
            maxDis = dis;
            pos = i;
        }
    }
    return pos;
}

float stdDev(float * arr, float av, int length){
    float difSquareSum = 0;
    for(int i = 0; i < length; i++){
        difSquareSum += pow((arr[i] - av), 2);
    }
    return sqrt(difSquareSum / length);
}


float PWMManager::readChannel(int chan){
    readBuffIt = 0;
    t = micros();
    currInPin = chan;

    /*
    if(chan == 0){
         system("gpio export 7");
    }
    */

/*   
//    system("gpio export 3 INPUT");
////    pinMode(3, INPUT); //Make SURE InputChannels[chan]...
////    wiringPiISR(3, INT_EDGE_BOTH, PWMInHandler);  
////////    setInt(inputChannels[chan]);
////////    while(readBuffIt < NUM_SAMPLES_TO_AV){}
////////    cout << "sampling finished" << endl;
//////    stopInt(inputChannels[chan]);

//    system("/usr/bin/gpio edge 7 none");
////////    system("gpio unexport 0");
////////    system("gpio unexport 3");
////////    system("gpio unexport 7");
////////    system("gpio unexport all");
*/

    float maxL = maxLengthPWM(chan);
    float minL = minLengthPWM(chan);
    float avL = (maxL + minL) / 2;
    float normFactor = (maxL - minL) / 2;

    int onHigh = 0;
    float tt;
    float dt;
    float tn = micros();
    while(readBuffIt < NUM_SAMPLES_TO_AV){
        if((digitalRead(inputChannels[chan]) && !onHigh) && (digitalRead(inputChannels[chan]) && digitalRead(inputChannels[chan]))){
            tn = micros();
            onHigh = 1;
        }
        else if ((!digitalRead(inputChannels[chan]) && onHigh) && (!digitalRead(inputChannels[chan]) && !digitalRead(inputChannels[chan]))) {
            tt = micros();
            dt = tt - tn;
            if ( (dt < maxL) && (dt > minL) ) {
                readBuff[readBuffIt] = dt;
                readBuffIt++;
            }
//            cout << dt << endl;
            onHigh = 0;
        }
    }
  

    
    float avRead = average(readBuff, NUM_SAMPLES_TO_AV);
    int numGoodSamples = NUM_SAMPLES_TO_AV;
    int outlier = -1; // initialize to "there are no outliers"
    int noStop = 1;
    while (noStop){
        outlier = findOutlier(readBuff, numGoodSamples);
        if(outlier >= 0){
            int goodReadBuffIt = 0;
            for(int outIt = 0; outIt < numGoodSamples; outIt++){
                if(outIt != outlier){                
                    goodReadBuff[goodReadBuffIt] = readBuff[outIt];
                    goodReadBuffIt++;
                }
            }
            numGoodSamples -= 1;
            for(int copyIt = 0; copyIt < numGoodSamples; copyIt++){
                readBuff[copyIt] = goodReadBuff[copyIt];
            }
        } 
        else {
            break;
        }
    }
    avRead = average(readBuff, numGoodSamples);

/*    float sD = stdDev(readBuff + 1, avRead, NUM_SAMPLES_TO_AV - 1);
    float sumReads = 0;
    float numOutliers = 1;
    for (int sDit = 0; sDit < NUM_SAMPLES_TO_AV - 1; sDit++){
        if (abs(readBuff[1 + sDit] - avRead) < sD){
            sumReads += readBuff[1 + sDit];
        }
        else {
            numOutliers++;
        }
    }
    avRead = sumReads / (NUM_SAMPLES_TO_AV - numOutliers);
*/

//    cout << avRead << endl;

    float normRead = (avRead - avL) / (normFactor);

/*    
    std::cout << "Read Buffer: " << std::endl;
    for (int i = 0; i < NUM_SAMPLES_TO_AV; i++){
        std::cout << readBuff[i] << std::endl;
    }
*/   
 
/*   
    std::cout << "av: " << std::endl;
    std::cout << avRead << std::endl;
    std::cout << "normalized av: " << std::endl;
    std::cout << normRead << std::endl;
*/
    return normRead;
}

int PWMManager::writeChannel(int channelIt, float normSpeed){
    float zeroPointDutyCycle = ((MIN_DUTY_CYCLE + MAX_DUTY_CYCLE)/2);
    float dutyCycleSpreadRad = (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE)/2;
    int speed = (int) ((zeroPointDutyCycle + dutyCycleSpreadRad*normSpeed) * MAX_OUT_PWM);
    pwmWrite(PIN_BASE + channelIt, speed);
    return(0);
}

void PWMManager::setInt(int channel){
    pinMode(channel, INPUT);
    //wiringPiISR(channel, INT_EDGE_BOTH, PWMInHandler);  
    return;
}

void PWMManager::stopInt(int channel){
    switch(channel){
        case 0:
             system("gpio unexport 0");
             break;
        case 3:
            system("gpio unexport 3");
            break;
        case 7:
            system("gpio unexport 7");
            break;
        default:
            std::cout << "An unrecognized pin PWM pin is being used" << std::endl;
            break;
    }
    return;
}


int PWMManager::minLengthPWM(int ind){ //ind for index in inputChannel List
    switch(ind){
        case 0:
            return 1000;
        case 1:
            return 1000;
        case 2:
            return 1000;
        default:
            std::cout << "Index not recognized in minLengthPWM" << std::endl;
    }


/*
    switch(ind){
        case 0:
            return 18230;
        case 1:
            return 18230;
        case 2:
            return 18230;
        default:
            std::cout << "Index not recognized in minLengthPWM" << std::endl;
    }
*/    
    return 0;
}

int PWMManager::maxLengthPWM(int ind){
    switch(ind){
        case 0:
            return 2050;
        case 1:
            return 2050;
        case 2:
            return 2050;
        default:
            std::cout << "Index not recognized in minLengthPWM" << std::endl;
    }
    
    // value for old config: ~ 19100
    return 0;
}
