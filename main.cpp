#include <thread>
#include "include/ProccesImage.h"

using namespace cv;
using namespace Group5;

bool search = true;
bool isClientConnected = false;


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






/*
    Mat edges;
    edges = imread("/home/sveyda/Desktop/pic.jpg", IMREAD_GRAYSCALE);
    namedWindow("edges", 1);

    //  Mat frame;
    //  cap >> frame; // get a new frame from camera
    //    cvtColor(frame, edges, CV_BGR2GRAY);
    GaussianBlur(edges, edges, Size(7, 7), 1.5, 1.5);

    Canny(edges, edges, 0, 50, 3);

    imshow("edges", edges);
    imwrite("/home/sveyda/Desktop/pic.jpg", edges);
    // if(waitKey(30) >= 0) break;

*/

    return 0;

}















