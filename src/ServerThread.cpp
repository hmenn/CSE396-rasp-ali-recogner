//
// Created by hmenn on 29.04.2017.
//

#include "../include/ServerThread.h"
#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"
#include "../include/ArduinoDriver.h"


void *serverJobs(void *args) {

  int socketFD;
  char *msg;
  int command;
  bool run = true;
  int tempI;
  char tempCh;
  int XStep, YStep;
  ArduinoDriver *myArduino = (ArduinoDriver *) args;

  fprintf(LOG_FD, "Server Thread started.");

  ConnectionHelper connectionHelper;

  try {

    while (1) {

      socketFD = connectionHelper.acceptConnection();
      fprintf(LOG_FD, "Thread connected socketfd:%d\n", socketFD);
      run = 1;

      while (run) {
        usleep(300);
        msg = connectionHelper.readSocket(Constants::COMMAND_MSG_SIZE);
        sscanf(msg, "%d", &command);
        fprintf(LOG_FD, "-->Command:%d\n", command);
        switch (command) {
          case Constants::REQ_CLOSE_CONNECTION: {
            connectionHelper.releaseConnection();
            fprintf(LOG_FD, "Client connection closed.\n");
            run = false;
            break;
          }
          case Constants::REQ_ASK_CURRENT_IMAGE: {
            char msg[10];
            std::vector<uchar> buff;//buffer for coding
            std::vector<int> param(2);

            param[0] = cv::IMWRITE_JPEG_QUALITY;
            param[1] = 50;//default(95) 0-100
            cv::imencode(".jpg", fm.getImage(), buff, param);
            bzero(msg,10);

            sprintf(msg,"%d",buff.size());
            printf("%s\n",msg);
            connectionHelper.writeSocket(msg);
            /*char *bd = (char *)buff.data();
            for(int i=0;i<buff.size();++i){
              connectionHelper.writeSocket(bd);
              ++bd;
            }*/
            fprintf(LOG_FD, "SocketImageWriteSize:%d\n", (int)buff.size());
            break;
          }
          case Constants::REQ_ASK_CURRENT_COORDS: {
            int x = myArduino->getX();
            int y = myArduino->getY();

            char buffer[Constants::MIN_BUFFER_SIZE];
            bzero(buffer, Constants::MIN_BUFFER_SIZE);
            sprintf(buffer, "%d,%d", x, y);
            int size = connectionHelper.writeSocket(buffer);
            fprintf(LOG_FD, "SocketWriteSize:%d\n", size);
            break;
          }
          case Constants::REQ_UPDATE_COORDS: {
            sscanf(msg, "%d%c%d%c%d", &tempI, &tempCh, &XStep, &tempCh, &YStep);
            printf("Update X:%d, Y:%d\n", XStep, YStep);
            connectionHelper.writeSocket("OK");
            myArduino->step(XStep, YStep);

            break;
          }
          case Constants::REQ_MANUAL_MODE: {
            int mode;
            sscanf(msg, "%d%c%d", &tempI, &tempCh, &mode);
            if (mode) {
              fprintf(LOG_FD, "Manual mode opened!\n");
              flag = true;
            } else {
              fprintf(LOG_FD, "Manual mode closed!\n");
              flag = false;
            }
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

