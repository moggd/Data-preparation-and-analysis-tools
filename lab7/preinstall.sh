#!/bin/bash
echo "Installing dependencies..."
if [[ "$OSTYPE" == "darwin"* ]]; then
    brew install opencv cmake gcc curl
else
    sudo apt update
    sudo apt install -y libopencv-dev cmake g++ gcc curl
fi

echo "Downloading Face Detection Models..."
curl -O https://raw.githubusercontent.com/opencv/opencv/master/samples/dnn/face_detector/deploy.prototxt
curl -O https://raw.githubusercontent.com/opencv/opencv_3rdparty/dnn_samples_face_detector_20170830/res10_300x300_ssd_iter_140000.caffemodel

echo "Setup completed successfully!"