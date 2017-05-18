// Created by Ahmet Alperen Bulut on 29.04.2017.
#include <iostream>
#include "SerialPort.h"
#include "../include/Requirements.h"

#ifndef CSE396_RASP_ALI_RECOGNER_ARDUINO_H
#define CSE396_RASP_ALI_RECOGNER_ARDUINO_H

using namespace std;

class ArduinoDriver {

public:
    ArduinoDriver(SerialPort::BaudRate baudRate);

    ~ArduinoDriver();

    bool connect();

    void disconnect();

    void step(int xStep, int yStep);

    string readString();

    int getX() { return xCor; }

    int getY() { return yCor; }
    int setX(int xCor){this->xCor=xCor;}
    int setY(int yCor){this->yCor=yCor;}



private:
    std::string port;
    SerialPort::BaudRate baudRate;
    bool connected;
    int xCor;
    int yCor;
    const int xMax = 700;//700
    const int yMax = 520;//480
    SerialPort *serialPort;
};

#endif //CSE396_RASP_ALI_RECOGNER_ARDUINO_H
