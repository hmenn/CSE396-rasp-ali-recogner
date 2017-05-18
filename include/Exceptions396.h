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

class ArduinoConnectionExcepiton: public exception{
public:
    const char *what()const throw(){
      return "Arduino connection error";
    }
};

typedef enum{
    CREATE_ERROR,
    BINDING_ERROR,
    LISTEN_ERROR,
    ACCEPT_ERROR
}SocketError;

class SocketCreationException: public exception{
public:
    SocketCreationException(int error){
      this->err=(SocketError)error;
    }
    const char *what()const throw(){
      switch (this->err){
        case CREATE_ERROR:
          return "Socket creation error!";
        case BINDING_ERROR:
          return "Socket bind error!";
        case LISTEN_ERROR:
          return "Socket listen error!";
        case ACCEPT_ERROR:
          return "Socket accept error!";
      }
    }

private:
    SocketError err;
};

#endif //CSE396_RASP_ALI_RECOGNER_EXCEPTIONS396_H
