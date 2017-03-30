#include <thread>
#include "include/ProccesImage.h"

using namespace cv;
using namespace Group5;

bool search = true;
bool isClientConnected = false;
bool isArduinoConnected = false;

int main() {

    //std::cerr<<"1"<<std::endl;
    SearchAli& ali = SearchAli::getInstance();

    ali.openCamera();

    std::thread takePhotoThread(&SearchAli::takeImage, &ali);
    //TODO  iki threadin senkronizasyonu mühim..
    //std::thread proccesFrameThread();

    while (search){
        std::cerr<<"lololo"<<std::endl;
        //Decide way
        //Send picture to computer.
        //
    }
    ali.setTakeImageFlag(false);
    takePhotoThread.join();

    return 0;
}















