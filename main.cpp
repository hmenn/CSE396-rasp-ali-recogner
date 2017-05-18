#include <thread>
#include "include/Requirements.h"
#include "include/ProccesImage.h"
#include "include/ConnectionHelper.h"
#include "include/ServerThread.h"
#include "include/ArduinoDriver.h"

using namespace cv;
using namespace std;

pthread_t thServer;
pthread_mutex_t mtxServer;
ArduinoDriver *myArduino;

int foundAngle;

void otomat(ProcessImage& processImage);

bool flag = false;
bool finishFlag=false;
int main() {

  try {

    ProcessImage processImage = ProcessImage::getInstance();
    processImage.openCamera(0);
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
    //pthread_create(&thServer, NULL, serverJobs, myArduino);

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
              otomat(processImage);
              //realFrameL.clear();
             // cerr<<"cikti";
          }
          else {



          }

      }
    for (int i = 0; i < 20; ++i) {
        if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {
            cout << "Angle:" << processImage.rotasyon(processImage.getLastImage());
            cerr << "Found" << endl;
        }
            break;
        }

    return 0;

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
                 if(a>0)
                     foundAngle=a;
                else
                     foundAngle=0;
                cerr << "Angle:" << foundAngle;
                cerr << "Found" << endl;

              /*  myArduino->step((-katsayi*75),0);
                usleep(2000);
                cout << myArduino->readString();*/
                finishFlag = true;
                break;
            }
            if(finishFlag)
                return;
            myArduino->step((katsayi*70), 0);
            usleep(2000);
            cout << myArduino->readString();
        }

        if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {
            a=processImage.rotasyon(processImage.getLastImage());

            if(a>0)
                foundAngle=a;
            else
                foundAngle=0;
            cerr << "Angle:" << foundAngle;
            cerr << "Found" << endl;
           /* myArduino->step((-katsayi*75),0);
            usleep(2000);
            cout << myArduino->readString();*/
            finishFlag = true;
            break;
        }
        if(finishFlag)
            return;

        myArduino->step(0, 125);
        usleep(2000);
        cout << myArduino->readString();
        if (processImage.takeImage(myArduino->getX(), myArduino->getY())) {
            a=processImage.rotasyon(processImage.getLastImage());

            if(a>0)
                foundAngle=a;
            else
                foundAngle=0;
            cerr << "Angle:" << foundAngle;
            cerr << "Found" << endl;
/*
            myArduino->step(0,-125);
            usleep(2000);
            cout << myArduino->readString();*/
            finishFlag = true;
            break;
        }
        if(finishFlag)
            return;
    }
    //finishFlag=false;
    //myArduino->step(-7000,-7000);
    //usleep(1000);
    //cout<<myArduino->readString();
    return;
}