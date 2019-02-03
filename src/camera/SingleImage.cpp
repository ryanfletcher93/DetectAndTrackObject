#include "Camera.h"

#include <opencv2/opencv.hpp>

using std::string;

using cv::Mat;

SingleImage::SingleImage(string imageFilePath, string intrinsicsFilePath) {
	this->filePath = imageFilePath;

	getCameraIntrinsicsFromFile(intrinsicsFilePath);
}


bool SingleImage::openCamera() {
	return true;
}


bool SingleImage::closeCamera() {
	return true;
}


Mat SingleImage::getImage() {
	Mat img;
	img = cv::imread(filePath);
	return img;
}