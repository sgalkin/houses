#include "utils.h"

#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <iostream>

namespace fs = boost::filesystem;

class SiftProcessor {
public:
    explicit SiftProcessor(const std::string& out) : file_(out, cv::FileStorage::WRITE) {}

    void operator()(const fs::path& path) {
        std::vector<cv::KeyPoint> keys;
        sift_(cv::imread(path.string(), 0), cv::Mat(), keys);
        cv::write(file_, path2id(path.string()), keys);
    }

private:
    cv::FileStorage file_;
    cv::SIFT sift_;
};

int main(int argc, char* argv[]) {
    if(argc != 3) {
        std::cerr << "usage: " << argv[0] << " " << "<imagedir>" << " " << "<result>" << std::endl;
        return 1;
    }
    SiftProcessor sift(argv[2]);
    std::for_each(fs::directory_iterator(argv[1]), fs::directory_iterator(), sift);
    return 0;
}


//     int width = 512, height = 512;
//     cv::Mat image = cv::Mat::zeros(height, width, CV_8UC3);

//     // Set up training data
//     float labels[4] = {1.0, -1.0, -1.0, -1.0};
//     cv::Mat labelsMat(3, 1, CV_32FC1, labels);
//     std::cout << labelsMat << std::endl;

//     float trainingData[4][2] = { {501, 10}, {255, 10}, {501, 255}, {10, 501} };
//     cv::Mat trainingDataMat(3, 2, CV_32FC1, trainingData);
//     std::cout << trainingDataMat << std::endl;
//     trainingDataMat.convertTo(CV_8FC1);

//     // Set up SVM's parameters
//     CvSVMParams params;
//     params.svm_type    = CvSVM::C_SVC;
//     params.kernel_type = CvSVM::LINEAR;
//     params.term_crit   = cvTermCriteria(CV_TERMCRIT_ITER, 100, 1e-6);

//     // Train the SVM
//     CvSVM SVM;
//     SVM.train(trainingDataMat, labelsMat, cv::Mat(), cv::Mat(), params);
    
//     cv::Vec3b green(0,255,0), blue (255,0,0);
//     // Show the decision regions given by the SVM
//     for (int i = 0; i < image.rows; ++i)
//         for (int j = 0; j < image.cols; ++j)
//         {
//             cv::Mat sampleMat = (cv::Mat_<float>(1,2) << i,j);
//             float response = SVM.predict(sampleMat);

//             if (response == 1)
//                 image.at<cv::Vec3b>(j, i)  = green;
//             else if (response == -1) 
//                 image.at<cv::Vec3b>(j, i)  = blue;
//         }

//     // Show the training data
//     int thickness = -1;
//     int lineType = 8;
//     cv::circle( image, cv::Point(501,  10), 5, cv::Scalar(  0,   0,   0), thickness, lineType);
//     cv::circle( image, cv::Point(255,  10), 5, cv::Scalar(255, 255, 255), thickness, lineType);
//     cv::circle( image, cv::Point(501, 255), 5, cv::Scalar(255, 255, 255), thickness, lineType);
//     cv::circle( image, cv::Point( 10, 501), 5, cv::Scalar(255, 255, 255), thickness, lineType);

//     // Show support vectors
//     thickness = 2;
//     lineType  = 8;
//     int c     = SVM.get_support_vector_count();

//     for (int i = 0; i < c; ++i)
//     {
//         const float* v = SVM.get_support_vector(i);
//         circle( image,  cv::Point( (int) v[0], (int) v[1]),   6,  cv::Scalar(128, 128, 128), thickness, lineType);
//     }

//     cv::imwrite("result.png", image);        // save the image 

// //    cv::imshow("SVM Simple Example", image); // show it to the user
// //    cv::waitKey(0);
