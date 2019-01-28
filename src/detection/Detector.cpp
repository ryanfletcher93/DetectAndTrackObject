#include <opencv2/opencv.hpp>

#include "Detector.h"

std::vector<cv::KeyPoint> Detector::identifyObjects(cv::Mat image) {
	std::vector<cv::KeyPoint> keypoints;

	cv::SimpleBlobDetector::Params params;
	params.blobColor = 255;
	params.minDistBetweenBlobs = 1;
	params.filterByColor = true;
	params.filterByArea = false;
	params.filterByCircularity = false;
	params.filterByConvexity = false;
	params.filterByInertia = false;

	cv::SimpleBlobDetector blobDetector(params);
	blobDetector.detect(image, keypoints);

	return keypoints;
}