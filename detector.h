#ifndef DETECTOR_H
#define DETECTOR_H

#include "utils.h"
#include "image.h"
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <vector>
#include <iostream>

// class KeyPointCollector {
// public:
//   ~KeyPointCollector() {}
//   
//   virtual void operator()(const boost::filesystem::path& path) = 0;
// };

template<typename Detector> struct ImageType;
template<> struct ImageType<cv::SurfFeatureDetector> {
  enum { type = CV_RGB2BGR };
};

template<> struct ImageType<cv::SiftFeatureDetector> {
  enum { type = CV_RGB2GRAY };
};

template<typename Detector>
class FeatureDetector {
public:
  explicit FeatureDetector(const std::string& out, Detector detector = Detector()) :
    file_(out, cv::FileStorage::WRITE),
    detector_(detector) {}

  void operator()(const boost::filesystem::path& path) {
    try {
      std::vector<cv::KeyPoint> keys;
      detector_.detect(image(path.string(), ImageType<Detector>::type), keys);
      cv::write(file_, path2id(path.string()), keys);
    } catch(cv::Exception& ex) {
      std::cerr << "Error while processing " << path << " " << ex.what() << std::endl;
    }
  }

private:
  cv::FileStorage file_;
  Detector detector_;
};

#endif // DETECTOR_H
