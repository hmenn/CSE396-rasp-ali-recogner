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


void otomat();

bool flag = false;

void run(ProcessImage &camera);

int main() {

  try {

    ProcessImage processImage = ProcessImage::getInstance();
    processImage.openCamera(1);


    string str;
    myArduino = new ArduinoDriver(SerialPort::BR_9600);
    if(!myArduino->connect()){
      return 0;
    }

    /*cout<<"Hand Shake completed!"<<endl;
    myArduino->step(30,130);
    usleep(3000);
    cout<<myArduino->readString();*/


    /*while(1){
      processImage.takeImage(0,0);
    }*/

    for(int i=0;i<200;++i){
      myArduino->step(75,0);
      usleep(2000);

      if(processImage.takeImage(0,0)){
        cout<<"Angle:"<<processImage.rotasyon(processImage.getLastImage());
        cerr<<"Found"<<endl;
        break;
      }

      cout<<myArduino->readString()<<std::endl;

    }




    /*int fd = open("/dev/ttyUSB0",O_RDWR  );
    if(fd ==-1){
      perror("Arduino port open!");
      exit(1);
    }


    char buf[25];
    sprintf(buf,"X=250,Y=350");

    int state =  write(fd,buf,strlen(buf));
    printf("Write size:%d\n",state);
    usleep(100000);

    char buf2[25];
    bzero(buf2,25);
    int size = read(fd,buf2,25);
    printf("Read size:%d - buf:%s\n",size,buf2);
*/
    return 0;


    /*
#ifdef RS232
    myArduino = new arduino(16,115200);
#endif

#ifdef SERIALPORT
    myArduino = new arduino("/dev/ttyUSB0");
#endif
    ProcessImage camera = ProcessImage::getInstance();
    myArduino->connect();
*/
    otomat();
    sleep(10);

    /*camera.openCamera(0);
    camera.takeImage(0, 0);
    camera.writeToFile("/home/hmenn/Desktop/");
*/
    //pthread_create(&thServer, NULL, serverJobs, myArduino);
    // vision operations
    // arduino connections

    /*while (1) {
      if (!flag)
        otomat();
      sleep(2);
    }*/

    //  connectionHelper.writePort();
    //connectionHelper.openArdConnection();
    // pthread_join(thServer,NULL);
  } catch (exception &e) {
    cout << e.what();
  }

  return 0;
}

void otomat() {
  myArduino->step(150, 150);


  /*myArduino->step(-1000, 240);
  myArduino->step(1000, 0);

  sleep(2);

  myArduino->step(-1000, -240);
  myArduino->step(1000, -240);
  myArduino->step(-1000, 0);

  sleep(3);*/

}

void run(ProcessImage &camera) {

  for (int i = 0; i < 1000; i += 20) {
  }


}













