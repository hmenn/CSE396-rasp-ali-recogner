#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"
#include "include/arduino.h"

using namespace cv;
using namespace std;

pthread_t thServer;
pthread_mutex_t mtxServer;
arduino *myArduino;


void otomat();

bool flag = false;

void run(ProcessImage & camera);

int main() {

  try{
#ifdef RS232
    myArduino = new arduino(16,115200);
#endif

#ifdef SERIALPORT
    myArduino = new arduino("/dev/ttyUSB1");
#endif
    ProcessImage camera = ProcessImage::getInstance();
    myArduino->connect();


    camera.openCamera(0);
    camera.takeImage(0,0);
    camera.writeToFile("/home/hmenn/Desktop/");

    pthread_create(&thServer, NULL, serverJobs, myArduino);
    // vision operations
    // arduino connections

    while (1) {
      if (!flag)
        otomat();
      sleep(2);
    }

    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();
    // pthread_join(thServer,NULL);
  } catch (exception &e) {
    cout << e.what();
  }

  return 0;
}

void otomat() {
  myArduino->step(1000, 240);
  myArduino->step(-1000, 240);
  myArduino->step(1000, 0);

  sleep(2);

  myArduino->step(-1000, -240);
  myArduino->step(1000, -240);
  myArduino->step(-1000, 0);

  sleep(3);
}

void run(ProcessImage & camera) {

  for (int i = 0; i < 1000; i+=20) {
  }


}













