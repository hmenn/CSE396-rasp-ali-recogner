#ifndef REQUIREMENTS396
#define REQUIREMENTS396

#include <iostream>
#include <unistd.h>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Exceptions396.h"
#include "Constants.h"
#include "Frame.h"

#define LOG_FD stdout
extern bool flag;
extern pthread_mutex_t realFrameClearMutex;
extern Frame fm;
extern bool finishFlag;
extern int foundAngle;
#endif