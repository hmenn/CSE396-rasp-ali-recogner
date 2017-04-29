#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"
#include "include/arduino.h"
using namespace cv;
using namespace std;
using namespace Group5;

bool search = true;
bool isClientConnected = false;
bool isArduinoConnected = false;

pthread_t thServer;
pthread_mutex_t mtxServer;

int main() {

  try{

   // pthread_create(&thServer,NULL,serverJobs,NULL);
    //fprintf(LOG_FD,"Server thread created.\n");
    char buf[50];
      sprintf(buf,"alperen!!!");
    // vision operations
    // arduino connections
    arduino myArduino(25,9600);
      myArduino.connect();
    RS232_enableRTS(25);

      myArduino.sendBuf((unsigned char*)buf,50);

    while(RS232_IsDCDEnabled(25));

      myArduino.disconnect();
    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();



   // pthread_join(thServer,NULL);
  }catch(exception &e){
    cout<<e.what();
  }

    return 0;
}















