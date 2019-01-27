#include "Detector.h"

#include <opencv2/opencv.hpp>

DetectorResults* BackgroundSubtractionDetector::detect() {
	DetectorResults* detectorResults = new DetectorResults();

	std::vector<cv::Mat> images = stereoCamera->getImages();
	std::vector<cv::Mat> thresholdedImages;
	for (cv::Mat image : images) {
		// Threshold images using user supplied config
		thresholdImage(image);

		// Detect objects in thresholded image
		identifyObjects(image);
	}

	return detectorResults;
}


/*
 *
 */
cv::Mat BackgroundSubtractionDetector::thresholdImage(cv::Mat image) {
	cv::Mat imageHsv, thresholdedImage;
	cv::Mat backgroundImageHsv;

	cv::cvtColor(image, imageHsv, cv::COLOR_BGR2HSV);

	return thresholdedImage;
}