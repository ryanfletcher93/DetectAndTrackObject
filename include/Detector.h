#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"


class DetectorResults {
private:


public:
		
};


class Detector {
protected:
	StereoCamera* stereoCamera;

	/*
	 *
	 */
	virtual std::vector<cv::KeyPoint> identifyObjects(cv::Mat image);

public:
	virtual DetectorResults* detect() = 0;
};


/*
 *
 */
class ThresholdDetector : public Detector {
private:
	/*
	 *
	 */
	cv::Mat thresholdImage(cv::Mat image);

public:
	ThresholdDetector();

	ThresholdDetector(StereoCamera* stereoCamera) {
		this->stereoCamera = stereoCamera;
	}

	/*
	 *
	 */
	DetectorResults* detect();

};


/*
 *
 */
class BackgroundSubtractionDetector : public Detector {
private:
	cv::Mat thresholdImage(cv::Mat image);

public:
	/*
	 *
	 */
	BackgroundSubtractionDetector() {

	}


	BackgroundSubtractionDetector(StereoCamera* stereoCamera) {
		this->stereoCamera = stereoCamera;
	}

	cv::Mat subtractBackground(cv::Mat image);

	DetectorResults* detect();
};


#endif //DETECTOR_H