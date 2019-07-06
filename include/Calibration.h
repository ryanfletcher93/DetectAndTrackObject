#ifndef CALIBRATION_H
#define CALIBRATION_H

#include <opencv2/opencv.hpp>

using std::string;
using std::vector;

using cv::Mat;


/*
 *
 */
class StereoParameters {
protected:
	Mat R1, R2, P1, P2, Q;

public:
	StereoParameters(Mat R1, Mat R2, Mat P1, Mat P2, Mat Q);
};

/**
 *
 */
class CalibrationParameters {
public:
	CalibrationParameters();

};


#endif // CALIBRATION_H