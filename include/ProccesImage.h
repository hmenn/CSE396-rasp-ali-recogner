//
// Created by sveyda on 23.03.2017.
//

#ifndef FINDINGALI_PROCCESIMAGE_H
#define FINDINGALI_PROCCESIMAGE_H

#include <iostream>
#include <vector>
#include <opencv2/core.hpp>
#include <string>
#include "Frame.h"
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;
extern vector<Frame> realFrameL;
class ProcessImage {

private:
    ProcessImage() {
      msecond = 50;//0.5 saniye
    }

    vector<Frame> frameList;  // filterli resim


    bool foundFlag = false;
    /**
     * TakeSingle Image Flag
     */
    bool takeImageFlag = true;
    /**
     * Bir sonraki fotoğrafı cekmeden önceki bekleme süresi mili saniye cinsinden
     */
    int msecond;
    /**
     * Fiziksel kamerayi kullanan kamera nesnesi
     */
    VideoCapture camera;

    /**
     * Cameradan alinan orjinal renkli fotograf
     */
    Mat picture;

    /*
     * Cameradan alinan orjinal resmin islenmis hali
     */
    Mat frame;

    //FUNCTIONS

    void SobelFilter(Mat &image1);

    int returnNumberOfEdgePixels(Mat image1);

    bool detectStickMan(Mat& src);

    int maxTotalWeight = 0, maxWeightIndex;
/*****************************************************************************/

public:
    static ProcessImage &getInstance();

    void writeToFile(string path);

    Mat concatImage();

    Mat findSector(int x, int y);

    /*
     * Kamerayi acıyor acmadi ise -1 fırlatiyor
     */
    void openCamera(int cid);

    /**
     * Take image from physical device run with threads
     */
    bool takeImage(int x, int y);

    /**
    * Setter and Getter for found if ali found set true
    */
    bool getFoundFlag() const;

    void setFoundFlag(bool found);

    /**
     * Setter and Getter for Take Input Flag
     */
    bool getTakeImageFlag() const;

    void setTakeImageFlag(bool flag);

    int rotasyon(Mat& src);

    Mat& getLastImage(){
        if(realFrameL.size()>0)
            return realFrameL[realFrameL.size()-1].getImage();

    }

    /**
     * Setter and Getter for msecond
     */
    int getMsecond() const;

    void setMsecond(int msecond);

    int getMaxFrameofX() {
      frameList[maxWeightIndex].getX();
    }

    int getMaxFrameofY() {
      frameList[maxWeightIndex].getY();
    }
};

#endif //FINDINGALI_PROCCESIMAGE_H