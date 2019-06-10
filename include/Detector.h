#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/opencv.hpp>

#include "StereoCamera.h"

#include <vector>

using std::string;

using cv::Mat;

class DetectorResults {
private:


public:
		
};


class BackgroundSubtraction {
private:
    Mat backgroundImg1;
    Mat backgroundImg2;

public:
    virtual void setBackgroundImages(Mat img1, Mat img2) {
        this->backgroundImg1 = img1;
        this->backgroundImg2 = img2;
    }
};


class Detector {
protected:
	StereoCamera* stereoCamera;

	/*
	 *
	 */
	virtual std::vector<cv::KeyPoint> identifyObjects(cv::Mat image);

public:
	virtual void setBackgroundImages(Mat img1, Mat img2) {};
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
	Mat thresholdImage(Mat image);

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
class BackgroundSubtractionDetector : public Detector, public BackgroundSubtraction {
private:
	Mat backgroundImage1;
	Mat backgroundImage2;

	Mat thresholdImage(Mat image);

public:
	/*
	 *
	 */
	BackgroundSubtractionDetector() {}


	BackgroundSubtractionDetector(StereoCamera* stereoCamera) {
		this->stereoCamera = stereoCamera;
	}

	void setBackgroundImages(Mat backgroundImage1, Mat backgroundImage2);

	Mat subtractBackground(Mat image, Mat backgroundImage);

	DetectorResults* detect();
};


/*
 *
 */
class ColourDetector : public Detector{
private:


protected:
	//std::vector<cv::KeyPoint> identifyObjects(cv::Mat image);

public:
	std::vector<cv::KeyPoint> identifyObjectsByColour(cv::Mat image, cv::Scalar min, cv::Scalar max);

	void performManualColourAnalysis(cv::Mat image);

	DetectorResults* detect() {};
};


#endif //DETECTOR_H