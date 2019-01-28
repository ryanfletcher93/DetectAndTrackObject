#include "Detector.h"

#include <opencv2/opencv.hpp>


DetectorResults* BackgroundSubtractionDetector::detect() {
	DetectorResults* detectorResults = new DetectorResults();

	std::array<cv::Mat, 2> images = stereoCamera->getImages();
	std::array<cv::Mat, 2> thresholdedImages;
	for (cv::Mat image : images) {
		// Threshold images using user supplied config
		cv::Mat thresholdedImage = subtractBackground(image);

		// Detect objects in thresholded image
		identifyObjects(thresholdedImage);
	}

	return detectorResults;
}


/*
 *
 */
cv::Mat BackgroundSubtractionDetector::subtractBackground(cv::Mat image) {
	cv::Mat imageHsv, thresholdedImage;
	cv::Mat backgroundImageHsv;

	cv::cvtColor(image, imageHsv, cv::COLOR_BGR2HSV);

	cv::absdiff(imageHsv, backgroundImageHsv, thresholdedImage);

	return thresholdedImage;
}