#include <string>
#include <array>

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"


StereoCamera::StereoCamera(Camera* cam1, Camera* cam2, std::string stereoIntrinsicsPath) {
	this->cam1 = cam1;
	this->cam2 = cam2;

	setStereoIntrinsicsFromFile(stereoIntrinsicsPath);
}

/*
 */
void StereoCamera::getImageFromCameras() {
	cv::Mat tempImage1, tempImage2;

	tempImage1 = cam1->getSingleImage();
	tempImage2 = cam2->getSingleImage();

	cv::cvtColor(tempImage1, this->images[0], CV_RGB2HSV);
	cv::cvtColor(tempImage2, this->images[1], CV_RGB2HSV);
}

/*
 *
 */
void StereoCamera::setStereoIntrinsicsFromFile(std::string filePath) {
	cv::FileStorage fs;
	fs.open(filePath, cv::FileStorage::READ);

	fs["R"] >> this->R;
	fs["E"] >> this->E;
	fs["F"] >> this->F;
	fs["T"] >> this->T;
}


/*
 *
 */
std::array<cv::Mat, 2> StereoCamera::rectifyImages() {
	// TODO: Implement rectificationsh here
	this->rectifiedImages[0] = this->images[0];
	this->rectifiedImages[1] = this->images[1];

	return rectifiedImages;
}