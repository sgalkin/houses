#include "utils.h"

#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <string>
#include <iostream>

namespace fs = boost::filesystem;

class BowTrainer {
public:
  BowTrainer(size_t clusters) : bow_(clusters) {}

  void operator()(const std::string& path, const std::string& sift) {
    std::cerr << "Reading " << sift << std::endl;
    cv::FileStorage sifts(sift, cv::FileStorage::READ);
    const cv::FileNode& root = sifts.root();
    std::for_each(root.begin(), root.end(), boost::bind(&BowTrainer::read, this, boost::cref(path), _1));
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
    std::vector<cv::KeyPoint> keys;
    cv::read(node, keys);
    cv::Mat img = cv::imread((fs::path(path) / id2path(node.name())).string() + ".jpg", 0);
    cv::Mat descriptor;
    sift_(img, cv::Mat(), keys, descriptor, true);
    bow_.add(descriptor);    
  }

  cv::SIFT sift_;
  cv::BOWKMeansTrainer bow_;
};

int main(int argc, char* argv[]) {
  if(argc < 4 || argc % 2 == 1) {
    std::cerr << "usage: " << argv[0] << " " 
	      << "<out>" << " " << "<dir>" << " " << "<sift>" << " ..." << std::endl;
    return 1;
  }
  
  BowTrainer bow(1000);
  for(char** arg = argv + 2; arg != argv + argc; arg += 2) {
    bow(*arg, *(arg + 1));
  }
  bow.train(argv[1]);
  return 0;
}
