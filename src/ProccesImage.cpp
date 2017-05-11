//
// Created by sveyda on 23.03.2017.
//

#include "../include/ProccesImage.h"

using namespace Group5;

void ProcessImage::openCamera(){

    camera.open(0);
    camera.set(CV_CAP_PROP_FPS, 1);
    if(!camera.isOpened())
        throw -1;
}
/*
 * Take single
 */
void ProcessImage::takeImage(int xCor, int yCor){
    camera >> frame;

    Frame real = Frame(xCor,yCor,frame);
    realFrameL.push_back(real);
    SobelFilter(frame);
    Frame sobel= Frame(xCor,yCor,frame);

    int total=returnNumberOfEdgePixels(sobel.getImage());
    sobel.setTotalPixel(total);
    frameList.push_back(sobel);

    if(maxTotalWeight<total) {
        maxTotalWeight = total;
        maxWeightIndex = frameList.size()-1;
    }


    /* if(!frame.empty()){
         imshow("Frame",frame);
     }
     waitKey(msecond);*/
}


void rotasyon(Mat src){


    Mat gray_mat, canny_mat;
    Mat contour_mat;

    //1.Read image file & clone.
    cv::Mat binaryMat(src.size(), src.type());

    //Apply thresholding
    cv::threshold(src, binaryMat, 25, 255, cv::THRESH_BINARY);
    contour_mat = src.clone();




    //3. Find & process the contours
    //3.1 find contours on the edge image.
    vector< vector< cv::Point> > contours;
    findContours(binaryMat, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    //3.2 draw contours & property value on the source image.
    double area, length;
    Mat c;
    cvtColor(contour_mat,contour_mat,COLOR_GRAY2BGR);
    drawContours(contour_mat, contours, -1, cv::Scalar(0), 2);  //draw contours on the image

    double maxlen = 0;
    int indx = -1;
    for(int i = 0; i < contours.size(); ++i) {
        length = arcLength(contours[i], true);
        if(length > maxlen){
            maxlen = length;
            indx = i;
        }
    }

    if(indx !=-1){
        Rect rect;
        RotatedRect rotate_rect;
        Point2f points[4];
        Point2f center;
        float radius;

        area = contourArea(contours[indx]);
        rotate_rect = minAreaRect(contours[indx]);
        length = arcLength(contours[indx], true);
        minEnclosingCircle(contours[indx], center, radius);

        rect = boundingRect(contours[indx]);
        minEnclosingCircle(contours[indx], center, radius);
        rotate_rect.points(points);


        vector< vector< Point> > polylines;
        polylines.resize(1);
        for(int j = 0; j < 4; ++j)
            polylines[0].push_back(points[j]);




        /* //draw contour property value at the contour center.
         char buffer[64] = {0};
         sprintf(buffer, "Area: %.2lf", area);
         putText(contour_mat, buffer, center, FONT_HERSHEY_SIMPLEX, .6, Scalar(0), 1);

         sprintf(buffer, "Length: %.2lf", length);
         putText(contour_mat, buffer, Point(center.x,center.y+20), FONT_HERSHEY_SIMPLEX, .6, Scalar(0), 1);

         //draw them on the bounding image.
         cv::rectangle(contour_mat, rect, Scalar(0,0,255), 2);
         image2=binaryMat(Rect(rect.x,rect.y,rect.width,rect.height));
         int x=0,y=0,total=0;
         for(int i=0;i<image2.rows;++i)
         {
             for(int j=0;j<image2.cols;++j)
             {
                 if((int)image2.at<uchar😠j,i)==255)
                 {
                     x=x+j;
                     y=y+i;
                     ++total;
                 }
             }
         }
         x=x/total;
         y=y/total;

         int X=image2.cols/2;
         int Y=image2.rows/2;
         if(x-X!=0) {
             double slope = (y - Y) / (x - X);
             cout « atan (slope) * 180 / 3.14«endl;
         }

         imshow("a",image2);*/
        waitKey(1);
    }





}


void ProcessImage::SobelFilter(Mat &image1)
{
    GaussianBlur( image1, image1, Size(5,5), 0, 0, BORDER_DEFAULT );

    /// Convert it to gray
    cvtColor( image1, image1, CV_BGR2GRAY );


    /// Generate grad_x and grad_y
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    /// Gradient X
    //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
    Sobel( image1, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( grad_x, abs_grad_x );

    /// Gradient Y
    //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
    Sobel( image1, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT );
    convertScaleAbs( grad_y, abs_grad_y );

    /// Total Gradient (approximate)
    addWeighted( abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image1 );
}

void ProcessImage::writeToFile(string path)
{
    string str="";
    for(int a =0 ;a<frameList.size();++a) // cektigi resimleri klasore yazar
    {

        string Result;          // string which will contain the result

        ostringstream convert;   // stream used for the conversion

        convert << a;      // insert the textual representation of 'Number' in the characters in the stream

        Result = convert.str();
        path += Result;
        path += ".jpg";
        cout << "path : " << path << endl;
        imwrite(path, frameList.at(a).getImage());
    }


}
int ProcessImage::returnNumberOfEdgePixels(Mat image1)
{
    cv::Mat binaryMat(image1.size(), image1.type());

    //Apply thresholding
    cv::threshold(image1, binaryMat, 25, 255, cv::THRESH_BINARY);

    int total=0;

    for(int i=0;i<binaryMat.rows;++i)
    {
        for(int j=0;j<binaryMat.cols;++j)
        {
            if((int)binaryMat.at<uchar>(j,i)==255)
                ++total;
        }

    }
    return total;
}

/**
 * @return Found flag ali bulundumu bulunmadi mi
 */
bool ProcessImage::getFoundFlag() const {
    return foundFlag;
}
/**
 * Ali bulunursa TRUE olarak set et
 * @param flag
 */
void  ProcessImage::setFoundFlag(bool found)  {
    foundFlag = false;
}

/*
 * get milisecond wait time for before taking other picture
 */
int ProcessImage::getMsecond() const {
    return msecond;
}

/*
 * set wait time for before taking picture
 */
void ProcessImage::setMsecond(int msecond) {
    ProcessImage::msecond = msecond;
}

/**
 * @return taking image flags
 */
bool ProcessImage::getTakeImageFlag() const {
    return takeImageFlag;
}
/**
 * Set FALSE when program ends
 * @param flag
 */
void  ProcessImage::setTakeImageFlag(bool flag){
    takeImageFlag=flag;
}
