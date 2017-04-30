//
// Created by hmenn on 29.04.2017.
//

#include "../include/ServerThread.h"
#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"

void *serverJobs(void *args) {

  int socketFD;
  char *msg;
  int command;
  bool run = true;
  int tempI;
  char tempCh;
  int XStep, YStep;

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

        fprintf(LOG_FD, "Command:%d\n", command);
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
            int size = connectionHelper.writeSocket("2 3");
            fprintf(LOG_FD, "Write:%d\n", size);
            break;
          }
          case Constants::REQ_UPDATE_COORDS: {
            sscanf(msg, "%d%c%d%c%d", &tempI, &tempCh, &XStep, &tempCh, &YStep);
            printf("Read X:%d, Y:%d\n", XStep, YStep);
            connectionHelper.writeSocket("OK");
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

