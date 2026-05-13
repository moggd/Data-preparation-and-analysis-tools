#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class FaceDetector
{
private:
  cv::dnn::Net net;
  std::thread workerThread;
  std::mutex mtx;
  std::atomic<bool> isRunning;
  std::atomic<bool> hasNewFrame;

  cv::Mat currentFrame;
  std::vector<cv::Rect> faces;

  void detectLoop();

public:
  FaceDetector();
  ~FaceDetector();
  void updateFrame(const cv::Mat &frame);
  std::vector<cv::Rect> getFaces();
};