#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"
#include "include/arduino.h"
using namespace cv;
using namespace std;
using namespace Group5;


pthread_t thServer;
pthread_mutex_t mtxServer;
arduino *myArduino;
void otomat();
bool flag=false;
int main() {

  try{
#ifdef RS232
    myArduino = new arduino(16,115200);
#endif

#ifdef SERIALPORT
    myArduino = new arduino("/dev/ttyUSB1");
#endif
    myArduino->connect();
    pthread_create(&thServer,NULL,serverJobs,myArduino);
    // vision operations
    // arduino connections

    while(1){
        if(!flag)
            otomat();
    }

    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();
   // pthread_join(thServer,NULL);
  }catch(exception &e){
    cout<<e.what();
  }

    return 0;
}

void otomat(){

        myArduino->step(1000,240);
        myArduino->step(-1000,240);
        myArduino->step(1000,0);

        sleep(2);

        myArduino->step(-1000,-240);
        myArduino->step(1000,-240);
        myArduino->step(-1000,0);

        sleep(3);
    }













