//
// Created by sveyda on 23.03.2017.
//
#include "../include/ProccesImage.h"

RNG rng(12345);

ProcessImage& ProcessImage::getInstance() {
  static ProcessImage INSTANCE;
  return INSTANCE; //Ilk seferde initialize e
}

void ProcessImage::openCamera(int cid) {

  camera.open(cid);
  camera.set(CV_CAP_PROP_FPS, 1);
  if (!camera.isOpened())
    throw -1;
}

/*
 * Take single
 */
void ProcessImage::takeImage(int xCor, int yCor) {
  camera >> frame;

  Frame real = Frame(xCor, yCor, frame);
  realFrameL.push_back(real);
  SobelFilter(frame);
  Frame sobel = Frame(xCor, yCor, frame);

  int total = returnNumberOfEdgePixels(sobel.getImage());
  sobel.setTotalPixel(total);
  frameList.push_back(sobel);

  if (maxTotalWeight < total) {
    maxTotalWeight = total;
    maxWeightIndex = frameList.size() - 1;
  }


  /* if(!frame.empty()){
       imshow("Frame",frame);
   }
   waitKey(msecond);*/
}

int ProcessImage::rotasyon(Mat src) {
  int angle = -1;
  Mat threshold_output;
  Mat edge;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  cv::Mat binaryMat(src.size(), src.type());
  cv::threshold(src, binaryMat, 15, 255, cv::THRESH_BINARY);

  imshow("denem", binaryMat);
  /// Find contours
  findContours(binaryMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect(contours.size());
  vector<RotatedRect> minEllipse(contours.size());
  vector<int> elipseIndex;
  /*cout<<endl<<contours[1]<<endl;
  cout<<endl<<Mat(contours[1])<<endl;*/

  Mat drawing = Mat::zeros(binaryMat.size(), CV_8UC3);
  double area = 0, maxArea = 0;
  int index = 0;
  for (int i = 0; i < contours.size(); i++) {
    minRect[i] = minAreaRect(Mat(contours[i]));
    area = contourArea(contours[i]);
    if (area > maxArea) {
      maxArea = area;
      index = i;
    }
    if (contours[i].size() > 50) {
      elipseIndex.push_back(i);
      minEllipse[i] = fitEllipse(Mat(contours[i]));
      circle(drawing, Point(minEllipse[i].center.x, minEllipse[i].center.y), 2, Scalar(0, 255, 255), 2, 8);
    }
  }
  //cout << "elipseIndex.size : " << elipseIndex.size() << endl;
  if (elipseIndex.size() == 2) {
    int bX, bY, sX, sY;
    int i, j, plusAngle;
    i = elipseIndex[0];
    j = elipseIndex[1];
    if (minEllipse[i].size.area() > minEllipse[j].size.area()) {
      //cout << "buyuk : " << minEllipse[i].center << " kucuk : " << minEllipse[j].center << endl;
      bY = minEllipse[i].center.x;
      bX = minEllipse[i].center.y;
      sY = minEllipse[j].center.x;
      sX = minEllipse[j].center.y;
    } else {
      //cout << "buyuk : " << minEllipse[j].center << " kucuk : " << minEllipse[i].center << endl;
      bY = minEllipse[j].center.x;
      bX = minEllipse[j].center.y;
      sY = minEllipse[i].center.x;
      sX = minEllipse[i].center.y;
    }
    cout << "bX: " << bX << " bY : " << bY << " sX : " << sX << " sY :" << sY << endl;
    // find zone
    if ((bX > sX) && (bY < sY)) {
      plusAngle = 0;
    } else if ((bX > sX) && (bY > sY)) {
      plusAngle = 90;
    } else if ((bX < sX) && (bY > sY)) {
      plusAngle = 180;
    } else if ((bX < sX) && (bY < sY)) {
      plusAngle = 270;
    } else if ((bX == sX) && (sY > bY)) {
      //cout << "1" << endl;
      angle = 0;
    } else if ((bX == sX) && (sY < bY)) {
      //cout << "2" << endl;
      angle = 180;
    } else if ((bX > sX) && (sY == bY)) {
      //cout << "3" << endl;
      angle = 90;
    } else if ((bX < sX) && (sY == bY)) {
      //cout << "4" << endl;
      angle = 270;
    } else
      cout << "girmedi" << endl;


    /// Draw contours + rotated rects + ellipses
    for (int i = 0; i < contours.size(); i++) {
      Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      // contour
      drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
      // ellipse
      ellipse(drawing, minEllipse[i], color, 2, 8);
      // rotated rectangle
      Point2f rect_points[4];
      minRect[i].points(rect_points);
      for (int j = 0; j < 4; j++)
        line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
    }
    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);

    if (angle == -1)
      angle = abs(minRect[index].angle) + plusAngle;
    cout << "**********  angle : " << angle << "  **********" << endl;
    return angle;
  } else {
    cerr << "ERROR" << endl;
    for (int i = 0; i < contours.size(); i++) {
      Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
      // contour
      drawContours(drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point());
      // ellipse
      ellipse(drawing, minEllipse[i], color, 2, 8);
      // rotated rectangle
      Point2f rect_points[4];
      minRect[i].points(rect_points);
      for (int j = 0; j < 4; j++)
        line(drawing, rect_points[j], rect_points[(j + 1) % 4], color, 1, 8);
    }
    /// Show in a window
    namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    imshow("Contours", drawing);
    return -1;
  }
}

void ProcessImage::SobelFilter(Mat &image1) {
  GaussianBlur(image1, image1, Size(5, 5), 0, 0, BORDER_DEFAULT);

  /// Convert it to gray
  cvtColor(image1, image1, CV_BGR2GRAY);


  /// Generate grad_x and grad_y
  Mat grad_x, grad_y;
  Mat abs_grad_x, abs_grad_y;

  /// Gradient X
  //Scharr( src_gray, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT );
  Sobel(image1, grad_x, CV_16S, 1, 0, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(grad_x, abs_grad_x);

  /// Gradient Y
  //Scharr( src_gray, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT );
  Sobel(image1, grad_y, CV_16S, 0, 1, 3, 1, 0, BORDER_DEFAULT);
  convertScaleAbs(grad_y, abs_grad_y);

  /// Total Gradient (approximate)
  addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, image1);
}

void ProcessImage::writeToFile(string path) {
  string str = "";
  for (int a = 0; a < frameList.size(); ++a) // cektigi resimleri klasore yazar
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

int ProcessImage::returnNumberOfEdgePixels(Mat image1) {
  cv::Mat binaryMat(image1.size(), image1.type());

  //Apply thresholding
  cv::threshold(image1, binaryMat, 25, 255, cv::THRESH_BINARY);

  int total = 0;

  for (int i = 0; i < binaryMat.rows; ++i) {
    for (int j = 0; j < binaryMat.cols; ++j) {
      if ((int) binaryMat.at<uchar>(j, i) == 255)
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
void ProcessImage::setFoundFlag(bool found) {
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
void ProcessImage::setTakeImageFlag(bool flag) {
  takeImageFlag = flag;
}
