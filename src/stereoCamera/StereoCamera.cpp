#include <string>
#include <array>

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

std::array<cv::Mat, 2> StereoCamera::getImages() {
	std::array<cv::Mat, 2> imagePair;

	imagePair[0] = cam1->getSingleImage();
	imagePair[1] = cam2->getSingleImage();

	return imagePair;
}


/*
 *
 */
void StereoCamera::rectifyImages(std::array<cv::Mat, 2> inImages, std::array<cv::Mat, 2> rectifiedImages) {

	cv::Mat rectifiedImg1, rectifiedImg2;

	// TODO: Implement rectificationsh here

	rectifiedImages[0] = rectifiedImg1;
	rectifiedImages[1] = rectifiedImg2;

	return;
}