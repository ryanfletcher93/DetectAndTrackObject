#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H

#include <array>

#include <opencv2/opencv.hpp>

#include "Camera.h"

class StereoCamera {
private:
	std::string cam1IpAddr, cam2IpAddr;
	IpCamera *cam1, *cam2;

public:
	StereoCamera(std::string cam1IpAddr, std::string cam2IpAddr) {
		this->cam1IpAddr = cam1IpAddr;
		this->cam2IpAddr = cam2IpAddr;
	}

	std::array<cv::Mat, 2> getImages();
};


#endif // STEREO_CAMERA_H