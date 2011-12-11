#include <opencv2/ml/ml.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
  if(argc != 4) {
    std::cerr << "usage: " << argv[0] << " <bow.yml> <svm.yml> <image>" << std::endl;
    return 1;
  }
  try {
    cv::FileStorage bowStorage(argv[1], cv::FileStorage::READ);
    cv::Mat bow;
    bowStorage["bow"] >> bow;
    cv::FileStorage svmStorage(argv[2], cv::FileStorage::READ);
    cv::SVM svm;
    svm.read(*svmStorage, *svmStorage["svm"]);
    cv::Mat image = cv::imread(argv[3], 0);

    cv::BOWImgDescriptorExtractor extractor(cv::Ptr<cv::DescriptorExtractor>(new cv::SiftDescriptorExtractor()),
					    cv::Ptr<cv::DescriptorMatcher>(new cv::BruteForceMatcher<cv::L2<float> >()));
    extractor.setVocabulary(bow);
    cv::SIFT sift;
    std::vector<cv::KeyPoint> keys;
    sift(image, cv::Mat(), keys);
    cv::Mat descriptor;
    extractor.compute(image, keys, descriptor);
    std::cout << svm.predict(descriptor) << std::endl;
    return 0;
  } catch (cv::Exception& ex) {
    std::cerr << "Error while processing " << argv[3] << " " << ex.what() << std::endl;
    return 1;
  }
}
