//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "rs232.h"
#include <iostream>
#include "SerialPort.h"
#include "../include/Requirements.h"
#ifndef CSE396_RASP_ALI_RECOGNER_ARDUINO_H
#define CSE396_RASP_ALI_RECOGNER_ARDUINO_H
#define SERIALPORT 1
//#define RS232 2
class arduino {

public:
    arduino(unsigned short int port, unsigned int baudRate);
    arduino(const std::string& port);
    ~arduino();

    bool connect();

    void disconnect();

    int sendByte(unsigned char byte);

    int sendBuf(unsigned char *buf, int size);

    void sendBuf(const char *buf);

    int receive(unsigned char *buf, int size);

    bool isConnected() { return connected; }

    int getX() { return xCor; }

    int getY() { return yCor; }

    void stepX(int step);

    void stepY(int step);

    void step(int xStep, int yStep);

private:
    unsigned short int port;
    std::string portName;
    unsigned int baudRate;
    const char mode[4] = {'8', 'N', '1', '\0'};
    bool connected;
    int xCor;
    int yCor;
    const int xMax = 700;
    const int yMax = 480;
#ifdef SERIALPORT
    SerialPort *mySerial;
#endif
};

#endif //CSE396_RASP_ALI_RECOGNER_ARDUINO_H
/*
 * http://www.teuniz.net/RS-232/
 *  usleep(1102000);*/
