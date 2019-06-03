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

	
	identifyObjects(thresholdedImage1);
	//identifyObjects(thresholdedImage2);

	return detectorResults;
}


/*
 *
 */
Mat BackgroundSubtractionDetector::subtractBackground(Mat image, Mat backgroundImage) {
	Mat imageRgb, imageGray, thresholdedImage;

	cv::cvtColor(backgroundImage, backgroundImage, cv::COLOR_HSV2RGB);

	thresholdedImage = image - backgroundImage;

	// Go through and find average value of 3 channels for each pixel
	cv::Mat1b aveMat(thresholdedImage.rows, thresholdedImage.cols, uchar(0));
	for (int i=0; i<thresholdedImage.rows; i++) {
		for (int j=0; j<thresholdedImage.cols; j++) {
			cv::Mat3b mat3b = thresholdedImage;
			const cv::Vec3b& v = mat3b(i, j);

			if (i > 100 && i < 400 && j > 400 && j < 600) {
				aveMat(i, j) = static_cast<uchar>(160);
				//aveMat(i,j) = static_cast<uchar>(round((double(v[0]) + double(v[1]) + double(v[2]))/3.0));
			}
		}
	}

	cv::threshold(thresholdedImage, thresholdedImage, 130, 255, cv::THRESH_BINARY);
	cv::cvtColor(thresholdedImage, thresholdedImage, cv::COLOR_RGB2GRAY);

	cv::threshold(aveMat, aveMat, 100, 255, cv::THRESH_BINARY);

	if (false) {
		cv::imshow("Thresholeded image", aveMat);
		cv::waitKey();
	}

	return aveMat;
}

/*
 */
void BackgroundSubtractionDetector::setBackgroundImages(Mat backgroundImg1, Mat backgroundImg2) {
	this->backgroundImage1 = backgroundImg1;
	this->backgroundImage2 = backgroundImg2;
}