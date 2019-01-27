#include <string>
#include <array>

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

void StereoCamera::getImageFromCameras() {
	this->images[0] = cam1->getSingleImage();
	this->images[1] = cam2->getSingleImage();
}


/*
 *
 */
std::vector<cv::Mat> StereoCamera::rectifyImages() {
	// TODO: Implement rectificationsh here

	this->rectifiedImages[0] = this->images[0];
	this->rectifiedImages[1] = this->images[1];

	return rectifiedImages;
}