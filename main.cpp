#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

static void help() {
    cout << "\nThis program demonstrates circle finding with the Hough transform.\n"
            "Usage:\n"
            "./houghcircles <image_name>, Default is ../data/board.jpg\n" << endl;
}

int main(int argc, char **argv) {
    cv::CommandLineParser parser(argc, argv,
                                 "{help h ||}{@image|../data/board.jpg|}"
    );
    if (parser.has("help")) {
        help();
        return 0;
    }
    string filename = parser.get<string>("@image");
    if (filename.empty()) {
        help();
        cout << "no image_name provided" << endl;
        return -1;
    }
    Mat img = imread(filename, 0);
    if (img.empty()) {
        help();
        cout << "can not open " << filename << endl;
        return -1;
    }

    imshow("detected circles", img);
    waitKey();
    return 0;
}