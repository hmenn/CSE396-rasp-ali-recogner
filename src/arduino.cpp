//
// Created by Ahmet Alperen Bulut on 29.04.2017.
//
#include "../include/arduino.h"
arduino::arduino( unsigned short int port,unsigned short int baudRate) {
    this->port=port;
    this->baudRate=baudRate;
    connected=false;

}
arduino::~arduino() {
    disconnect();
}
bool arduino::connect() {
    if(RS232_OpenComport(port, baudRate, mode)){
        std::cerr<<"Arduino can not connected\n";
        connected=false;
    }
    std::cerr<<"Arduino connected\n";
        connected=true;
    return connected;
}
void arduino::disconnect() {

    RS232_CloseComport(port);
    connected=false;
}
int arduino::sendByte(unsigned char byte) {
    return RS232_SendByte(port,byte);
}
int arduino::sendBuf(unsigned char *buf, int size) {
    return RS232_SendBuf(port,buf,size);
}
void arduino::sendBuf(const char *buf) {
     RS232_cputs(port,buf);
}
int arduino::receive(unsigned char *buf, int size) {
    return RS232_PollComport(port,buf,size);
}
