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

    pthread_create(&thServer,NULL,serverJobs,NULL);

    while(1){

    }
    // vision operations
    // arduino connections
    arduino myArduino(24,115200);
      myArduino.connect();
      int i=0;
      while(1) {
          if(i%3==0)
          myArduino.step(500,200);
          else
              myArduino.step(-200,-200);
          ++i;

      }
      myArduino.disconnect();


    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();



   // pthread_join(thServer,NULL);
  }catch(exception &e){
    cout<<e.what();
  }

    return 0;
}















