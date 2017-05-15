//
// Created by AhmetAlperenBulut on 05.05.2017.
//

#ifndef CSE396_RASP_ALI_RECOGNER_FRAME_H
#define CSE396_RASP_ALI_RECOGNER_FRAME_H

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
using namespace cv;
class Frame {

private:
public:
    Frame(int x, int y,  Mat Image);

    Frame();

    int getX() const;

    void setX(int x);

    int getY() const;

    void setY(int y);

    Mat &getImage();

    void setImage(const Mat &Image);

    int getTotalPixel() const;

    void setTotalPixel(int totalPixel);

private:
    int x;
    int y;
    Mat Image;
    int totalPixel;
};



#endif //CSE396_RASP_ALI_RECOGNER_FRAME_H
