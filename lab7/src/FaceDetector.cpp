#include "FaceDetector.hpp"
#include <chrono>

FaceDetector::FaceDetector() : isRunning(true), hasNewFrame(false)
{
  net = cv::dnn::readNetFromCaffe("deploy.prototxt", "res10_300x300_ssd_iter_140000.caffemodel");
  workerThread = std::thread(&FaceDetector::detectLoop, this);
}

FaceDetector::~FaceDetector()
{
  isRunning = false;
  if (workerThread.joinable())
  {
    workerThread.join();
  }
}

void FaceDetector::updateFrame(const cv::Mat &frame)
{
  std::lock_guard<std::mutex> lock(mtx);
  frame.copyTo(currentFrame);
  hasNewFrame = true;
}

std::vector<cv::Rect> FaceDetector::getFaces()
{
  std::lock_guard<std::mutex> lock(mtx);
  return faces;
}

void FaceDetector::detectLoop()
{
  while (isRunning)
  {
    cv::Mat frameToProcess;
    {
      std::lock_guard<std::mutex> lock(mtx);
      if (!hasNewFrame || currentFrame.empty())
        continue;
      currentFrame.copyTo(frameToProcess);
      hasNewFrame = false;
    }

    cv::Mat blob = cv::dnn::blobFromImage(frameToProcess, 1.0, cv::Size(300, 300), cv::Scalar(104.0, 177.0, 123.0));
    net.setInput(blob);
    cv::Mat detection = net.forward();
    cv::Mat detectionMat(detection.size[2], detection.size[3], CV_32F, detection.ptr<float>());

    std::vector<cv::Rect> detectedFaces;
    for (int i = 0; i < detectionMat.rows; i++)
    {
      float confidence = detectionMat.at<float>(i, 2);
      if (confidence > 0.5)
      {
        int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frameToProcess.cols);
        int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frameToProcess.rows);
        int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frameToProcess.cols);
        int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frameToProcess.rows);
        detectedFaces.push_back(cv::Rect(cv::Point(x1, y1), cv::Point(x2, y2)));
      }
    }

    {
      std::lock_guard<std::mutex> lock(mtx);
      faces = detectedFaces;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}