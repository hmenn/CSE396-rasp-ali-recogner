//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "../include/ArduinoDriver.h"
#include <string>

using namespace std;


ArduinoDriver::ArduinoDriver(SerialPort::BaudRate baudRate) {
  this->port = "/dev/ttyUSB0";
  this->baudRate = baudRate;
  connected = false;
  xCor = 0;
  yCor = 0;
  serialPort = NULL;
}

ArduinoDriver::~ArduinoDriver() {
  if (serialPort != NULL)
    delete (serialPort);
}

bool ArduinoDriver::connect() {

  serialPort = new SerialPort(this->port, this->baudRate);
  if (!serialPort->open()) {
    cout << "Unable to connect /dev/ttyUSB0" << endl;
    serialPort->setPortName("/dev/ttyUSB1");
    if (!serialPort->open()) {
      cout << "Unable to connect /dev/ttyUSB1" << endl;
        connected=false;
      return false;
    }
  }
  if (!(serialPort->prepare())) {
    usleep(100);
    cerr << "Prepare failed" << endl;
      connected=false;
    return false;
  }
    connected=true;
  return serialPort->makeHandshake();
}

void ArduinoDriver::disconnect() {
  serialPort->close();
  connected = false;
}


void ArduinoDriver::step(int xStep, int yStep) {
int tempX=xCor,tempY=yCor;
 /* if (xStep > 0) {
    // alltakiler guvenlik amacli kaldirilabilir
    if (xMax >= xStep + xCor) {
      xCor += xStep;
    } else {
      xStep = xMax - xCor;
      xCor = xMax;
    }
  } else {

    if (xCor + xStep >= 0) {
      xCor += xStep;
    } else {
      xStep = -xCor;
      xCor = 0;
    }

  }
  if (yStep > 0) {
    // alltakiler guvenlik amacli kaldirilabilir
    if (yMax >= yStep + yCor) {
      yCor += yStep;

    } else {
      yStep = yMax - yCor;
      yCor = yMax;

    }
  } else {
    if (yCor + yStep >= 0) {
      yCor += yStep;
    } else {
      yStep = -yCor;
      yCor = 0;
    }
  }*/
    if(xStep==-5987 && yStep==-5987){
        xCor=0;
        yCor=0;
        tempX=0;
        tempY=0;
    }

        else
    {
        if (xStep > 0) {
            // alltakiler guvenlik amacli kaldirilabilir
            if (xMax >= xStep + xCor) {
                tempX += xStep;
            } else {
                xStep = xMax - xCor;
                tempX = xMax;
            }
        } else {

            if (xCor + xStep >= 0) {
                tempX += xStep;
            } else {
                xStep = -xCor;
                tempX = 0;
            }

        }
        if (yStep > 0) {
            // alltakiler guvenlik amacli kaldirilabilir
            if (yMax >= yStep + yCor) {
                tempY += yStep;

            } else {
                yStep = yMax - yCor;
                tempY = yMax;

            }
        } else {
            if (yCor + yStep >= 0) {
                tempY += yStep;
            } else {
                yStep = -yCor;
                tempY = 0;
            }
        }
    }
  /*  std::cerr<<"xStep= "<<xStep<<" xCor= "<<xCor<<std::endl;
    std::cerr<<"yStep= "<<yStep<<" yCor= "<<yCor<<std::endl;*/
  char msg[Constants::MAX_BUFFER_SIZE];
  bzero(msg, Constants::MAX_BUFFER_SIZE);
  sprintf(msg, "X=%d,Y=%d.", xStep, yStep);
    cerr<<msg<<endl;
  while (!serialPort->writeString(msg));

    xCor=tempX;
    yCor=tempY;
  //std::cout << "Write Complete" << std::endl;
}

string ArduinoDriver::readString() {
  std::string str;
  while (!serialPort->readUntil(str, '.'));
  return str;
}