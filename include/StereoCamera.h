#ifndef STEREO_CAMERA_H
#define STEREO_CAMERA_H

#include <array>

#include <opencv2/opencv.hpp>

#include "Camera.h"


/* StereoCamera encapluslates all information needed as part of a two cameras
 * that are aligned in stereo and have had calibration done.
 * 
 * Currenly all images are stored as HSV for easier processing.
 */
class StereoCamera {
private:

protected:
	Camera *cam1, *cam2;
	std::array<cv::Mat, 2> images;
	std::array<cv::Mat, 2> rectifiedImages;

	cv::Mat R, E, F;
	cv::Vec3d T;

	// Functions
	void setStereoIntrinsicsFromFile(std::string);

public:
	StereoCamera() {}

	StereoCamera(Camera* cam1, Camera* cam2, std::string stereoIntrinsicsPath);

	/* Returns the most recent captured images in HSV
	 */
	std::array<cv::Mat, 2> getImages() {
		return images;
	}

	/* Take a snapshot from the cameras, convert to HSV format and 
	 * store in member variables
	 */
	void getImageFromCameras();

	/* Use stereo camera intrinsics to rectify images
	 */
	std::array<cv::Mat, 2> rectifyImages();
};



/*
 *
 */
class StereoCameraWithBackground : public StereoCamera {
private:
	cv::Mat backgroundImg1, backgroundImg2;

public:
	using StereoCamera::StereoCamera;

};


#endif // STEREO_CAMERA_H