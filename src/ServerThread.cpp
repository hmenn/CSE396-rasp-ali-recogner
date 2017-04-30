//
// Created by hmenn on 29.04.2017.
//

#include "../include/ServerThread.h"
#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"
#include "../include/arduino.h"


void *serverJobs(void *args) {

  int socketFD;
  char *msg;
  int command;
  bool run = true;
  int tempI;
  char tempCh;
  int XStep, YStep;
  arduino *myArduino = (arduino *)args;

  fprintf(LOG_FD, "Thread started.");

  ConnectionHelper connectionHelper;


  try {

    while (1) {

      socketFD = connectionHelper.acceptConnection();
      fprintf(LOG_FD, "Thread connected socketfd:%d\n", socketFD);
      run=1;

      while (run) {
        msg = connectionHelper.readSocket(Constants::COMMAND_MSG_SIZE);
        sscanf(msg, "%d", &command);

        fprintf(LOG_FD, "Readded command from socket:%d\n", command);
        switch (command) {
          case Constants::REQ_CLOSE_CONNECTION: {
            connectionHelper.releaseConnection();
            fprintf(LOG_FD, "Client connection closed.\n");
            run = false;
            break;
          }
          case Constants::REQ_ASK_CURRENT_IMAGE: {
            fprintf(LOG_FD, "SEND IMAGE WILL IMPLEMENT LATER!!!\n");
            break;
          }
          case Constants::REQ_ASK_CURRENT_COORDS: {
            int x=myArduino->getX();
            int y=myArduino->getY();
            char buffer[Constants::MIN_BUFFER_SIZE];
            sprintf(buffer,"%d %d",x,y);
            int size = connectionHelper.writeSocket(buffer);
            fprintf(LOG_FD, "SocketWriteSize:%d\n", size);
            break;
          }
          case Constants::REQ_UPDATE_COORDS: {
            sscanf(msg, "%d%c%d%c%d", &tempI, &tempCh, &XStep, &tempCh, &YStep);
            printf("Update X:%d, Y:%d\n", XStep, YStep);
            myArduino->step(XStep,YStep);
            connectionHelper.writeSocket("OK");
            break;
          }case Constants::REQ_MANUAL_MODE: {
            int mode;
            sscanf(msg, "%d%c%d", &tempI,&tempCh,&mode);
            if(mode) fprintf(LOG_FD,"Manual mode opened!\n");
            else fprintf(LOG_FD,"Manual mode closed!\n");
            connectionHelper.writeSocket("OK");
            // CLOSE ARDUINO CONTROL FROM IMAGE PROCESS
            break;
          }
          default: {
            fprintf(LOG_FD, "Invalid command");
            break;
          }
        }
        free(msg);
      }
    }
  } catch (exception &e) {
    fprintf(LOG_FD, "Exception in thread: %s\n", e.what());
  }


  pthread_exit(NULL);
}

