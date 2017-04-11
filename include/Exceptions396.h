//
// Created by hmenn on 11.04.2017.
//

#ifndef EXCEPTIONS396_H
#define EXCEPTIONS396_H

#include <exception>

using std::exception;

class InvalidConnectionException: public exception{
public:
    const char *what()const throw(){
      return "Connection error. Please check IP!";
    }
};

class SocketCreationException: public exception{
public:
    const char *what()const throw(){
      return "Socket Creation error.";
    }
};

#endif //CSE396_RASP_ALI_RECOGNER_EXCEPTIONS396_H
