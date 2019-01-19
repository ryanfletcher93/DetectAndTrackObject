#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H

#include <array>

#include <opencv2/opencv.hpp>

#include "Camera.h"

class StereoCamera {
private:
	Camera *cam1, *cam2;

public:
	StereoCamera(Camera* cam1, Camera* cam2) {
		this->cam1 = cam1;
		this->cam2 = cam2;
	}

	std::array<cv::Mat, 2> getImages();

	void rectifyImages(std::array<cv::Mat, 2> inImagePair, std::array<cv::Mat, 2> rectifiedImages);

};


#endif // STEREO_CAMERA_H