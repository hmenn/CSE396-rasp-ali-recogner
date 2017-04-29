//
// Created by hmenn on 29.04.2017.
//

#include "../include/ServerThread.h"
#include "../include/ConnectionHelper.h"
#include "../include/Requirements.h"

void* serverJobs(void *args){

  printf("Thread started.");

  ConnectionHelper connectionHelper;
  connectionHelper.listenPort();

  pthread_exit(NULL);
}

