//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "rs232.h"
#include <iostream>

#ifndef CSE396_RASP_ALI_RECOGNER_ARDUINO_H
#define CSE396_RASP_ALI_RECOGNER_ARDUINO_H

class arduino {

public:
    arduino(unsigned short int port, unsigned int baudRate);

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
    unsigned int baudRate;
    const char mode[4] = {'8', 'N', '1', '\0'};
    bool connected;
    int xCor;
    int yCor;
    const int xMax = 700;
    const int yMax = 480;
};

#endif //CSE396_RASP_ALI_RECOGNER_ARDUINO_H
/*
 * http://www.teuniz.net/RS-232/
 *  usleep(1102000);*/
