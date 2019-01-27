#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H

#include <array>

#include <opencv2/opencv.hpp>

#include "Camera.h"

class StereoCamera {
private:

protected:
	Camera *cam1, *cam2;
	std::vector<cv::Mat> images;

	std::vector<cv::Mat> rectifiedImages;

public:
	StereoCamera() {
		
	}


	StereoCamera(Camera* cam1, Camera* cam2) {
		this->cam1 = cam1;
		this->cam2 = cam2;
	}

	std::vector<cv::Mat> getImages() {
		return images;
	}

	void getImageFromCameras();

	std::vector<cv::Mat> rectifyImages();

};



/*
 *
 */
class StereoCameraWithBackground : public StereoCamera {
private:
	cv::Mat backgroundImg1, backgroundImg2;

public:
	StereoCameraWithBackground(
			Camera* cam1,
			Camera* cam2, 
			cv::Mat backgroundImg1, 
			cv::Mat backgroundImg2);


};


#endif // STEREO_CAMERA_H