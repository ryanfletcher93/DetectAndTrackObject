#include "Camera.h"

#include <opencv2/opencv.hpp>

VideoFromFile::VideoFromFile(std::string videoFilePath, std::string intrinsicsFilePath) {
	this->fileName = videoFilePath;

	getCameraIntrinsicsFromFile(intrinsicsFilePath);
}

/*
 *
 */
bool VideoFromFile::openCamera() {
	bool isOpenSuccessful = video.open(fileName);

	return isOpenSuccessful;
}


/*
 *
 */
bool VideoFromFile::closeCamera() {
	video.release();

	return true;
}


/*
 *
 */
cv::Mat VideoFromFile::getImage() {
    cv::Mat tempImg;
    video >> tempImg;

    return tempImg;
}