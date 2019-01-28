#include "Detector.h"

#include <opencv2/opencv.hpp>

/*
 *
 */
ThresholdDetector::ThresholdDetector() {

}

DetectorResults* ThresholdDetector::detect() {
	DetectorResults* detectorResults = new DetectorResults();

	std::array<cv::Mat, 2> images = stereoCamera->getImages();
	std::array<cv::Mat, 2> thresholdedImages;
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
cv::Mat ThresholdDetector::thresholdImage(cv::Mat image) {
	cv::Mat imageHsv, thresholdedImage;
	cv::Mat backgroundImageHsv;

	cv::cvtColor(image, imageHsv, cv::COLOR_BGR2HSV);

	return thresholdedImage;
}