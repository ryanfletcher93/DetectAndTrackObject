#include <opencv2/opencv.hpp>

#include "Detector.h"

#include <vector>
#include <string>
#include <algorithm>

using std::string;
using std::min;
using std::max;

using cv::setTrackbarPos;
using cv::namedWindow;
using cv::createTrackbar;

std::vector<cv::KeyPoint> Detector::identifyObjects(cv::Mat image) {
	std::vector<cv::KeyPoint> keypoints;

	cv::SimpleBlobDetector::Params params;
	params.blobColor = 0;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.minArea = 1000;
	params.maxArea = 100000000000000000;

	cv::SimpleBlobDetector blobDetector(params);
	blobDetector.detect(image, keypoints);

	std::cout << keypoints.size() << std::endl;

	Mat im_with_keypoints;
	cv::drawKeypoints( image, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	if (false) {
		cv::imshow("", im_with_keypoints);
		cv::waitKey();
	}

	return keypoints;
}


const int max_value_H = 360/2;
const int max_value = 255;
string window_capture_name = "Video Capture";
string window_detection_name = "Object Detection";
int low_H = 0, low_S = 0, low_V = 0;
int high_H = max_value_H, high_S = max_value, high_V = max_value;



static void on_low_H_thresh_trackbar(int, void *)
{
    low_H = min(high_H-1, low_H);
    setTrackbarPos("Low H", window_detection_name, low_H);
}
static void on_high_H_thresh_trackbar(int, void *)
{
    high_H = max(high_H, low_H+1);
    setTrackbarPos("High H", window_detection_name, high_H);
}
static void on_low_S_thresh_trackbar(int, void *)
{
    low_S = min(high_S-1, low_S);
    setTrackbarPos("Low S", window_detection_name, low_S);
}
static void on_high_S_thresh_trackbar(int, void *)
{
    high_S = max(high_S, low_S+1);
    setTrackbarPos("High S", window_detection_name, high_S);
}
static void on_low_V_thresh_trackbar(int, void *)
{
    low_V = min(high_V-1, low_V);
    setTrackbarPos("Low V", window_detection_name, low_V);
}
static void on_high_V_thresh_trackbar(int, void *)
{
    high_V = max(high_V, low_V+1);
    setTrackbarPos("High V", window_detection_name, high_V);
}



void ColourDetector::performManualColourAnalysis(cv::Mat image) {

    namedWindow(window_detection_name);
    // Trackbars to set thresholds for HSV values
    createTrackbar("Low H", window_detection_name, &low_H, max_value_H, on_low_H_thresh_trackbar);
    createTrackbar("High H", window_detection_name, &high_H, max_value_H, on_high_H_thresh_trackbar);
    createTrackbar("Low S", window_detection_name, &low_S, max_value, on_low_S_thresh_trackbar);
    createTrackbar("High S", window_detection_name, &high_S, max_value, on_high_S_thresh_trackbar);
    createTrackbar("Low V", window_detection_name, &low_V, max_value, on_low_V_thresh_trackbar);
    createTrackbar("High V", window_detection_name, &high_V, max_value, on_high_V_thresh_trackbar);

	cv::Mat hsvImage;
	while (true) {
		cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

		cv::Mat mask;
		cv::inRange(hsvImage, cv::Scalar(low_H, low_S, low_V), cv::Scalar(high_H, high_S, high_V), mask);

		cv::imshow(window_detection_name, mask);
		cv::waitKey();
	}
}


std::vector<cv::KeyPoint> ColourDetector::identifyObjectsByColour(cv::Mat image, cv::Scalar min, cv::Scalar max) {

	bool showImages = false;

	cv::Mat hsvImage;
	cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

	cv::Mat thresholdedImage;
	cv::inRange(hsvImage, min, max, thresholdedImage);


	if (showImages) {
		cv::imshow("Thresholded image", thresholdedImage);
		cv::waitKey();
	}

	std::vector<cv::KeyPoint> res = identifyObjects(thresholdedImage);
	return res;
}