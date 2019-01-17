#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

class Detector {
protected:
	StereoCamera* stereoCamera;

public:
	Detector();

	Detector(StereoCamera* stereoCamera) {
		this->stereoCamera = stereoCamera;
	}

	virtual void detect() = 0;
};


class ThresholdDetector : Detector {
private:


public:
	ThresholdDetector(StereoCamera* stereoCamera) {
		this->stereoCamera = stereoCamera;
	}

	void detect();
};



#endif //DETECTOR_H