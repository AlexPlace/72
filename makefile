OBJS = testMain.o PWMManager.o InputManager.o pca9685.o RobotCar.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

testMain : $(OBJS)
	$(CC) $(LFLAGS) $(OBJS) -o testMain -l wiringPi -l pthread

PWMManager.o : PWMManager.h
	$(CC) $(CFLAGS) PWMManager.cpp -l wiringPi -l pthread -l wiringPiPca9685

InputManager.o : InputManager.h
	$(CC) $(CFLAGS) InputManager.cpp
	
pca9685.o : pca9685.h
	$(CC) $(CFLAGS) pca9685.c
	
RobotCar.o : RobotCar.h
	$(CC) $(CFLAGS) RobotCar.cpp

testMain.o : 
	$(CC) $(CFLAGS) testMain.cpp

clean:
	\rm *.o *~ testMain