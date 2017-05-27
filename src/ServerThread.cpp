//
// Created by hmenn on 29.04.2017.
//

#include "../include/ServerThread.h"
#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"
#include "../include/ArduinoDriver.h"
#include "../include/ProccesImage.h"


void *serverJobs(void *args) {

  int socketFD;
  char *msg;
  int command;
  bool run = true;
  int tempI;
  char tempCh;
  int XStep, YStep;
  ArduinoDriver *myArduino = (ArduinoDriver *) args;

  //  fprintf(LOG_FD, "Server Thread started.");

  ConnectionHelper connectionHelper;

  try {

    while (1) {

      socketFD = connectionHelper.acceptConnection();
      //fprintf(LOG_FD, "Thread connected socketfd:%d\n", socketFD);
      run = 1;

      msg = connectionHelper.readSocket(Constants::COMMAND_MSG_SIZE);
      //  printf("Msg:%s\n", msg);
      if (strcmp(msg, "H") == 0) {
        int si = connectionHelper.writeSocket("S");
        //   printf("%d\n", si);
      }
      // printf("HandShake!!\n");


      while (run) {
        cerr<<"CONTROL"<<endl;
        usleep(500000);
        msg = connectionHelper.readSocket(Constants::COMMAND_MSG_SIZE);
        //cerr << "deneme" << msg << endl;

        sscanf(msg, "%d", &command);
        //fprintf(LOG_FD, "-->Command:%d\n", command);
        switch (command) {
          case Constants::REQ_CLOSE_CONNECTION: {
            connectionHelper.releaseConnection();
            //fprintf(LOG_FD, "Client connection closed.\n");
            run = false;
            break;
          }
          case Constants::REQ_ASK_CURRENT_IMAGE: {
            char msg[Constants::MIN_BUFFER_SIZE];
            std::vector<uchar> buff;//buffer for coding
            std::vector<int> param(2);

            vector<int> compressionParams;
            compressionParams.push_back((int &&) CV_IMWRITE_JPEG_QUALITY);
            compressionParams.push_back(25);

            // param[0] = cv::IMWRITE_JPEG_QUALITY;
            // param[1] = 50;//default(95) 0-100
            if (realFrameL.size() > 0) {
              cv::imencode("*.jpg", realFrameL.at(realFrameL.size() - 1).getImage(), buff,
                           compressionParams);

            }


            bzero(msg, Constants::MIN_BUFFER_SIZE);
            sprintf(msg, "%d", (int) buff.size());
            int a = connectionHelper.writeSocket(msg);
            //cerr<<"Size:"<<msg;

            //imshow("tst",fm->getImage());
            /*FILE* n = fopen("/home/hmenn/Desktop/x.jpg", "w");
            for(int i=0; i<buff.size();++i){
              fprintf(n, "%c", buff.at(i));

            }

            fclose(n);*/

            int b = 0;
            for (int i = 0; i < buff.size(); ++i) {
              // ar[i] = buff.at(i);uff
              usleep(1);
              char k = buff.at(i);
              b += connectionHelper.writeSocket1(&k);
              //cerr << "yazdım : " << b << " dger  " << (int) buff.at(i) << " gercek " << buff.at(i) << endl;
            }


            break;
          }
          case Constants::REQ_ASK_CURRENT_COORDS: {
            int x = myArduino->getX();
            int y = myArduino->getY();
            char buffer[Constants::MIN_BUFFER_SIZE];
            bzero(buffer, Constants::MIN_BUFFER_SIZE);
            //sprintf(buffer, "%d,%d.", x, y);
            if (!finishFlag)
              sprintf(buffer, "%d,%d,%d.", x, y, -1);
            else {
              sprintf(buffer, "%d,%d,%d.", x, y, foundAngle);
              foundAngle = -1;
            }
            int size = connectionHelper.writeSocket(buffer);
            //fprintf(LOG_FD, "SocketWriteSize:%d\n", size);
            break;
          }
          case Constants::REQ_UPDATE_COORDS: {
            sscanf(msg, "%d%c%d%c%d", &tempI, &tempCh, &XStep, &tempCh, &YStep);
            // printf("Update X:%d, Y:%d\n", XStep, YStep);

            //  connectionHelper.writeSocket("OK");
            myArduino->step(XStep, YStep);
            usleep(3000);
            cout << myArduino->readString();
            break;
          }
          case Constants::REQ_MANUAL_MODE: {
            int mode;
            sscanf(msg, "%d%c%d", &tempI, &tempCh, &mode);
            if (mode) {
              // fprintf(LOG_FD, "Manual mode opened!\n");
              flag = true;
               
              pthread_mutex_trylock(&realFrameClearMutex);
              Frame temp = realFrameL.at(realFrameL.size() - 1);
              realFrameL.pop_back();
              realFrameL.clear();
              realFrameL.push_back(temp);
              pthread_mutex_unlock(&realFrameClearMutex);

            } else {
              //fprintf(LOG_FD, "Manual mode closed!\n");
              flag = false;
              finishFlag = false;
            }
            //connectionHelper.writeSocket("OK");
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

