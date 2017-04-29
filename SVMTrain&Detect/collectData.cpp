#include "svm.h"

using namespace cv;
using namespace cv::ml;
using namespace std;


void processVideo();
void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector);
void draw_locations(Mat & img, const vector< Rect > & locations, const Scalar & color);
bool detect(Mat& img,Mat& draw);
void train();
Ptr<SVM> svm;
HOGDescriptor hog;


int main(int argc, char** argv) {

    Mat img,draw;
    Mat gray_img,canny_img;
    VideoCapture video;
    vector< vector< cv::Point> > contours;
    Mat bounding_mat;
    bool end_of_process = false;

    video.open(1);
    video.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    video.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    if (!video.isOpened()) {
        cerr << "Unable to open the device" << endl;
        exit(-1);
    }

    double max_area =0;
    int count =0;
    while (!end_of_process) {
        try
        {
            video >> img;
            draw = img.clone();

            cvtColor( img, gray_img, COLOR_RGB2GRAY );
            Canny(gray_img, canny_img, 30, 128);

#ifdef _DEBUG
       //     imshow("Gray",gray_img);
           imshow("Canny",canny_img);
#endif
            findContours(canny_img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
            drawContours(img, contours, -1, cv::Scalar(0), 2);

            for(int i = 0; i < contours.size(); ++i)
            {
                Point2f points[4];
                Rect rect;
                RotatedRect rotate_rect;
                double area = 0.0;

                rect = boundingRect(contours[i]);
                rotate_rect = minAreaRect(contours[i]);
                rotate_rect.points(points);
                vector< vector< Point> > polylines;
                polylines.resize(1);
                for(int j = 0; j < 4; ++j)
                    polylines[0].push_back(points[j]);

                area = contourArea(contours[i]);
                if(area > 700){

                    Rect zone ;
                    string file_path = EXTRACTION_PATH +  std::to_string(count) +"n"+ ".png"; //Extraction path klasörü olusturlmalı yoksa yazmaz
                    std::cerr<<file_path<<std::endl;

                    zone = boundingRect(contours[i]); //Data icin en uygun rectange

                    std::ostringstream str;
                    std::ostringstream str1;

                    str<<"Positon: x:"<<zone.tl().x<<" y:"<<zone.tl().y;
                    str1<<"With: "<< zone.width <<"Height: "<<zone.height;

                    //TODO  size correction

                    Mat data = draw(zone); //Resim üzerinden  rectanglı alma


                    resize(data, data, IMAGE_SIZE); //tekrar boyutlandır
                    imwrite(file_path, data); //data imageini dosyaya yaz



                    //Scalar(b,g,r);
                    //Rectangle cizme görebilmek icin
                    cv::polylines(img, polylines, true, Scalar(0, 255, 0), 2);
                    cv::rectangle(img, rect, Scalar(0,0,255), 2);//rectangle ciziyor

                    cv::putText(img,str.str() ,cv::Point(10,40),FONT_HERSHEY_PLAIN,2, 3,2);
                    cv::putText(img,str1.str() ,cv::Point(10,80),FONT_HERSHEY_PLAIN,2, 3,2);

                    count++; //isimlendirmek icin
                }
            }

            imshow("Img",img);
            waitKey(10);
        }
        catch (Exception& e) {
            const char* err_msg = e.what();
            std::cout << "Exception caught:\n" << err_msg << std::endl;
        }

    }

}


Rect sizeCorrection(const Rect& ){

}