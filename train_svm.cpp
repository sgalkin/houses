#include "utils.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/ml/ml.hpp>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/optional.hpp>
#include <iostream>
#include <string>

namespace fs = boost::filesystem;

class SVMTrainer {
public:
  explicit SVMTrainer(const std::string& bow) : 
    isPositive_(true),
    descriptor_(cv::Ptr<cv::DescriptorExtractor>(new cv::SiftDescriptorExtractor()),
		cv::Ptr<cv::DescriptorMatcher>(new cv::BruteForceMatcher<cv::L2<float> >())),
    response_(0, 1, CV_32FC1)
  {
    cv::FileStorage bowStorage(bow, cv::FileStorage::READ);
    cv::Mat bowData;
    bowStorage["bow"] >> bowData;
    descriptor_.setVocabulary(bowData);
    train_ = cv::Mat(0, descriptor_.descriptorSize(), descriptor_.descriptorType());
  }

  void operator()(const std::string& path, const std::string& sift) {
    std::cerr << "Reading " << sift << std::endl;
    cv::FileStorage sifts(sift, cv::FileStorage::READ);
    const cv::FileNode& root = sifts.root();
    std::for_each(root.begin(), root.end(), boost::bind(&SVMTrainer::read, this, boost::cref(path), _1));
    isPositive_ = false;
  }

  void train(const std::string& filename) {
    std::cerr << "Training SVM" << std::endl;
    svm_.train(*train_, response_);
    std::cerr << "Saving" << std::endl;
    cv::FileStorage svm(filename, cv::FileStorage::WRITE);
    svm_.write(*svm, "svm");
  }

private:
  void read(const std::string& path, const cv::FileNode& node) {
    std::vector<cv::KeyPoint> keys;
    cv::read(node, keys);
    cv::Mat img = cv::imread((fs::path(path) / id2path(node.name())).string() + ".jpg", 0);
    cv::Mat descriptor;
    descriptor_.compute(img, keys, descriptor);
    train_->push_back(descriptor);
    response_.push_back(isPositive_ ? 1 : -1);
  }

  bool isPositive_;
  cv::BOWImgDescriptorExtractor descriptor_;
  boost::optional<cv::Mat> train_;
  cv::Mat response_;
  cv::SVM svm_;
};

int main(int argc, char* argv[]) {
  if(argc < 6 && argc % 2 == 0) {
    std::cerr << "usage: " << argv[0] << " <bow.yml> <svm.yml> "
	      << "<positive> <sift>"
	      << "<negative> <sift> ..." << std::endl;
    return 1;
  }
  SVMTrainer svm(argv[1]);
  for(char** arg = argv + 3; arg != argv + argc; arg += 2) {
    svm(*arg, *(arg + 1));
  }
  svm.train(argv[2]);
  return 0;
}
