//
// Created by sveyda on 23.03.2017.
//
#include <zconf.h>
#include "../include/ProccesImage.h"

RNG rng(12345);
vector<Frame> realFrameL;
ProcessImage &ProcessImage::getInstance() {
  static ProcessImage INSTANCE;
  return INSTANCE; //Ilk seferde initialize e
}

Frame *fm;

void ProcessImage::openCamera(int cid) {

  camera.open(cid);
  camera.set(CV_CAP_PROP_FPS, 30);
  if (!camera.isOpened())
    throw -1;

   // sleep(1);

}

/*
 * Take single
 */
bool ProcessImage::takeImage(int xCor, int yCor) {
  camera >> frame;
  Frame real = Frame(xCor, yCor, frame);
  realFrameL.push_back(real);
 // fm = Frame(0, 0, frame);
 // imshow("pen", frame);
  //waitKey(5);
  //rotasyon(frame);
  /*while(1) {
    camera >> frame;
    imshow("pen", frame);
    waitKey(5);
  }*/
  //return false;
  return detectStickMan(frame);
}

int ProcessImage::rotasyon(Mat& src) {
  double angle = -1;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  cv::Mat binaryMat(src.size(), src.type());
  GaussianBlur(src, src, Size(3, 3), 2);
  Canny(src, binaryMat, 40, 120, 3);
  dilate(binaryMat, binaryMat, getStructuringElement(MORPH_RECT, Size(5, 5)));
  /*imshow("binary", binaryMat);
  waitKey(1);*/
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
    double bX, bY, sX, sY;
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
    //cout << "bX: " << bX << " bY : " << bY << " sX : " << sX <<  " sY :" << sY << endl;
    // find zone
    if ((bX > sX) && (bY < sY)) {
      angle = 90 - (atan((sY - bY) / (bX - sX)) * 180 / CV_PI);
      //cout << "1" << endl;
    } else if ((bX > sX) && (bY > sY)) {
      angle = atan((bY - sY) / (bX - sX)) * 180 / CV_PI;
      plusAngle = 90;
      angle += plusAngle;
      //cout << "2" << endl;
    } else if ((bX < sX) && (bY > sY)) {
      angle = atan((sX - bX) / (bY - sY)) * 180 / CV_PI;
      plusAngle = 180;
      angle += plusAngle;
      //cout << "3" << endl;
    } else if ((bX < sX) && (bY < sY)) {
      angle = atan((sY - bY) / (sX - bX)) * 180 / CV_PI;
      plusAngle = 270;
      angle += plusAngle;
      // cout << "4" << endl;
    } else if ((bX == sX) && (sY > bY)) {
      //cout << "5" << endl;
      angle = 0;
    } else if ((bX == sX) && (sY < bY)) {
      //cout << "6" << endl;
      angle = 180;
    } else if ((bX > sX) && (sY == bY)) {
      //cout << "7" << endl;
      angle = 90;
    } else if ((bX < sX) && (sY == bY)) {
      //cout << "8" << endl;
      angle = 270;
    } else
      cout << "girmedi" << endl;


    /// Draw contours + rotated rects + ellipses
    for (int i = 0; i < contours.size(); i++) {
      //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      // contour
      drawContours(drawing, contours, i, Scalar(255, 255, 255), 1, 8, vector<Vec4i>(), 0, Point());
      // ellipse
      ellipse(drawing, minEllipse[i], Scalar(0, 255, 255), 2, 8);
      // rotated rectangle
      Point2f rect_points[4];
      minRect[i].points(rect_points);
      for (int j = 0; j < 4; j++)
        line(drawing, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 1, 8);
    }

    if (angle == -1) {
      //cout << "angle : " << abs(minRect[index].angle) << endl;
      angle = abs(minRect[index].angle) + plusAngle;
      //cout << "angle : " << abs(minRect[index].angle) << " plusAngle : " << plusAngle << endl;
    }
    cerr << "**********  angle : " << (int) angle << "  **********" << endl;
      /// Show in a window
      //namedWindow("Contours", CV_WINDOW_AUTOSIZE);
      //imshow("Contours", drawing);
      //waitKey(100);
    return angle;
  } else {
    //cerr << "No angle" << endl;
    for (int i = 0; i < contours.size(); i++) {
      //Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
      // contour
      drawContours(drawing, contours, i, Scalar(0, 255, 0), 1, 8, vector<Vec4i>(), 0, Point());
      // ellipse
      ellipse(drawing, minEllipse[i], Scalar(0, 255, 255), 2, 8);
      // rotated rectangle
      Point2f rect_points[4];
      minRect[i].points(rect_points);
      for (int j = 0; j < 4; j++)
        line(drawing, rect_points[j], rect_points[(j + 1) % 4], Scalar(0, 0, 255), 1, 8);
    }
    /// Show in a window
  //  namedWindow("Contours", CV_WINDOW_AUTOSIZE);
    //  imshow("Contours", drawing);
     // waitKey(1000);
    return -1;
  }
  return -1;
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

bool ProcessImage::detectStickMan(Mat& src) {
  cerr<<"Here"<<endl;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  //filters
  cv::Mat binaryMat(src.size(), src.type());
  GaussianBlur(src, src, Size(3, 3), 2);
  Canny(src, binaryMat, 40, 120, 3);

  dilate(binaryMat, binaryMat, getStructuringElement(MORPH_RECT, Size(5, 5)));
  //imshow("binary", src);
  //waitKey(1);

  /// Find contours
  findContours(binaryMat, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));

  /// Find the rotated rectangles and ellipses for each contour
  vector<RotatedRect> minRect(contours.size());
  vector<RotatedRect> minEllipse(contours.size());
  vector<int> elipseIndex;

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
    }
  }
  int maxElips, minElips;
  if (elipseIndex.size() == 2) {
    int i, j, plusAngle;
    i = elipseIndex[0];
    j = elipseIndex[1];
    if (minEllipse[i].size.area() > minEllipse[j].size.area()) {
      maxElips = i;
      minElips = j;
    } else {
      maxElips = j;
      minElips = i;
    }
    double rate = 0;
    if (minEllipse[maxElips].size.height < minEllipse[maxElips].size.width) {
      rate = minEllipse[minElips].size.width / minEllipse[minElips].size.height;
    } else if (minEllipse[maxElips].size.height > minEllipse[maxElips].size.width) {
      rate = minEllipse[minElips].size.height / minEllipse[minElips].size.width;
    } else
      rate = 1;

    int maxMinEl, maxMaxEl;

    // min elipsin max capi
    if (minEllipse[maxElips].size.width > minEllipse[maxElips].size.height)
      maxMinEl = minEllipse[maxElips].size.width;
    else
      maxMinEl = minEllipse[maxElips].size.height;
    // max elipsin max capi
    if (minEllipse[minElips].size.width > minEllipse[minElips].size.height)
      maxMaxEl = minEllipse[minElips].size.width;
    else
      maxMaxEl = minEllipse[minElips].size.height;

    if (minRect[index].boundingRect().contains(minEllipse[maxElips].center)
        && rate < 2 && minRect[index].boundingRect().area() > 20000
        && (maxMaxEl / maxMinEl) < 2) {
      cout << "buldu " << endl;
      return true;
    }
    return false;
  }
  return false;
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

Mat ProcessImage::concatImage() {


  //sıfıra sıfır dan baslayan ilk bölge birinci bölge
  // 1 2  3 4
  // 5 6 7 8
  Mat a, b, res1, res2, result;
  int dummyX = 50, dummyY = 50;
  //frame listten resimleri c
  a = findSector(dummyX, dummyY);
  b = findSector(dummyX, dummyY);
  hconcat(a, b, res1);
  a = findSector(dummyX, dummyY);
  b = findSector(dummyX, dummyY);
  hconcat(res1, a, res1);
  hconcat(res1, b, res1);
  a = findSector(dummyX, dummyY);
  b = findSector(dummyX, dummyY);
  hconcat(a, b, res2);
  a = findSector(dummyX, dummyY);
  b = findSector(dummyX, dummyY);
  hconcat(res2, a, res2);
  hconcat(res2, b, res2);

  vconcat(res1, res2, result);

  return result;
}

/**
 * 8 parcanın tam ortasında cekilmis imageeler ///cok mu verimsiz aynı anda concatta bul.//
 * @param x her bir parcanın arduinodan gelen orta noktasının xi
 * @param y her bir parcanın arduinodan gelen orta noktasının yi
 * @return  o noktada cekilen image
 */
Mat ProcessImage::findSector(int x, int y) {
  for (int i = 0; i < frameList.size(); ++i) {
    if (frameList.at(i).getX() == x & frameList.at(i).getY() == y)
      return frameList.at(i).getImage();
  }
  //  return NULL;
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
