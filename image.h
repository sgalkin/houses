#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

inline cv::Mat image(const std::string& file, int flags) {
  cv::Mat image;
  cv::cvtColor(cv::imread(file, 1), image, flags);
  return image;
}

inline cv::Mat bgrImage(const std::string& file) {
  return image(file, CV_RGB2BGR);
}

inline cv::Mat grayImage(const std::string& file) {
  return image(file, CV_RGB2GRAY);
}

#endif // IMAGE_H
