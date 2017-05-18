//
// Created by Ahmet Alperen BULUT on 25.04.2017.
//

#include "arduinoStepMotor.h"

arduinoStepMotor::arduinoStepMotor(unsigned int maxSteps,unsigned short directionPin,unsigned short stepperPin,unsigned short enablePin){
    this->maxSteps=maxSteps;
    this->directionPin=directionPin;
    this->stepperPin=stepperPin;
    this->currentStep=0;
    this->speed=5000;//default
    this->enablePin=enablePin;
    init();
}

arduinoStepMotor::arduinoStepMotor(unsigned int maxSteps,unsigned short directionPin,unsigned short stepperPin){
    this->maxSteps=maxSteps;
    this->directionPin=directionPin;
    this->stepperPin=stepperPin;
    this->currentStep=0;
    this->speed=5000;//default
    this->enablePin=0;
    init();
}

void arduinoStepMotor::init() {

    pinMode(this->directionPin,OUTPUT);
    digitalWrite(this->directionPin,HIGH);
    pinMode(this->stepperPin,OUTPUT);
    digitalWrite(this->stepperPin,LOW);
    pinMode(this->enablePin,OUTPUT);
    digitalWrite(this->enablePin,HIGH);//disable

}

void arduinoStepMotor::steps(bool dir, unsigned int steps) {

    digitalWrite(this->directionPin,dir);

    delay(10);//datasheette yazmıyor sallama bi deger

    if(dir)
    {
        if(this->maxSteps>=steps+this->currentStep)
            this->currentStep+=steps;
        else{//eger steps parametresi gidebileceğim uc noktadan daha uzaga gitmeye calisiyorsa en fazla maxstepe kadar gidiyor fazlasına izin vemriyr
            steps=this->maxSteps-this->currentStep;
            this->currentStep=this->maxSteps;
        }
    }
    else
    {
        if(this->currentStep-steps>=0)
            this->currentStep-=steps;
        else
        {
            steps=this->currentStep;
            this->currentStep=0;
        }

    }

    for(int i=0;i<steps;++i){
        digitalWrite(this->stepperPin,HIGH);
        delayMicroseconds(this->speed);//delayMicroseconds
        digitalWrite(this->stepperPin,LOW);
        delayMicroseconds(this->speed/2);
    }

}

void arduinoStepMotor::steps(int steps) {

    if(steps>=0){//pozitif
        digitalWrite(this->directionPin,HIGH);
        if(this->maxSteps>=steps+this->currentStep)
            this->currentStep+=steps;
        else{//eger steps parametresi gidebileceğim uc noktadan daha uzaga gitmeye calisiyorsa en fazla maxstepe kadar gidiyor fazlasına izin vemriyr
            steps=this->maxSteps-this->currentStep;
            this->currentStep=this->maxSteps;
        }
    }
    else{//negatif
        digitalWrite(this->directionPin,LOW);
        
        if(this->currentStep+steps>=0){
            this->currentStep+=steps;
            steps=-steps;
        }
        else
        {
            steps=this->currentStep;
            this->currentStep=0;
        }
    }
    delay(5);
    for(int i=0;i<steps;++i){
        digitalWrite(this->stepperPin,HIGH);
        delayMicroseconds(this->speed);//bunda uzun  kalacak altta kısa bunlarıda default ayarlayıp adamın dısardan girdigi degere gore ayarla
        digitalWrite(this->stepperPin,LOW);
        delayMicroseconds(this->speed);
    }

}

void arduinoStepMotor::reset(void){

}

void arduinoStepMotor::setSpeed(unsigned int selectSpeed) {
    this->speed=selectSpeed;
}

unsigned int arduinoStepMotor::getSpeed(void) {
    return this->speed;
}

unsigned int arduinoStepMotor::getCurrentStep(void) {
    return this->currentStep;
}

void arduinoStepMotor::enable() {
    digitalWrite(this->enablePin,LOW);//enable
}

void arduinoStepMotor::disable() {
    digitalWrite(this->enablePin,HIGH);//disable
}

