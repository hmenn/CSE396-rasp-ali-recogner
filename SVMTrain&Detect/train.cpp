#include "svm.h"



void train();
void load_images(string directory, vector<Mat>& image_list);
vector<string> files_in_directory(string directory);
void compute_hog(const vector< Mat > & img_lst, vector< Mat > & gradient_lst, const Size & size);
Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size);
void train_svm(const vector< Mat > & gradient_lst, const vector< int > & labels);
void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData);
void convert_to_binary(string directory,string todirectory);


int main(int argc, char** argv)
{

    convert_to_binary(POSITIVE_TRAINING_SET_PATH,POSITIVE_BINARY);
    convert_to_binary(NEGATIVE_TRAINING_SET_PATH,NEGATIVE_BINARY);
//    train();
    return 0;
}

void convert_to_binary(string directory,string todirectory) {

    Mat img,dst;
    vector<string> files;
    files = files_in_directory(directory);

    int threshold_value = 140; //
    int threshold_type = 0;
    int const max_BINARY_value = 255;



    for (int i = 0; i < files.size(); ++i) {
#ifdef _DEBUG
        std::cerr<<files.at(i)<<std::endl;
        std::cerr<<todirectory+to_string(i)+".jpg"<<std::endl;
#endif

        img = imread(files.at(i));
        if (img.empty())
            continue;
        threshold( img, dst, threshold_value, max_BINARY_value,threshold_type );
        imwrite(todirectory+to_string(i)+".jpg",dst);

#ifdef _DEBUG
        imshow("image", img);
        imshow( "result", dst );
        waitKey(100);
#endif
    }
}


void train(){

    vector<Mat> pos_lst,full_neg_lst,neg_lst,gradient_lst;
    vector<int> labels;

    load_images(POSITIVE_TRAINING_SET_PATH, pos_lst);
    labels.assign(pos_lst.size(), +1);

    load_images(NEGATIVE_TRAINING_SET_PATH, full_neg_lst);
    labels.insert(labels.end(), full_neg_lst.size(), -1);

    compute_hog(pos_lst, gradient_lst, IMAGE_SIZE);
    compute_hog(full_neg_lst,gradient_lst, IMAGE_SIZE);

    train_svm(gradient_lst, labels);
}

void load_images(string directory, vector<Mat>& image_list) {

    Mat img;
    vector<string> files;
    files = files_in_directory(directory);


    for (int i = 0; i < files.size(); ++i) {
       //std::cerr<<files.at(i)<<std::endl;
        img = imread(files.at(i));
        if (img.empty())
            continue;

#ifdef _DEBUG
        imshow("image", img);
		waitKey(100);
#endif
        resize(img, img, IMAGE_SIZE);
        image_list.push_back(img.clone());
    }
}

vector<string> files_in_directory(string directory)
{
    vector<string> files;
    char buf[256];
    string command;

    //std::cerr<<directory<<std::endl;
    command = "ls " + directory;
    std::tr1::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);

    char cwd[256];
    getcwd(cwd, sizeof(cwd));

    while (!feof(pipe.get()))
        if (fgets(buf, 256, pipe.get()) != NULL) {
            string file(cwd);
           // std::cerr<<buf<<std::endl;
            file.append("/");
            file.append(directory);
            file.append(buf);

            file.pop_back();
            files.push_back(file);
            std::cerr<<file<<std::endl;
        }

    return files;
}

void compute_hog(const vector< Mat > & img_lst, vector< Mat > & gradient_lst, const Size & size)
{
    HOGDescriptor hog;
    hog.winSize = size;
    Mat gray;
    vector< Point > location;
    vector< float > descriptors;

    vector< Mat >::const_iterator img = img_lst.begin();
    vector< Mat >::const_iterator end = img_lst.end();
    for (; img != end; ++img)
    {
        cvtColor(*img, gray, COLOR_BGR2GRAY);
        hog.compute(gray, descriptors, Size(8, 8), Size(0, 0), location);
        gradient_lst.push_back(Mat(descriptors).clone());
#ifdef _DEBUG
        imshow("gradient", get_hogdescriptor_visu(img->clone(), descriptors, size));
		waitKey(20);
#endif
    }
}

Mat get_hogdescriptor_visu(const Mat& color_origImg, vector<float>& descriptorValues, const Size & size)
{
    const int DIMX = size.width;
    const int DIMY = size.height;
    float zoomFac = 3;
    Mat visu;
    resize(color_origImg, visu, Size((int)(color_origImg.cols*zoomFac), (int)(color_origImg.rows*zoomFac)));

    int cellSize = 8;
    int gradientBinSize = 9;
    float radRangeForOneBin = (float)(CV_PI / (float)gradientBinSize); // dividing 180� into 9 bins, how large (in rad) is one bin?

    // prepare data structure: 9 orientation / gradient strenghts for each cell
    int cells_in_x_dir = DIMX / cellSize;
    int cells_in_y_dir = DIMY / cellSize;
    float*** gradientStrengths = new float**[cells_in_y_dir];
    int** cellUpdateCounter = new int*[cells_in_y_dir];
    for (int y = 0; y<cells_in_y_dir; y++)
    {
        gradientStrengths[y] = new float*[cells_in_x_dir];
        cellUpdateCounter[y] = new int[cells_in_x_dir];
        for (int x = 0; x<cells_in_x_dir; x++)
        {
            gradientStrengths[y][x] = new float[gradientBinSize];
            cellUpdateCounter[y][x] = 0;

            for (int bin = 0; bin<gradientBinSize; bin++)
                gradientStrengths[y][x][bin] = 0.0;
        }
    }

    // nr of blocks = nr of cells - 1
    // since there is a new block on each cell (overlapping blocks!) but the last one
    int blocks_in_x_dir = cells_in_x_dir - 1;
    int blocks_in_y_dir = cells_in_y_dir - 1;

    // compute gradient strengths per cell
    int descriptorDataIdx = 0;
    int cellx = 0;
    int celly = 0;

    for (int blockx = 0; blockx<blocks_in_x_dir; blockx++)
    {
        for (int blocky = 0; blocky<blocks_in_y_dir; blocky++)
        {
            // 4 cells per block ...
            for (int cellNr = 0; cellNr<4; cellNr++)
            {
                // compute corresponding cell nr
                cellx = blockx;
                celly = blocky;
                if (cellNr == 1) celly++;
                if (cellNr == 2) cellx++;
                if (cellNr == 3)
                {
                    cellx++;
                    celly++;
                }

                for (int bin = 0; bin<gradientBinSize; bin++)
                {
                    float gradientStrength = descriptorValues[descriptorDataIdx];
                    descriptorDataIdx++;

                    gradientStrengths[celly][cellx][bin] += gradientStrength;

                } // for (all bins)


                // note: overlapping blocks lead to multiple updates of this sum!
                // we therefore keep track how often a cell was updated,
                // to compute average gradient strengths
                cellUpdateCounter[celly][cellx]++;

            } // for (all cells)


        } // for (all block x pos)
    } // for (all block y pos)


    // compute average gradient strengths
    for (celly = 0; celly<cells_in_y_dir; celly++)
    {
        for (cellx = 0; cellx<cells_in_x_dir; cellx++)
        {

            float NrUpdatesForThisCell = (float)cellUpdateCounter[celly][cellx];

            // compute average gradient strenghts for each gradient bin direction
            for (int bin = 0; bin<gradientBinSize; bin++)
            {
                gradientStrengths[celly][cellx][bin] /= NrUpdatesForThisCell;
            }
        }
    }

    for (celly = 0; celly<cells_in_y_dir; celly++)
        // draw cells
    {
        for (cellx = 0; cellx<cells_in_x_dir; cellx++)
        {
            int drawX = cellx * cellSize;
            int drawY = celly * cellSize;

            int mx = drawX + cellSize / 2;
            int my = drawY + cellSize / 2;

            rectangle(visu, Point((int)(drawX*zoomFac), (int)(drawY*zoomFac)), Point((int)((drawX + cellSize)*zoomFac), (int)((drawY + cellSize)*zoomFac)), Scalar(100, 100, 100), 1);

            // draw in each cell all 9 gradient strengths
            for (int bin = 0; bin<gradientBinSize; bin++)
            {
                float currentGradStrength = gradientStrengths[celly][cellx][bin];

                // no line to draw?
                if (currentGradStrength == 0)
                    continue;

                float currRad = bin * radRangeForOneBin + radRangeForOneBin / 2;

                float dirVecX = cos(currRad);
                float dirVecY = sin(currRad);
                float maxVecLen = (float)(cellSize / 2.f);
                float scale = 2.5; // just a visualization scale, to see the lines better

                // compute line coordinates
                float x1 = mx - dirVecX * currentGradStrength * maxVecLen * scale;
                float y1 = my - dirVecY * currentGradStrength * maxVecLen * scale;
                float x2 = mx + dirVecX * currentGradStrength * maxVecLen * scale;
                float y2 = my + dirVecY * currentGradStrength * maxVecLen * scale;

                // draw gradient visualization
                line(visu, Point((int)(x1*zoomFac), (int)(y1*zoomFac)), Point((int)(x2*zoomFac), (int)(y2*zoomFac)), Scalar(0, 255, 0), 1);

            } // for (all bins)

        } // for (cellx)
    } // for (celly)


    // don't forget to free memory allocated by helper data structures!
    for (int y = 0; y<cells_in_y_dir; y++)
    {
        for (int x = 0; x<cells_in_x_dir; x++)
        {
            delete[] gradientStrengths[y][x];
        }
        delete[] gradientStrengths[y];
        delete[] cellUpdateCounter[y];
    }
    delete[] gradientStrengths;
    delete[] cellUpdateCounter;

    return visu;

}


void train_svm(const vector< Mat > & gradient_lst, const vector< int > & labels) {

    // Default values to train SVM
    cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
    svm->setCoef0(0.0);
    svm->setDegree(3);
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-3));
    svm->setGamma(0);
    svm->setKernel(SVM::LINEAR);
    svm->setNu(0.5);
    svm->setP(0.1); // for EPSILON_SVR, epsilon in loss function?
    svm->setC(0.01); // From paper, soft classifier
    svm->setType(SVM::EPS_SVR); // C_SVC; // EPSILON_SVR; // may be also NU_SVR; // do regression task

    Mat train_data;
    convert_to_ml(gradient_lst, train_data);

    clog << "Start training...";
    svm->train(train_data, ROW_SAMPLE, Mat(labels));
    clog << "...[done]" << endl;

    svm->save(TRAINED_SVM);

}

void convert_to_ml(const std::vector< cv::Mat > & train_samples, cv::Mat& trainData) {

    int rows = (int)train_samples.size();
    int cols = (int)std::max(train_samples[0].cols, train_samples[0].rows);

    cv::Mat tmp(1, cols, CV_32FC1);
    trainData = cv::Mat(rows, cols, CV_32FC1);
    vector< Mat >::const_iterator itr = train_samples.begin();
    vector< Mat >::const_iterator end = train_samples.end();

    for (int i = 0; itr != end; ++itr, ++i) {
        CV_Assert(itr->cols == 1 ||  itr->rows == 1);
        if (itr->cols == 1) {
            transpose(*(itr), tmp);
            tmp.copyTo(trainData.row(i));
        }
        else if (itr->rows == 1) {
            itr->copyTo(trainData.row(i));
        }
    }
}
