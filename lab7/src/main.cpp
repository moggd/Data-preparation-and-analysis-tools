#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"
#include "FaceDetector.hpp"

int main()
{
  try
  {
    CameraProvider camera;
    KeyProcessor keys;
    FrameProcessor processor;
    Display display("Lab 7 - Multithreaded OpenCV");
    FaceDetector faceDetector;

    cv::createTrackbar("Brightness", display.getWindowName(), &FrameProcessor::brightness, 100);
    cv::setMouseCallback(display.getWindowName(), FrameProcessor::onMouse, nullptr);

    while (true)
    {
      cv::Mat frame = camera.getFrame();
      if (frame.empty())
        break;

      Mode currentMode = keys.getCurrentMode();

      if (currentMode == Mode::FACE)
      {
        faceDetector.updateFrame(frame);
      }

      cv::Mat processedFrame = processor.process(frame, currentMode == Mode::FACE ? Mode::NORMAL : currentMode);

      if (currentMode == Mode::FACE)
      {
        std::vector<cv::Rect> faces = faceDetector.getFaces();
        for (const auto &face : faces)
        {
          cv::rectangle(processedFrame, face, cv::Scalar(0, 255, 0), 3);
          cv::putText(processedFrame, "Face", cv::Point(face.x, face.y - 10),
                      cv::FONT_HERSHEY_SIMPLEX, 0.9, cv::Scalar(0, 255, 0), 2);
        }
        cv::putText(processedFrame, "Mode: FACE DETECTION (Async)", cv::Point(10, 90),
                    cv::FONT_HERSHEY_SIMPLEX, 0.6, cv::Scalar(0, 255, 0), 2);
      }

      display.show(processedFrame);

      int key = cv::waitKey(30);
      if (key == 27 || key == 'q')
        break;

      keys.processKey(key);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}