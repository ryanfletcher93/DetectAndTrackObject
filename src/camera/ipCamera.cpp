#include "Camera.h"

#include <opencv2/opencv.hpp>

IpCamera::IpCamera() {

}

IpCamera::IpCamera(std::string ipAddr) {
	this->ipAddr = ipAddr;
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