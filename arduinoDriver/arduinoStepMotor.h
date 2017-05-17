//
// Created by Ahmet Alperen BULUT on 25.04.2017.
//

#ifndef ARDUINOSTEPMOTOR_H
#define ARDUINOSTEPMOTOR_H
#include <Arduino.h>
//enum speed{fastest=500,fast=1000,mid=2000,slow=3000,slowest=5000};
class arduinoStepMotor {

    public:
        arduinoStepMotor(unsigned int maxSteps,unsigned short directionPin,unsigned short stepperPin,unsigned short enablePin);
        arduinoStepMotor(unsigned int maxSteps,unsigned short directionPin,unsigned short stepperPin);
        void steps(bool dir,unsigned int steps);
        void steps(int steps);
        void setSpeed(unsigned int selectSpeed);
        unsigned int getSpeed(void);
        unsigned int getCurrentStep(void);
        void enable(void);
        void disable(void);
        void reset(void);
        int currentStep;
    private:
        int maxSteps;
        unsigned short enablePin;
        unsigned short directionPin;
        unsigned short stepperPin;
        
        unsigned int speed;
        void init();
};


#endif //ARDUINOSTEPMOTOR_H

