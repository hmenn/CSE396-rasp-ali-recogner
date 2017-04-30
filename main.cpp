#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"

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

    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();



    pthread_join(thServer,NULL);
  }catch(exception &e){
    cout<<e.what();
  }

    return 0;
}















