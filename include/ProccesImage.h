//
// Created by sveyda on 23.03.2017.
//

#ifndef FINDINGALI_PROCCESIMAGE_H
#define FINDINGALI_PROCCESIMAGE_H
#include "opencv2/opencv.hpp"

using namespace cv;

namespace Group5{

class SearchAli{

private:
    SearchAli() {
        msecond = 50;//0.5 saniye
    }


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

    /*
    * Kameradan her bir private int msecond kadarda fotoğraf çekiyor.
    */
    void takeSingleImage();


/*****************************************************************************/

public:

    static SearchAli& getInstance() {
        static SearchAli instance;
        return instance; //Ilk seferde initialize e
    }

    /*
     * Kamerayi acıyor acmadi ise -1 fırlatiyor
     */
    void openCamera();

    /**
     * Take image from physical device run with threads
     */
    void takeImage();

    /*
     * TODO sendCoordinate
     */
    std::string sendCoordinate();


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

    /**
     * Setter and Getter for msecond
     */
    int getMsecond() const;
    void setMsecond(int msecond);
};

}




#endif //FINDINGALI_PROCCESIMAGE_H
