//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "../include/arduino.h"
arduino::arduino( unsigned short int port,unsigned int baudRate) {
    this->port=port;
    this->baudRate=baudRate;
    connected=false;
    xCor=0;
    yCor=0;
}
arduino::~arduino() {
    disconnect();
}
bool arduino::connect() {
    if(RS232_OpenComport(port, baudRate, mode)){
        std::cerr<<"Arduino can not connected\n";
        return connected=false;
    }
    std::cerr<<"Arduino connected\n";
        step(30,130);
        xCor=0;
        yCor=0;
        connected=true;
    return connected;
}
void arduino::disconnect() {

    RS232_CloseComport(port);
    connected=false;
}
int arduino::sendByte(unsigned char byte) {
    int returnV = RS232_SendByte(port,byte);
    usleep(1102000);
    return returnV;
}
int arduino::sendBuf(unsigned char *buf, int size) {
    int returnV = RS232_SendBuf(port,buf,size);
    //usleep(1102000);
    return returnV;
}
void arduino::sendBuf(const char *buf) {
    RS232_cputs(port,buf);
    //usleep(1102000);
    return;
}
int arduino::receive(unsigned char *buf, int size) {
    return RS232_PollComport(port,buf,size);
}
void arduino::stepX(int step) {
    char buffer[25];
    if(step>0){
        // alltakiler guvenlik amacli kaldirilabilir
        if(xMax>=step+xCor){
            xCor+=step;
        }
        else{
            step=xMax-xCor;
            xCor=xMax;
        }
    }
    else{

        if(xCor+step>=0){
            xCor+=step;
        }
        else{
            step=-xCor;
            xCor=0;
        }

    }
    sprintf(buffer,"X=%d",step);
    sendBuf((unsigned char*)buffer,strlen(buffer));
    usleep(1102000);
}
void arduino::stepY(int step) {
    char buffer[25];
    if(step>0){
        // alltakiler guvenlik amacli kaldirilabilir
        if(yMax>=step+yCor){
            yCor+=step;
        }
        else{
            step=yMax-yCor;
            yCor=yMax;
        }
    }
    else{
        if(yCor+step>=0){
            yCor+=step;
        }
        else{
            step=-yCor;
            yCor=0;
        }

    }
    sprintf(buffer,"Y=%d",step);
    sendBuf((unsigned char*)buffer,strlen(buffer));
    usleep(1102000);
}
void arduino::step(int xStep, int yStep) {
    char buffer[25];
    if(xStep>0){
        // alltakiler guvenlik amacli kaldirilabilir
        if(xMax>=xStep+xCor){
            xCor+=xStep;
        }
        else{
            xStep=xMax-xCor;
            xCor=xMax;
        }
    }
    else{

        if(xCor+xStep>=0){
            xCor+=xStep;
        }
        else{
            xStep=-xCor;
            xCor=0;
        }

    }
    if(yStep>0){
        // alltakiler guvenlik amacli kaldirilabilir
        if(yMax>=yStep+yCor){
            yCor+=yStep;
        }
        else{
            yStep=yMax-yCor;
            yCor=yMax;
        }
    }
    else{
        if(yCor+yStep>=0){
            yCor+=yStep;
        }
        else{
            yStep=-yCor;
            yCor=0;
        }

    }
    sprintf(buffer,"X=%d,Y=%d",xStep,yStep);
    sendBuf((unsigned char*)buffer,strlen(buffer));
    //usleep(1102000);
}