#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/opencv.hpp>

/**
 *
 */
class Camera {
private:

protected:
	virtual bool openCamera() = 0;
	virtual bool closeCamera() = 0;
	virtual cv::Mat getImage() = 0;

public:
	Camera() {

	}

	cv::Mat getSingleImage();
	cv::Mat getImageOnButtonPress();
	std::vector<cv::Mat> getImagesOnButtonPress(int numImages);
	std::vector<cv::Mat> getImagesUsingDelay(int numImages, int delay);
	std::vector<cv::Mat> getImagesUntilCharacter(char stopChar);
};

/**
 *
 */
class IpCamera : public Camera {
private:
	cv::VideoCapture video;
	std::string ipAddr;

protected:
	bool openCamera();
	bool closeCamera();
	cv::Mat getImage();

public:
	IpCamera(std::string);

	std::string getIpAddr() {
		return this->ipAddr;
	}
	void setIpAddr(std::string ipAddr) {
		this->ipAddr = ipAddr;
	}
};

#endif // CAMERA_H