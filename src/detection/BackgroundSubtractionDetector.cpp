#include "Detector.h"

#include <opencv2/opencv.hpp>

using std::array;

using cv::Mat;


DetectorResults* BackgroundSubtractionDetector::detect() {
	DetectorResults* detectorResults = new DetectorResults();

	array<Mat, 2> images = stereoCamera->getImages();
	array<Mat, 2> thresholdedImages;

	Mat thresholdedImage1 = subtractBackground(images[0], backgroundImage1);
	Mat thresholdedImage2 = subtractBackground(images[1], backgroundImage2);

	return detectorResults;
}


/*
 *
 */
Mat BackgroundSubtractionDetector::subtractBackground(Mat image, Mat backgroundImage) {
	Mat imageRgb, imageGray, thresholdedImage;

	//cv::cvtColor(backgroundImage, backgroundImage, cv::COLOR_RGB2HSV);
	cv::cvtColor(backgroundImage, backgroundImage, cv::COLOR_RGB2HSV);

	thresholdedImage = image - backgroundImage;

	//cv::absdiff(image, backgroundImage, thresholdedImage);

	cv::cvtColor(thresholdedImage, thresholdedImage, cv::COLOR_HSV2RGB);
	//cv::cvtColor(thresholdedImage, thresholdedImage, cv::COLOR_RGB2GRAY);

	cv::imshow("Thresholeded image", thresholdedImage);
	cv::waitKey();

	return thresholdedImage;
}

/*
 */
void BackgroundSubtractionDetector::setBackgroundImages(Mat backgroundImg1, Mat backgroundImg2) {
	this->backgroundImage1 = backgroundImg1;
	this->backgroundImage2 = backgroundImg2;
}