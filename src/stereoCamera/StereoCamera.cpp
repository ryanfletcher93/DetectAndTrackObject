#include <string>
#include <array>

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

std::array<cv::Mat, 2> StereoCamera::getImages() {
	std::array<cv::Mat, 2> imagePair;

	imagePair[0] = cam1->getImage();
	imagePair[1] = cam2->getImage();

	return imagePair;
}