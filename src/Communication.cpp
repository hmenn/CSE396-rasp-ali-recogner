//
// Created by sveyda on 28.03.2017.
//

#include <iostream>
#include "../include/Communication.h"

using namespace Group5;

CommunicationWithArduino::CommunicationWithArduino(){

    connectArduino();
}

CommunicationWithArduino::~CommunicationWithArduino()
{
    RS232_CloseComport(port_number);
}

void CommunicationWithArduino::connectArduino() {

    if(!RS232_OpenComport(port_number, baud_rate, mode)){
        isConnected = true;
    }else{
        std::cerr<<" Sorry,There is a problem! I cannot connect the arduino;"<<std::endl;
        throw -1;
    }
}

int CommunicationWithArduino::sendArduino(std::string value) {
    //TODO sendArduino to message send byte byte
    //TODO string formati belirlenecek
    //while(x)
   //RS232_SendByte()

    return -999;
}

void CommunicationWithArduino::recieveArduino() {
    //TODO ?
}