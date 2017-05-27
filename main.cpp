#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"
#include "include/ArduinoDriver.h"

using namespace cv;
using namespace std;

pthread_t thServer;

ArduinoDriver *myArduino;
pthread_mutex_t realFrameClearMutex = PTHREAD_MUTEX_INITIALIZER;
int foundAngle;

void otomat(ProcessImage& processImage);

bool flag = false;
bool finishFlag=false;
int main() {

  try {

    ProcessImage processImage = ProcessImage::getInstance();
    processImage.openCamera(1);
    myArduino = new ArduinoDriver(SerialPort::BR_9600);
    if (!myArduino->connect()) {
      return 0;

    }
      cout<<"Hand Shake completed!"<<endl;
      pthread_create(&thServer, NULL, serverJobs, myArduino);
      myArduino->step(120,90);
        myArduino->setX(0);
      myArduino->setY(0);
      usleep(3000);
      cout<<myArduino->readString();


      while(1){
          /*if(finishFlag)
          {
              myArduino->step(-5000,-5000);
              usleep(3000);
              cout<<myArduino->readString();
              finishFlag=false;
              sleep(2);
          }*/
          if(!flag && !finishFlag){
              pthread_mutex_lock(&realFrameClearMutex);
              otomat(processImage);
              std::cerr<<"cikti";
              pthread_mutex_unlock(&realFrameClearMutex);
              //realFrameL.clear();
             // cerr<<"cikti";
          }
          else {

          }
      }

    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();
    // pthread_join(thServer,NULL);
  } catch (exception &e) {
    cout << e.what();
  }
}
void otomat(ProcessImage& processImage){
    short int katsayi=1;
    int a;
    for (int j = 0; j < 5; ++j) {
        if(j%2==1)
            katsayi=-abs(katsayi);
        else
            katsayi=abs(katsayi);

        for (int i = 0; i <= 9; ++i) {

            if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {

                a =  processImage.rotasyon(processImage.getLastImage());
                 if(a==-1)
                 {}
                else{
                     foundAngle=a;
                cerr << "Angle:" << foundAngle;
                cerr << "Found" << endl;

              /*  myArduino->step((-katsayi*75),0);
                usleep(2000);
                cout << myArduino->readString();*/
                finishFlag = true;
                break;}
            }
            if(finishFlag)
                return;
            myArduino->step((katsayi*70), 0);
            usleep(2000);
            cout << myArduino->readString();
        }

        if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {
            a=processImage.rotasyon(processImage.getLastImage());

            if(a==-1)
            {}
            else {
                foundAngle = a;
                cerr << "Angle:" << foundAngle;
                cerr << "Found" << endl;
                /* myArduino->step((-katsayi*75),0);
                 usleep(2000);
                 cout << myArduino->readString();*/
                finishFlag = true;
                break;
            }
        }
        if(finishFlag)
            return;

        myArduino->step(0, 125);
        usleep(2000);
        cout << myArduino->readString();
        if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {
            a=processImage.rotasyon(processImage.getLastImage());

            if(a==-1)
            {}
            else {
                foundAngle = a;
                cerr << "Angle:" << foundAngle;
                cerr << "Found" << endl;
/*
            myArduino->step(0,-125);
            usleep(2000);
            cout << myArduino->readString();*/
                finishFlag = true;
                break;
            }
        }
        if(finishFlag)
            return;
    }
  if(finishFlag)
    return;
  //finishFlag=true;
  myArduino->step(-7000,-7000);
  usleep(2000);
  cout<<myArduino->readString();

    return;
}