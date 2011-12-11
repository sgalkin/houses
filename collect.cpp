#include "detector.h"
#include <opencv2/features2d/features2d.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

int main(int argc, char* argv[]) {
  if(argc != 3) {
    std::cerr << "usage: " << argv[0] << " " << "<imagedir>" << " " << "<result>" << std::endl;
    return 1;
  }
  FeatureDetector<cv::SurfFeatureDetector> detector(argv[2]);
  std::for_each(fs::directory_iterator(argv[1]), fs::directory_iterator(), detector);
  return 0;
}
