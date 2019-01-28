#include "Camera.h"

#include <opencv2/opencv.hpp>

IpCamera::IpCamera(std::string ipAddr, std::string intrinsicsFilePath) {
	this->ipAddr = ipAddr;

	getCameraIntrinsicsFromFile(intrinsicsFilePath);
}

/*
 *
 */
bool IpCamera::openCamera() {
	bool isOpenSuccessful = video.open("rtsp://" + ipAddr + ":554/onvif1");

	return isOpenSuccessful;
}


/*
 *
 */
bool IpCamera::closeCamera() {
	video.release();
	
	return true;
}


/*
 *
 */
cv::Mat IpCamera::getImage() {
    cv::Mat tempImg;
    video >> tempImg;

    return tempImg;
}