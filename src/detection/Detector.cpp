#include <opencv2/opencv.hpp>

#include "Detector.h"

std::vector<cv::KeyPoint> Detector::identifyObjects(cv::Mat image) {
	std::vector<cv::KeyPoint> keypoints;

	//cv::bitwise_not(image, image);

	cv::SimpleBlobDetector::Params params;
	params.blobColor = 255;
	params.minDistBetweenBlobs = 1;
	params.filterByInertia = false;
	params.filterByConvexity = false;
	params.filterByColor = false;
	params.filterByCircularity = false;
	params.filterByArea = true;
	params.minArea = 10;
	params.maxArea = 10000000;

	cv::SimpleBlobDetector blobDetector(params);
	cv::imshow("", image);
	cv::waitKey();
	blobDetector.detect(image, keypoints);

	std::cout << keypoints.size() << std::endl;

	Mat im_with_keypoints;
	cv::drawKeypoints( image, keypoints, im_with_keypoints, cv::Scalar(0,0,255), cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	if (true) {
		cv::imshow("", im_with_keypoints);
		cv::waitKey();
	}

	return keypoints;
}