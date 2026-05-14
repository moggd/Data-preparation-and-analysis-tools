#pragma once
#include <opencv2/opencv.hpp>
#include "KeyProcessor.hpp"

class FrameProcessor
{
public:
  static int brightness; // Для слайдера
  static cv::Point drawingPt1, drawingPt2;
  static bool isDrawing;

  cv::Mat process(const cv::Mat &input, Mode mode);
  static void onMouse(int event, int x, int y, int flags, void *userdata);
};