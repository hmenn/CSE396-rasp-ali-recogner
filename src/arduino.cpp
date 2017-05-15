//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "../include/arduino.h"
#include <cstring>
arduino::arduino( unsigned short int port,unsigned int baudRate) {
    this->port=port;
    this->baudRate=baudRate;
    connected=false;
    xCor=0;
    yCor=0;

}
arduino::arduino(const std::string& port) {
    this->portName=port;
    this->baudRate=baudRate;
    connected=false;
    xCor=0;
    yCor=0;
    mySerial=new SerialPort(port,SerialPort::BR_115200);
}
arduino::~arduino() {
#ifdef RS232
    disconnect();
#endif
#ifdef SERIALPORT
    delete(mySerial);
#endif
}
bool arduino::connect() {
#ifdef RS232
    if(RS232_OpenComport(port, baudRate, mode)){
        std::cerr<<"Arduino can not connected\n";
        return connected=false;
    }
    std::cerr<<"Arduino connected\n";
    sleep(2);//connect icin gerekli
    step(30,130);
#endif

#ifdef SERIALPORT
    mySerial->open();
    std::cerr<<"Arduino connected\n";

#endif
    xCor=0;
    yCor=0;
    connected=true;
    return connected;
}
void arduino::disconnect() {
#ifdef RS232
    RS232_CloseComport(port);
#endif
#ifdef SERIALPORT
    mySerial->close();
#endif
    connected=false;
}
int arduino::sendByte(unsigned char byte) {
#ifdef RS232
    int returnV = RS232_SendByte(port,byte);
    usleep(1102000);
#endif
#ifdef SERIALPORT
    int returnV=mySerial->write(byte);
#endif
    return returnV;
}
int arduino::sendBuf(unsigned char *buf, int size) {
#ifdef RS232
    int returnV = RS232_SendBuf(port,buf,size);
    usleep(1102000);

#endif
#ifdef SERIALPORT
    int returnV = mySerial->write(*buf);
#endif
    return returnV;
}
void arduino::sendBuf(const char *buf) {
#ifdef RS232
    RS232_cputs(port,buf);
    usleep(1102000);
#endif
#ifdef SERIALPORT
    mySerial->writeString(std::string(buf));
#endif
    return;
}
int arduino::receive(unsigned char *buf, int size) {
#ifdef RS232
    return RS232_PollComport(port,buf,size);
#endif
#ifdef SERIALPORT
    return mySerial->read((char*) buf);
#endif
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
#ifdef RS232
    sendBuf((unsigned char*)buffer,strlen(buffer));
    usleep(1102000);
#endif
#ifdef SERIALPORT
    mySerial->write(buffer);
#endif
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
#ifdef RS232
    sendBuf((unsigned char*)buffer,strlen(buffer));
    usleep(1102000);
#endif
#ifdef SERIALPORT
    mySerial->write(buffer);
#endif
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
   // std::cerr<<buffer<<std::endl;
#ifdef RS232
    std::cerr<<sendBuf((unsigned char*)buffer,strlen(buffer))<<std::endl;
    usleep(1102000);
#endif
#ifdef SERIALPORT
    std::cerr<<mySerial->write(buffer)<<std::endl;
    memset(buffer,0,25);
    mySerial->read(buffer);
    std::cerr<<buffer<<std::endl;
    // TODO : CHECH EKSİK
#endif
}