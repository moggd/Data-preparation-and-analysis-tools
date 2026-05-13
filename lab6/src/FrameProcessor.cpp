#include "FrameProcessor.hpp"

int FrameProcessor::brightness = 50;
cv::Point FrameProcessor::drawingPt1 = cv::Point(-1, -1);
cv::Point FrameProcessor::drawingPt2 = cv::Point(-1, -1);
bool FrameProcessor::isDrawing = false;

void FrameProcessor::onMouse(int event, int x, int y, int flags, void *userdata)
{
  if (event == cv::EVENT_LBUTTONDOWN)
  {
    isDrawing = true;
    drawingPt1 = cv::Point(x, y);
    drawingPt2 = drawingPt1;
  }
  else if (event == cv::EVENT_MOUSEMOVE && isDrawing)
  {
    drawingPt2 = cv::Point(x, y);
  }
  else if (event == cv::EVENT_LBUTTONUP)
  {
    isDrawing = false;
    drawingPt2 = cv::Point(x, y);
  }
}

cv::Mat FrameProcessor::process(const cv::Mat &input, Mode mode)
{
  cv::Mat output;

  switch (mode)
  {
  case Mode::NORMAL:
    input.copyTo(output);
    break;
  case Mode::INVERT:
    cv::bitwise_not(input, output);
    break;
  case Mode::BLUR:
    cv::GaussianBlur(input, output, cv::Size(15, 15), 0);
    break;
  case Mode::CANNY:
    cv::cvtColor(input, output, cv::COLOR_BGR2GRAY);
    cv::Canny(output, output, 50, 150);
    cv::cvtColor(output, output, cv::COLOR_GRAY2BGR);
    break;
  case Mode::SOBEL:
    cv::Mat grad_x, grad_y, abs_grad_x, abs_grad_y;
    cv::Sobel(input, grad_x, CV_16S, 1, 0, 3);
    cv::Sobel(input, grad_y, CV_16S, 0, 1, 3);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, output);
    break;
  }

  output.convertTo(output, -1, 1, brightness - 50);

  if (drawingPt1.x != -1 && drawingPt2.x != -1)
  {
    cv::rectangle(output, drawingPt1, drawingPt2, cv::Scalar(0, 255, 0), 2);
  }

  static int frames = 0;
  static int64 startTime = cv::getTickCount();
  frames++;
  int64 currentTime = cv::getTickCount();
  double fps = frames / ((currentTime - startTime) / cv::getTickFrequency());
  cv::putText(output, "FPS: " + std::to_string((int)fps), cv::Point(10, 30),
              cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);
  cv::putText(output, "Modes: 1-Norm, 2-Inv, 3-Blur, 4-Canny, 5-Sobel", cv::Point(10, 60),
              cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(255, 0, 0), 2);

  return output;
}