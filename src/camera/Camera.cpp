#include "Camera.h"

#include <opencv2/opencv.hpp>

/*
 *
 */
cv::Mat Camera::getSingleImage() {
	cv::Mat img;

	openCamera();
	img = getImage();
	closeCamera();

	return img;
}


/*
 *
 */
cv::Mat Camera::getImageOnButtonPress() {
	cv::Mat res;

	std::cout << "Press enter to take image of camera" << std::endl;
	getchar();

	res = getSingleImage();

	return res;
}


/*
 *
 */
std::vector<cv::Mat> Camera::getImagesOnButtonPress(int numImages) {
	std::vector<cv::Mat> images;

	openCamera();

	std::cout << "Press enter to take image of camera" << std::endl;

	cv::Mat tempMat;
	for (int i=0; i<numImages; i++) {
		std::cout << i << ": ";
		getchar();
		tempMat = getImage();
		images.push_back(tempMat);
	}

	closeCamera();

	return images;
}


/**
 *
 */
std::vector<cv::Mat> Camera::getImagesUntilCharacter(char stopChar) {
	std::vector<cv::Mat> images;

	openCamera();

	cv::Mat tempMat;
	char lastChar;
	while ((lastChar = getchar()) != stopChar) {
		tempMat = getImage();
		images.push_back(tempMat);
	}

	closeCamera();

	return images;
}


/*
 *
 */
std::vector<cv::Mat> Camera::getImagesUsingDelay(int numImages, int delay) {
	std::vector<cv::Mat> images;

	openCamera();

	cv::Mat tempMat;
	for (int i=0; i<numImages; i++) {
		tempMat = getImage();
		images.push_back(tempMat);

		// TODO: set delay

	}

	closeCamera();

	return images;
}


/*
 *
 */
void Camera::getCameraIntrinsicsFromFile(std::string filePath) {
	cv::FileStorage fs;
	fs.open(filePath , cv::FileStorage::READ);

	fs["K"] >> this->K;
	fs["D"] >> this->D;
	fs["BOARD_WIDTH"] >> this->boardWidth;
	fs["BOARD_HEIGHT"] >> this->boardHeight;
	fs["SQUARE_SIZE"] >> this->squareSize;
}