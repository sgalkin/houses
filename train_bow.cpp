#include "utils.h"
#include "image.h"
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

namespace fs = boost::filesystem;

template<typename Extractor> struct ImageType;
template<> struct ImageType<cv::SIFT> { enum { type = CV_RGB2GRAY }; };
template<> struct ImageType<cv::SURF> { enum { type = CV_RGB2GRAY }; };
template<> struct ImageType<cv::OpponentColorDescriptorExtractor> { enum { type = CV_RGB2BGR }; };

template<typename Extractor>
class BowTrainer {
public:
  explicit BowTrainer(size_t clusters, const Extractor& extractor = Extractor()) :
    bow_(clusters),
    extractor_(extractor) {}

  void operator()(const std::string& path, const std::string& features) {
    try {
      std::cerr << "Reading " << features << std::endl;
      cv::FileStorage keys(features, cv::FileStorage::READ);
      const cv::FileNode& root = keys.root();
      std::for_each(root.begin(), root.end(), boost::bind(&BowTrainer::read, this, boost::cref(path), _1));
    } catch (cv::Exception& ex) {
      std::cerr << "Error while processing " << path << " " << ex.what() << std::endl;
    }
  }

  void train(const std::string& path) {
    cv::FileStorage storage(path, cv::FileStorage::WRITE);
    std::cerr << "Clustering..." << std::endl;
    cv::Mat cluster = bow_.cluster();
    std::cerr << "Saving..." << std::endl;
    storage << "bow" << cluster;
  }

private:
  void read(const std::string& path, const cv::FileNode& node) {
    try {
      std::vector<cv::KeyPoint> keys;
      cv::read(node, keys);
      cv::Mat descriptor;
      extractor_.compute(image((fs::path(path) / id2path(node.name())).string() + ".jpg", ImageType<Extractor>::type), keys, descriptor);
      bow_.add(descriptor);
    } catch(cv::Exception& ex) {
      std::cerr << "Error while processing " << path << " " << ex.what() << std::endl;
    }
  }

  cv::BOWKMeansTrainer bow_;
  Extractor extractor_;
};

int main(int argc, char* argv[]) {
  if(argc < 4 || argc % 2 == 1) {
    std::cerr << "usage: " << argv[0] << " " 
	      << "<out>" << " " << "<dir>" << " " << "<sift>" << " ..." << std::endl;
    return 1;
  }
  
  BowTrainer<cv::OpponentColorDescriptorExtractor> bow(1000, cv::OpponentColorDescriptorExtractor(new cv::SurfDescriptorExtractor));
  for(char** arg = argv + 2; arg != argv + argc; arg += 2) {
    bow(*arg, *(arg + 1));
  }
  bow.train(argv[1]);
  return 0;
}
