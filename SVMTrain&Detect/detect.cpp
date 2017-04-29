#include "svm.h"
#define _DEBUG

bool end_of_process = false;
void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector);
void draw_locations(Mat & img, const vector< Rect > & locations, const Scalar & color);
bool detect(Mat& img,Mat& draw);
void train();

Ptr<SVM> svm;
HOGDescriptor hog;

int main(int argc, char** argv) {

    VideoCapture camera;
    Mat img,img2;
    Mat bin_img,gray_img,canny_img;


    Mat bounding_mat;


    camera.open(1);
    camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
    camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);

    if (!camera.isOpened()) {
        cerr << "Unable to open the device" << endl;
        exit(-1);
    }

    hog.winSize = IMAGE_SIZE;
    svm = StatModel::load<SVM>(TRAINED_SVM); //Train edilmis svm modelini yukleme
    vector< float > hog_detector;
    get_svm_detector(svm, hog_detector); //Modelden hog detector cikarma
    hog.setSVMDetector(hog_detector); //Cikarilen hogdescriptoru  set etme

    double max_area = 0;

    while (!end_of_process) {
        try {
            vector< vector< cv::Point> > contours;
            camera >> img;      //cameradan görüntü alma
            img2 = img.clone(); //görüntünün kopyasi

            cvtColor( img, gray_img, COLOR_RGB2GRAY ); //gri hali
            threshold( gray_img, bin_img, 110, 255, 0 );  //binary hali
            Canny(gray_img, canny_img, 30, 128); //canny görüntüsü
            findContours(canny_img, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); // canny den  counter cikarma
            //drawContours(img, contours, -1, cv::Scalar(0), 2);

#ifdef _DEBUG
            //imshow("Gray",gray_img);
            imshow("Binary",bin_img);
            imshow("Canny",canny_img);
#endif
           /* int max_loc=-1;
            for (int k = 0; k <contours.size() ; ++k) {


                Rect rect;
                rect = boundingRect(contours[k]);
                double area = contourArea(contours[k]);
                cv::rectangle(img, rect, Scalar(0,0,255), 2);
                if(max_area < area){
                    max_area = area;
                    std::cerr<<"area"<<" "<<max_area<<std::endl;
                    max_loc = k;
                }
            }
            std::cerr<<"Max_loc"<<max_loc<<endl;
            if(max_loc > 0){
                Rect rect;
                rect = boundingRect(contours[max_loc]);
                cv::rectangle(img, rect, Scalar(0,255,255), 2);
            }
            */

            for(int i = 0; i < contours.size(); ++i) {

                Point2f points[4];
                Rect rect;
                RotatedRect rotate_rect;
                rect = boundingRect(contours[i]);

                rotate_rect = minAreaRect(contours[i]);
                rotate_rect.points(points);

                vector< vector< Point> > polylines;
                polylines.resize(1);
                for(int j = 0; j < 4; ++j)
                    polylines[0].push_back(points[j]);
                double area = contourArea(contours[i]);
                if(area > 800) {
                    Rect x;
                    x = boundingRect(contours[i]);
                    Mat fsvm = img2(x);
                    cerr<<fsvm.size();
                    //Size correction

                    resize(fsvm, fsvm, IMAGE_SIZE);
                    std::cerr<<"area"<<" "<<area<<std::endl;
                    //draw them on the bounding image.
                    cv::polylines(img, polylines, true, Scalar(0, 255, 0), 2);
                    cv::rectangle(img, rect, Scalar(0, 0, 255), 2);
                    detect(img,fsvm);
                }


            }

            imshow("Img",img);
            waitKey(10);
        } catch (Exception& e) {

            const char* err_msg = e.what();
            std::cout << "exception caught: \n" << err_msg << std::endl;
        }
    }



    return 0;
}

bool detect(Mat& img,Mat& draw)
{
    vector< Rect > locations;
    draw = img.clone();
    locations.clear();

    hog.detectMultiScale(img, locations);
    if(locations.size()>= 1)
        return true;
    draw_locations(draw, locations, Scalar(255, 0, 0));
    std::cerr<<"Found Num:"<<locations.size();
    return false;
}

void get_svm_detector(const Ptr<SVM>& svm, vector< float > & hog_detector) {

    Mat sv = svm->getSupportVectors();
    const int sv_total = sv.rows;
    Mat alpha, svidx;
    double rho = svm->getDecisionFunction(0, alpha, svidx);
    CV_Assert(alpha.total() == 1 && svidx.total() == 1 && sv_total == 1);
    CV_Assert((alpha.type() == CV_64F && alpha.at<double>(0) == 1.) ||
              (alpha.type() == CV_32F && alpha.at<float>(0) == 1.f));
    CV_Assert(sv.type() == CV_32F);
    hog_detector.clear();
    hog_detector.resize(sv.cols + 1);
    memcpy(&hog_detector[0], sv.ptr(), sv.cols*sizeof(hog_detector[0]));
    hog_detector[sv.cols] = (float)-rho;
}

void draw_locations(Mat & img, const vector< Rect > & locations, const Scalar & color) {

    if (!locations.empty()) {

        vector< Rect >::const_iterator loc = locations.begin();
        vector< Rect >::const_iterator end = locations.end();
        for (; loc != end; ++loc) {
            rectangle(img, *loc, color, 2);
        }
        //  imshow("Img", img);
    }
}
