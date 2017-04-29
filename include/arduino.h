//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "rs232.h"
#include <iostream>
#ifndef CSE396_RASP_ALI_RECOGNER_ARDUINO_H
#define CSE396_RASP_ALI_RECOGNER_ARDUINO_H
class arduino{

public:
    arduino( unsigned short int port,unsigned short int baudRate);
    ~arduino();
    bool connect();
    void disconnect();
    int sendByte(unsigned char byte);
    int sendBuf(unsigned char* buf,int size);
    void sendBuf(const char* buf);
    int receive(unsigned char* buf,int size);
    bool isConnected(){ return connected;}

private:
    unsigned short int port;
    unsigned short int baudRate;
    const char mode[4]={'8','N','1',0};
    bool  connected;
};
#endif //CSE396_RASP_ALI_RECOGNER_ARDUINO_H
