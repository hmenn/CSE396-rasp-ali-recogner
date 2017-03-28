//
// Created by sveyda on 23.03.2017.
//

#include "../include/ProccesImage.h"

using namespace Group5;

void SearchAli::openCamera(){

    camera.open(0);
    camera.set(CV_CAP_PROP_FPS, 1);
    if(!camera.isOpened())
        throw -1;
}
/*
 * Take single
 */
void SearchAli::takeSingleImage(){
    camera >> frame;
    if(!frame.empty()){ //TODO her foto cekiminde if calısacak iyilestir. gerekte olmayabilir.
        imshow("Frame",frame);
    }
    waitKey(msecond);
}

/**
 * Take image from physical device while program ends
 */
void SearchAli::takeImage() {
    while(takeImageFlag)
        this->takeSingleImage();
}



std::string SearchAli::sendCoordinate() {
    //TODO string formati belirlenecek.
    //TODO send coordinat
    return "";
};


/**
 * @return Found flag ali bulundumu bulunmadi mi
 */
bool SearchAli::getFoundFlag() const {
    return foundFlag;
}
/**
 * Ali bulunursa TRUE olarak set et
 * @param flag
 */
void  SearchAli::setFoundFlag(bool found)  {
    foundFlag = false;
}

/*
 * get milisecond wait time for before taking other picture
 */
int SearchAli::getMsecond() const {
    return msecond;
}

/*
 * set wait time for before taking picture
 */
void SearchAli::setMsecond(int msecond) {
    SearchAli::msecond = msecond;
}

/**
 * @return taking image flags
 */
bool SearchAli::getTakeImageFlag() const {
    return takeImageFlag;
}
/**
 * Set FALSE when program ends
 * @param flag
 */
void  SearchAli::setTakeImageFlag(bool flag){
    takeImageFlag=flag;
}
