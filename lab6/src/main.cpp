#include "CameraProvider.hpp"
#include "KeyProcessor.hpp"
#include "FrameProcessor.hpp"
#include "Display.hpp"

int main()
{
  try
  {
    CameraProvider camera;
    KeyProcessor keys;
    FrameProcessor processor;
    Display display("Lab 6 - OpenCV");

    cv::createTrackbar("Brightness", display.getWindowName(), &FrameProcessor::brightness, 100);
    cv::setMouseCallback(display.getWindowName(), FrameProcessor::onMouse, nullptr);

    while (true)
    {
      cv::Mat frame = camera.getFrame();
      if (frame.empty())
        break;

      cv::Mat processedFrame = processor.process(frame, keys.getCurrentMode());
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