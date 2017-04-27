//
// Created by sveyda on 21.04.2017.
//

#ifndef SVMTRAIN_DETECT_SVM_H
#define SVMTRAIN_DETECT_SVM_H

//opencv headerlari
#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <memory>
#include <unistd.h>
#include <tr1/memory>

#define WINDOW_NAME "CSE396"
#define TRAINED_SVM "/home/sveyda/Desktop/SvmTrain&Detect/output.yml"
#define EXTRACTION_PATH "/home/sveyda/Desktop/SvmTrain&Detect/Extracted_raw/"

#define POSITIVE_TRAINING_SET_PATH "positive/"
#define NEGATIVE_TRAINING_SET_PATH "negative/"

#define POSITIVE_BINARY "/home/sveyda/Desktop/positive_binary/"
#define NEGATIVE_BINARY "/home/sveyda/Desktop/negative_binary/"

#define TRAINED_SVM "/home/sveyda/Desktop/SvmTrain&Detect/output.yml"


#define	IMAGE_SIZE Size(128, 128)

//#define _DEBUG

using namespace cv;
using namespace cv::ml;
using namespace std;


#endif //SVMTRAIN_DETECT_SVM_H
