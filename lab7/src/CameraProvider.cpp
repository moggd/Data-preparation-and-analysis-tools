#include "CameraProvider.hpp"
#include <stdexcept>

CameraProvider::CameraProvider()
{
  cap.open(0);
  if (!cap.isOpened())
  {
    throw std::runtime_error("Error: Cannot open camera.");
  }
}

CameraProvider::~CameraProvider()
{
  cap.release();
}

cv::Mat CameraProvider::getFrame()
{
  cv::Mat frame;
  cap >> frame;
  return frame;
}