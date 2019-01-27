#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

StereoCameraWithBackground::StereoCameraWithBackground(
		Camera* cam1, 
		Camera* cam2, 
		cv::Mat backgroundImg1, 
		cv::Mat backgroundImg2) {
	
	this->cam1 = cam1;
	this->cam2 = cam2;
	this->backgroundImg1 = backgroundImg1;
	this->backgroundImg2 = backgroundImg2;
}